/*
 * Generator.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

#include <vector>

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Default constructor.
 */
Generator::Generator() :
	mSystem(NULL),
	mClock(NULL),
	mEnvironment(NULL),
	mParallel(new PACC::Threading::Condition()),
	mSequential(new PACC::Threading::Semaphore(0))
{}

/*!
 * \brief Construct a generator as a copy of an original.
 * \param inOriginal A const reference to the original.
 */
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

/*!
 * \brief Construct a generator with specific system, clock and environment.
 * \param inSystem A handle to the system.
 * \param inClock A handle to the clock.
 * \param inEnvironment A handle to the environment.
 */
Generator::Generator(Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
	mSystem(inSystem),
	mClock(inClock),
	mEnvironment(inEnvironment),
	mParallel(new PACC::Threading::Condition()),
	mSequential(new PACC::Threading::Semaphore(0))
{}

/*!
 * \brief Destructor.
 */
Generator::~Generator() {
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSubThreads[i]->setPosition(GenerationThread::eEND);
	}
	mParallel->lock();
	mParallel->broadcast();
	mParallel->unlock();
}

/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
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

	refresh();

	// read and set seed for each randomizer
	readRandomizerInfo(inIter++);

	// read population profiles
	readProfiles(inIter);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Generator::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	// write the seeds of randomizers
	this->writeRandomizerInfo(ioStreamer, inIndent);

	// write profiles
	this->writeProfiles(ioStreamer, inIndent);
}

/*!
 * \brief  Generate a quantity of individuals from a specific profile using specific prefix and start index.
 * \param  inProfile A const reference to the profile name.
 * \param  inSize The size of sub-population to generate.
 * \param  inPrefix A const reference to the ID prefix of new individuals.
 * \param  inStartingIndex The start index of individuals.
 * \return A handle to a bag of newly generated individuals.
 * \throw  SCHNAPS::Core::RunTimeException if specific profile does not exist.
 */
Individual::Bag::Handle Generator::generate(const std::string& inProfile, unsigned int inSize, const std::string& inPrefix, unsigned int inStartingIndex) {
	schnaps_StackTraceBeginM();
	ProfileMap::iterator lIterProfiles = mProfiles.find(inProfile);
	if (lIterProfiles == mProfiles.end()) {
		std::ostringstream lOSS;
		lOSS << "The profile '" << inProfile << "' does not exist; ";
		lOSS << "could not generate individuals.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}

	GenProfile::Handle lProfile = lIterProfiles->second;

	// compute variables to erase
	Core::StringArray::Handle lEraseVariable = new Core::StringArray();
	std::vector<std::string> lVariables = lProfile->getDemography().getVariableList();
	for (unsigned int i = 0; i < lVariables.size(); i++) {
		if (lProfile->getIndividualModel().find(lVariables[i]) == lProfile->getIndividualModel().end()) {
			// if variable not in individual model, add to variable list for thrash
			lEraseVariable->push_back(lVariables[i]);
		}
	}

	// backup randomizer seeds
	Core::ULongArray lBackupSeed;
	Core::StringArray lBackupState;

	// for computing threads sub-size to generate
	unsigned int lQuotient = inSize / mSubThreads.size();
	unsigned int lRemainder = inSize % mSubThreads.size();
	unsigned int lSubSize;

	// init Threads
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mContext[i]->setGenProfile(Core::castHandleT<GenProfile>(lProfile->deepCopy(*mSystem)));

		// compute sub-size
		lSubSize = lQuotient;
		if (i < lRemainder) {
			lSubSize++;
		}

		mSubThreads[i]->setPosition(GenerationThread::eGENERATION);
		mSubThreads[i]->setGenerationInfo(lSubSize, inPrefix, inStartingIndex, lEraseVariable);
		inStartingIndex += lSubSize;

		// backup and reset randomizer info
		lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
		lBackupState.push_back(mSystem->getRandomizer(i).getState());
		mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
	}

	// build individuals
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

		// reset old randomizer info
		mRandomizerCurrentSeed[i] = mSystem->getRandomizer(i).getSeed();
		mRandomizerCurrentState[i] = mSystem->getRandomizer(i).getState();
		mSystem->getRandomizer(i).reset(lBackupSeed[i], lBackupState[i]);
	}

	return lIndividuals;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::Individual::Bag::Handle SCHNAPS::Simulation::Generator::generate(const std::string&, unsigned int, const std::string&, unsigned int)");
}

/*!
 * \brief Build individuals using specific thread.
 * \param inThread A handle to the executing thread.
 */
void Generator::buildIndividuals(GenerationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
	GenerationContext::Handle lContext = inThread->getContextHandle();
	std::stringstream lID;
	unsigned int lIndividualIndex = inThread->getStartingIndex();

	inThread->getIndividuals().clear();
	inThread->getIndividuals().reserve(inThread->getSize());
	
	std::vector<std::string> lDemographyVariables = lContext->getGenProfile().getDemography().getVariableList();
	std::vector<std::string> lSimulationVariables = lContext->getGenProfile().getSimulationVariables().getVariableList();

	for (unsigned int i = 0; i < inThread->getSize(); i++) {
		lID.str("");
		lID << inThread->getPrefix() << "/" << lIndividualIndex++;

		inThread->getIndividuals().push_back(new Individual(lID.str()));
		lContext->setIndividual(inThread->getIndividuals().back());

		do {
			lContext->getIndividual().getState().clear();
			
			for (unsigned int j = 0; j < lDemographyVariables.size(); j++) {
				lContext->getIndividual().getState().insertVariable(
					lDemographyVariables[j],
					lContext->getGenProfile().getDemography().getVariableInitTree(lDemographyVariables[j]).interpret(*lContext));
			}
			// retry until a valid individual is created
		} while (Core::castHandleT<Core::Bool>(lContext->getGenProfile().getAcceptFunction().interpret(*lContext))->getValue() == false);

		// add simulation variables
		for (unsigned int j = 0; j < lSimulationVariables.size(); j++) {
			lContext->getIndividual().getState().insertVariable(
				lSimulationVariables[j],
				lContext->getGenProfile().getSimulationVariables().getVariableInitTree(lSimulationVariables[j]).interpret(*lContext));
		}

		// erase non-wanted demographic variables
		for (unsigned int j = 0; j < inThread->getEraseVariables().size(); j++) {
			lContext->getIndividual().getState().removeVariable(inThread->getEraseVariables()[j]);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::buildIndividuals(SCHNAPS::Simulation::GenerationThread::Handle)");
}

/*!
 * \brief Refresh simulator structure with up-to-date parameters.
 */
void Generator::refresh() {
	schnaps_StackTraceBeginM();
	// create one context per thread + sub threads
	unsigned int lNbThreads_new = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.generator")).getValue();
	unsigned int lNbThreads_old = mContext.size();

	if (lNbThreads_old < lNbThreads_new) {
		for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
			mContext.push_back(new GenerationContext(mSystem, mClock, mEnvironment));
			mContext.back()->setThreadNb(i);

			mSubThreads.push_back(new GenerationThread(mParallel, mSequential, mContext.back()));
			mSequential->wait();
		}
	} else if (lNbThreads_new < lNbThreads_old) {
		// TODO: remove unused contexts and subthreads
	}
	
	// update generator randomizers information
	mRandomizerInitSeed.resize(lNbThreads_new, 0);
	mRandomizerInitState.resize(lNbThreads_new, "");

	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::refresh()");
}

void Generator::clearRandomizer() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
		mRandomizerCurrentSeed[i] = 0;
		mRandomizerCurrentState[i] = "";
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::clearRandomizer()");
}

void Generator::resetRandomizer() {
	schnaps_StackTraceBeginM();
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::resetRandomizer()");
}

// private functions

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

	unsigned int lThreadsGenerator = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.generator")).getValue();
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
			mRandomizerInitSeed[lThreadNumber] = SCHNAPS::str2uint(lChild->getAttribute("seed"));
			mRandomizerInitState[lThreadNumber] = lChild->getAttribute("state");
			lThreadNumber++;
		}
	}

	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::readRandomizerInfo(PACC::XML::ConstIterator)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::readProfile(PACC::XML::ConstIterator, SCHNAPS::Simulation::Profile&)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::writeRandomizerInfo(PACC::XML::Streamer&, bool)");
}

void Generator::writeProfiles(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Profiles");
	for (ProfileMap::const_iterator lIt = mProfiles.begin(); lIt != mProfiles.end(); lIt++) {
		lIt->second->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::writeProfiles(PACC::XML::Streamer&, bool)");
}
