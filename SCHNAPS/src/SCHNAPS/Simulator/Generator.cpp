/*
 * Generator.cpp
 *
 *  Created on: 2010-03-29
 *  Author: Audrey Durand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vector>

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

Generator::Generator() :
		mSystem(NULL),
		mClock(NULL),
		mEnvironment(NULL),
		mParallel(new PACC::Threading::Condition()),
		mSequential(new PACC::Threading::Semaphore(0))
{}

Generator::Generator(const Generator& inOriginal) :
		mSystem(inOriginal.mSystem),
		mClock(inOriginal.mClock),
		mEnvironment(inOriginal.mEnvironment),
		mRandomizerInitSeed(inOriginal.mRandomizerInitSeed),
		mRandomizerInitState(inOriginal.mRandomizerInitState),
		mRandomizerCurrentSeed(inOriginal.mRandomizerCurrentSeed),
		mRandomizerCurrentState(inOriginal.mRandomizerCurrentState),
		mContext(inOriginal.mContext),
		mSubThreads(inOriginal.mSubThreads),
		mParallel(inOriginal.mParallel),
		mSequential(inOriginal.mSequential),
		mProfiles(inOriginal.mProfiles)
{}

Generator::Generator(core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
		mSystem(inSystem),
		mClock(inClock),
		mEnvironment(inEnvironment),
		mParallel(new PACC::Threading::Condition()),
		mSequential(new PACC::Threading::Semaphore(0))
{}

Generator::~Generator() {
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSubThreads[i]->setPosition(GenerationThread::eEND);
	}
	mParallel->lock();
	mParallel->broadcast();
	mParallel->unlock();
}

void Generator::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" + getName() + "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading generator\n");
#endif

		inIter = inIter->getFirstChild();

		init();

		// Read and set seed for each randomizer
		readRandomizerInfo(inIter++);

		// Read population profiles
		readProfiles(inIter);
	schnaps_StackTraceEndM("void simulator::Generator::read(PACC::XML::ConstIterator)");
}

void Generator::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	// Write the seeds of randomizers
	this->writeRandomizerInfo(ioStreamer, inIndent);

	// Write profiles
	this->writeProfiles(ioStreamer, inIndent);
}

Individual::Bag::Handle Generator::generate(std::string inProfile, unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex) {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
		if (mProfiles.find(inProfile) == mProfiles.end()) {
			throw schnaps_InternalExceptionM("Could not find profile '" + inProfile + "' in generator!");
		}
#else
		schnaps_AssertM(mProfiles.find(inProfile) != mProfiles.end());
#endif

		GenProfile::Handle lProfile = mProfiles.find(inProfile)->second;

		// Compute variables to erase
		core::StringArray::Handle lEraseVariable = new core::StringArray();
		for (Demography::const_iterator lIt = lProfile->getDemography().begin(); lIt != lProfile->getDemography().end(); lIt++) {
			if (lProfile->getIndividualModel().find(lIt->first) == lProfile->getIndividualModel().end()) {
				// If variable not in individual model, thrash
				lEraseVariable->push_back(lIt->first);
			}
		}

		// Backup randomizer seeds
		core::ULongArray lBackupSeed;
		core::StringArray lBackupState;

		// For computing threads sub-size to generate
		unsigned int lQuotient = inSize / mSubThreads.size();
		unsigned int lRemainder = inSize % mSubThreads.size();
		unsigned int lSubSize;

		// Init Threads
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mContext[i]->setGenProfile(lProfile->deepCopy(*mSystem));

			// Compute sub-size
			lSubSize = lQuotient;
			if (i < lRemainder) {
				lSubSize++;
			}

			mSubThreads[i]->setPosition(GenerationThread::eGENERATION);
			mSubThreads[i]->setGenerationInfo(lSubSize, inPrefix, inStartingIndex, lEraseVariable);
			inStartingIndex += lSubSize;

			// Backup and reset randomizer info
			lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
			lBackupState.push_back(mSystem->getRandomizer(i).getState());
			mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
		}

		// Build individuals
#ifdef SCHNAPS_FULL_DEBUG
			printf("Generating %u individuals\n", inSize);
#endif
		mParallel->lock();
		mParallel->broadcast();
		mParallel->unlock();
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mSequential->wait();
		}

		Individual::Bag::Handle lIndividuals = new Individual::Bag();
		lIndividuals->reserve(inSize);
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			lIndividuals->insert(lIndividuals->end(), mSubThreads[i]->getIndividuals().begin(), mSubThreads[i]->getIndividuals().end());

			// Reset old randomizer info
			mRandomizerCurrentSeed[i] = mSystem->getRandomizer(i).getSeed();
			mRandomizerCurrentState[i] = mSystem->getRandomizer(i).getState();
			mSystem->getRandomizer(i).reset(lBackupSeed[i], lBackupState[i]);
		}

		return lIndividuals;
	schnaps_StackTraceEndM("simulator::Individual::Bag::Handle simulator::Generator::generate(std::string, unsigned int, std::string, unsigned int)");
}

void Generator::buildIndividuals(GenerationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
		GenerationContext::Handle lContext = inThread->getContextHandle();
		std::stringstream lID;
		unsigned int lIndividualIndex = inThread->getStartingIndex();

		inThread->getIndividuals().clear();
		inThread->getIndividuals().reserve(inThread->getSize());

		for (unsigned int i = 0; i < inThread->getSize(); i++) {
			lID.str("");
			lID << inThread->getPrefix() << "/" << lIndividualIndex++;

			inThread->getIndividuals().push_back(new Individual(lID.str()));
			lContext->setIndividual(inThread->getIndividuals().back());

			do {
				lContext->getIndividual().getState().clear();

				for (unsigned int j = 0; j < lContext->getGenProfile().getDemography().size(); j++) {
					lContext->getIndividual().getState().insert(std::pair<std::string, core::Atom::Handle>(
							lContext->getGenProfile().getDemography()[j].first,
							core::castHandleT<core::Atom>(lContext->getGenProfile().getDemography()[j].second->interpret(*lContext))));
				}
				// Retry until a valid individual is created
			} while (core::castHandleT<core::Bool>(lContext->getGenProfile().getAcceptFunction().interpret(*lContext))->getValue() == false);

			// Add simulation variables
			for (unsigned int j = 0; j < lContext->getGenProfile().getSimulationVariables().size(); j++) {
				lContext->getIndividual().getState().insert(std::pair<std::string, core::Atom::Handle>(
						lContext->getGenProfile().getSimulationVariables()[j].first,
						core::castHandleT<core::Atom>(lContext->getGenProfile().getSimulationVariables()[j].second->interpret(*lContext))));
			}

			// Erase non-wanted demographic variables
			for (unsigned int j = 0; j < inThread->getEraseVariables().size(); j++) {
				lContext->getIndividual().getState().erase(inThread->getEraseVariables()[j]);
			}
		}
	schnaps_StackTraceEndM("void simulator::Generator::buildIndividuals(simulator::GenerationThread*)");
}

void Generator::clearRandomizer() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
		mRandomizerCurrentSeed[i] = 0;
		mRandomizerCurrentState[i] = "";
	}
	schnaps_StackTraceEndM("void simulator::Generator::clearRandomizer()");
}

void Generator::resetRandomizer() {
	schnaps_StackTraceBeginM();
		mRandomizerCurrentSeed = mRandomizerInitSeed;
		mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void simulator::Generator::resetRandomizer()");
}

// Private functions

void Generator::init() {
	schnaps_StackTraceBeginM();
		// Create one context per thread + sub threads
		unsigned int lNbThreads_new = core::castHandleT<core::UInt>(mSystem->getParameters()["threads.generator"])->getValue();
		unsigned int lNbThreads_old = mContext.size();

		if (lNbThreads_old < lNbThreads_new) {
			for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
				mContext.push_back(new GenerationContext(mSystem, mClock, mEnvironment));
				mContext.back()->setThreadNb(i);

				mSubThreads.push_back(new GenerationThread(mParallel, mSequential, mContext.back()));
				mSequential->wait();
			}
		}
	schnaps_StackTraceEndM("void simulator::Simulator::readInput(PACC::XML::ConstIterator)");
}

void Generator::readRandomizerInfo(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "RandomizerInfo") {
			std::ostringstream lOSS;
			lOSS << "tag <RandomizerInfo> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading randomizer info\n");
#endif

		unsigned int lThreadsGenerator = core::castHandleT<core::UInt>(mSystem->getParameters()["threads.generator"])->getValue();
		mRandomizerInitSeed.clear();
		mRandomizerInitState.clear();
		mRandomizerInitSeed.resize(lThreadsGenerator, 0);
		mRandomizerInitState.resize(lThreadsGenerator, "");

		if ((inIter->getChildCount() != 0) && (inIter->getChildCount() != lThreadsGenerator)) {
			throw schnaps_IOExceptionNodeM(*inIter, "expected a randomizer per thread!");
		}

		unsigned int lThreadNumber = 0;
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "Randomizer") {
					std::ostringstream lOSS;
					lOSS << "tag <Randomizer> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				mRandomizerInitSeed[lThreadNumber] = core::str2uint(lChild->getAttribute("seed"));
				mRandomizerInitState[lThreadNumber] = lChild->getAttribute("state");
				lThreadNumber++;
			}
		}

		mRandomizerCurrentSeed = mRandomizerInitSeed;
		mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void simulator::Generator::readRandomizerInfo(PACC::XML::ConstIterator)");
}

void Generator::readProfiles(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Profiles") {
			std::ostringstream lOSS;
			lOSS << "tag <Profiles> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading profiles\n");
#endif

		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				mProfiles.insert(std::pair<std::string, GenProfile::Handle>(lChild->getAttribute("label"), new GenProfile()));
				mProfiles[lChild->getAttribute("label")]->readWithSystem(lChild, *mSystem);
			}
		}
	schnaps_StackTraceEndM("void simulator::Generator::readProfile(PACC::XML::ConstIterator, simulator::Profile&)");
}

void Generator::writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("RandomizerInfo");
		for (unsigned int i = 0; i < mRandomizerInitState.size(); i++) {
			ioStreamer.openTag("Randomizer");
			ioStreamer.insertAttribute("seed", mRandomizerInitSeed[i]);
			ioStreamer.insertAttribute("state", mRandomizerInitState[i]);
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void simulator::Generator::writeRandomizerInfo(PACC::XML::Streamer&, bool)");
}

void Generator::writeProfiles(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Profiles");
		for (ProfileMap::const_iterator lIt = mProfiles.begin(); lIt != mProfiles.end(); lIt++) {
			lIt->second->write(ioStreamer, inIndent);
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void simulator::Generator::writeProfiles(PACC::XML::Streamer&, bool)");
}
