/*
 * Simulator.cpp
 *
 *  Created on: 2009-03-12
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

Simulator::Simulator() :
		mSystem(new SCHNAPS::Core::System()),
		mClock(new Clock()),
		mEnvironment(new Environment()),
		mPopulationManager(new PopulationManager(mSystem, mClock, mEnvironment)),
		mBlackBoard(new BlackBoard()),
		mWaitingQMaps(new WaitingQMaps()),
		mParallel(new PACC::Threading::Condition()),
		mSequential(new PACC::Threading::Semaphore(0)),
		mBlackBoardWrt(new PACC::Threading::Semaphore(1))
{
	// Initialize factory with elements
	mSystem->getFactory().insertAllocator("State", new SCHNAPS::Simulation::State::Alloc());
	mSystem->getFactory().insertAllocator("Individual", new SCHNAPS::Simulation::Individual::Alloc());
	mSystem->getFactory().insertAllocator("Environment", new SCHNAPS::Simulation::Environment::Alloc());
	mSystem->getFactory().insertAllocator("Population", new SCHNAPS::Simulation::Population::Alloc());
	mSystem->getFactory().insertAllocator("Generator", new SCHNAPS::Simulation::Generator::Alloc());
	mSystem->getFactory().insertAllocator("Process", new SCHNAPS::Simulation::Process::Alloc());
	mSystem->getFactory().insertAllocator("Clock", new SCHNAPS::Simulation::Clock::Alloc());

	// Initialize required parameters
	mSystem->getParameters().addParameter("print.prefix", new SCHNAPS::Core::String(""));
	mSystem->getParameters().addParameter("print.input", new SCHNAPS::Core::Bool(false));
	mSystem->getParameters().addParameter("print.output", new SCHNAPS::Core::Bool(true));
	mSystem->getParameters().addParameter("print.log", new SCHNAPS::Core::Bool(true));
	mSystem->getParameters().addParameter("print.conf", new SCHNAPS::Core::Bool(false));
	mSystem->getParameters().addParameter("threads.simulator", new SCHNAPS::Core::UInt(1));
	mSystem->getParameters().addParameter("threads.generator", new SCHNAPS::Core::UInt(1));
}

Simulator::~Simulator() {
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSubThreads[i]->setPosition(SimulationThread::eEND);
	}
	mParallel->lock();
	mParallel->broadcast();
	mParallel->unlock();
}

void Simulator::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		inIter = inIter->getFirstChild();

		// System must be read first
		mSystem->read(inIter++);

		// Initialize simulator
		init();

		// Read simulation input configuration
		readInput(inIter++);

		// Read simulation configuration
		readSimulation(inIter++);

		// Read simulation output configuration
		readOutput(inIter);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::read(PACC::XML::ConstIterator)");
}

void Simulator::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	mSystem->write(ioStreamer, inIndent);
	writeInput(ioStreamer, inIndent);
	writeSimulation(ioStreamer, inIndent);
	writeOutput(ioStreamer, inIndent);
}

void Simulator::configure(const std::string& inParameters) {
	schnaps_StackTraceBeginM();
		mSystem->getParameters().readStr(inParameters);

		// Initialize simulator
		init();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::configure(const std::string&)");
}

std::string Simulator::getConfiguration() {
	schnaps_StackTraceBeginM();
		return mSystem->getParameters().writeStr();
	schnaps_StackTraceEndM("std::string SCHNAPS::Simulation::getConfiguration()");
}

void Simulator::simulate(std::string inScenarioLabel) {
	schnaps_StackTraceBeginM();
		// Reset simulation structures
		mClock->reset();
		mEnvironment->reset();
		mPopulationManager->getPrefixes().clear();
		mBlackBoard->clear();
		mWaitingQMaps->getEnvironmentWaitingQMap().clear();
		mWaitingQMaps->getIndividualsWaitingQMaps().clear();

		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mSubThreads[i]->resetIndexes();

			mContext[i]->reset();
		}

		// Validate scenario
#ifndef SCHNAPS_NDEBUG
		if (mContext[0]->getScenarios().find(inScenarioLabel) == mContext[0]->getScenarios().end()) {
			throw schnaps_InternalExceptionM("Could not find " + inScenarioLabel + " in scenario list!");
		}
#else
		schnaps_AssertM(mContext[0]->getScenarios().find(inScenarioLabel) != mContext[0]->getScenarios().end());
#endif

		// Current position in simulation
		enum Position {eSTEP, eSUBSTEP};
		Position lPositionEnv;
		bool lSubStep;

		// Population index bounds
		unsigned int lNewIndividuals_LowerBound = 0;

		// Backup randomizer seeds
		SCHNAPS::Core::ULongArray lBackupSeed;
		SCHNAPS::Core::StringArray lBackupState;

		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			// Set subthread on specific scenario
			mSubThreads[i]->setScenarioLabel(inScenarioLabel);

			// Reset randomizer info
			lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
			lBackupState.push_back(mSystem->getRandomizer(i).getState());
			mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
		}

		// Print info
		std::string lPrintPrefix = SCHNAPS::Core::castHandleT<SCHNAPS::Core::String>(mSystem->getParameters()["print.prefix"])->getValue();
		bool lPrintInput = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(mSystem->getParameters()["print.input"])->getValue();
		bool lPrintOutput = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(mSystem->getParameters()["print.output"])->getValue();
		bool lPrintLog = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(mSystem->getParameters()["print.log"])->getValue();
		ogzstream lOGZS;
		std::stringstream lSS;

		if (lPrintInput) {
			lSS.str("");
			lSS << lPrintPrefix << "Input.gz";
			lOGZS.open(lSS.str().c_str(), std::ios::out);
			mEnvironment->print(lOGZS, mOutputParameters.mEnvironment);
		}

		if (lPrintLog) {
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				lSS.str("");
				lSS << lPrintPrefix << "Log_" << i << ".gz";
				mSystem->getLogger(i).open(lSS.str());
			}
		}

		do { // while (mClock->step(*mContext[0]))
#ifdef SCHNAPS_FULL_DEBUG
			printf("Time %lu\n", mClock->getValue());
#endif

			lPositionEnv = eSTEP;
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSubThreads[i]->setPosition(SimulationThread::eSTEP);
			}

			// Add new individuals
			if (mEnvironment->getPopulation().addIndividuals(mPopulationManager->getIndividuals()) == true) {
				// Resize waiting map FIFOs to include new individuals
				mWaitingQMaps->getIndividualsWaitingQMaps().resize(mEnvironment->getPopulation().size());

				// Assign individuals to threads
				for (unsigned int i = 0; i < mSubThreads.size(); i++) {
					mSubThreads[i]->addNewIndexes(lNewIndividuals_LowerBound, mEnvironment->getPopulation().size());
				}

				// Print input if asked
				if (lPrintInput == true) {
					printInput(lOGZS, lNewIndividuals_LowerBound);
				}

				// Reset new individuals bounds
				lNewIndividuals_LowerBound = mEnvironment->getPopulation().size();
			}

			do {
				lSubStep = false;

#ifdef SCHNAPS_FULL_DEBUG
				printf("Processing environment\n");
#endif
				mContext[0]->setIndividual(mEnvironment);
				switch (lPositionEnv) {
				case eSTEP:
					if (mClock->getValue() == 0) {
						// Processing environment scenario
						if (mContext[0]->getScenarios().find(inScenarioLabel)->second.mProcessEnvironment != NULL) {
							mContext[0]->getScenarios().find(inScenarioLabel)->second.mProcessEnvironment->execute(*mContext[0]);
						}
					} else {
						// Processing clock observers
						for (unsigned int j = 0; j < mContext[0]->getClockObservers().mProcessEnvironment.size(); j++) {
							mContext[0]->getClockObservers().mProcessEnvironment[j]->execute(*mContext[0]);
						}
						// Processing current environment FIFO
						while (mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].empty() == false) {
							mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].front()->execute(*mContext[0]);
							mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].pop();
						}
					}
					lPositionEnv = eSUBSTEP;
					break;
				case eSUBSTEP:
					// Processing current environment FIFO
					while (mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].empty() == false) {
						mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].front()->execute(*mContext[0]);
						mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].pop();
					}
					mWaitingQMaps->getEnvironmentWaitingQMap().erase(mClock->getValue());
					break;
				default:
					break;
				}

				// Pushing processes pushed by environment
				if (mContext[0]->getPushList().empty() == false) {
					lSubStep = true;
					while (mContext[0]->getPushList().empty() == false) {
						switch (mContext[0]->getPushList().front().mTarget) {
						case Process::eCurrent:
						case Process::eEnvironment:
							mWaitingQMaps->getEnvironmentWaitingQMap()[mContext[0]->getPushList().front().mTime].push(
									new ProcessPushed(mContext[0]->getPushList().front().mProcess));
							break;
						case Process::eIndividuals:
							for (unsigned int i = 0; i < mEnvironment->getPopulation().size(); i++) {
								mWaitingQMaps->getIndividualsWaitingQMaps()[i][mContext[0]->getPushList().front().mTime].push(
										new ProcessPushed(mContext[0]->getPushList().front().mProcess));
							}
							break;
						default:
							throw schnaps_InternalExceptionM("Undefined process target!");
							break;
						}
						mContext[0]->getPushList().pop_front();
					} // while (mContext[0]->getPushList().empty() == false)
				}

#ifdef SCHNAPS_FULL_DEBUG
				printf("Processing individuals\n");
#endif
				// Processing individuals
				mParallel->lock();
				mParallel->broadcast();
				mParallel->unlock();
				for (unsigned int i = 0; i < mSubThreads.size(); i++) {
					mSequential->wait();
				}

				// Pushing processes pushed by individuals
				if (mBlackBoard->empty() == false) {
					lSubStep = true;
				}
				for (BlackBoard::iterator lIt = mBlackBoard->begin(); lIt != mBlackBoard->end(); lIt++) {
					while (lIt->second.empty() == false) {
						switch (lIt->second.front().mTarget) {
						case Process::eCurrent:
							mWaitingQMaps->getIndividualsWaitingQMaps()[lIt->first][lIt->second.front().mTime].push(
									new ProcessPushed(lIt->second.front().mProcess));
							break;
						case Process::eEnvironment:
							mWaitingQMaps->getEnvironmentWaitingQMap()[lIt->second.front().mTime].push(
									new ProcessPushed(lIt->second.front().mProcess));
							break;
						case Process::eIndividuals:
							for (unsigned int i = 0; i < mEnvironment->getPopulation().size(); i++) {
								mWaitingQMaps->getIndividualsWaitingQMaps()[i][lIt->second.front().mTime].push(
										new ProcessPushed(lIt->second.front().mProcess));
							}
							break;
						default:
							throw schnaps_InternalExceptionM("Undefined process target!");
							break;
						}
						lIt->second.pop_front();
					} // while (lIt->second.empty() == false
				} // for each BlackBoard::PushTracker
				mBlackBoard->clear();
			} while (lSubStep == true);
		} while (mClock->step(*mContext[0]));

		// Reset randomizers old info
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mRandomizerCurrentSeed[i] = mSystem->getRandomizer(i).getSeed();
			mRandomizerCurrentState[i] = mSystem->getRandomizer(i).getState();
			mSystem->getRandomizer(i).reset(lBackupSeed[i], lBackupState[i]);
		}

		// Close log files
		if (lPrintLog) {
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSystem->getLogger(i).close();
			}
		}

		// Close print input file if opened
		if (lPrintInput) {
			lOGZS.close();
		}

		// Print results
		if (lPrintOutput) {
			lSS.str("");
			lSS << lPrintPrefix << "Output.gz";
			lOGZS.open(lSS.str().c_str(), std::ios::out);
			printOutput(lOGZS);
			lOGZS.close();
		}

		// Print summary
		if (lPrintInput || lPrintOutput) {
			lSS.str("");
			lSS << lPrintPrefix << "Summary.gz";
			lOGZS.open(lSS.str().c_str(), std::ios::out);
			printSummary(lOGZS);
			lOGZS.close();
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::;Simulator::simulate(std::string, std::string, std::string)");
}

void Simulator::clearRandomizer() {
	schnaps_StackTraceBeginM();
		for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
			mRandomizerCurrentSeed[i] = 0;
			mRandomizerCurrentState[i] = "";
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::clearRandomizer()");
}

void Simulator::resetRandomizer() {
	schnaps_StackTraceBeginM();
		mRandomizerCurrentSeed = mRandomizerInitSeed;
		mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::resetRandomizer()");
}

void Simulator::processScenario(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
		try {
			SimulationContext& lContext = inThread->getContext();
			const std::vector<unsigned int>& lNewIndexes = inThread->getNewIndexes();

			// If their is a scenario for individuals
			if (lContext.getScenarios().find(inThread->getScenarioLabel())->second.mProcessIndividual != NULL) {
				// Execute the scenario
				for (unsigned int i = 0; i < lNewIndexes.size(); i++) {
					lContext.setIndividual(lContext.getEnvironment().getPopulation()[lNewIndexes[i]]);
					lContext.getScenarios().find(inThread->getScenarioLabel())->second.mProcessIndividual->execute(lContext);
					// Keeping track of processes pushed by individual at index i
					if (lContext.getPushList().empty() == false) {
						inThread->waitBlackBoard();
						inThread->getBlackBoard()[lNewIndexes[i]] = lContext.getPushList();
						inThread->postBlackBoard();
						lContext.getPushList().clear();
					}
				}
			}
		} catch (SCHNAPS::Core::AssertException e) {
			e.explain(std::cout);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::processScenario(SCHNAPS::Simulation::Thread*)");
}

void Simulator::processClockStep(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
		try {
			SimulationContext& lContext = inThread->getContext();
			const std::vector<unsigned int>& lIndexes = inThread->getIndexes();

			std::vector<std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();

			for (unsigned int i = 0; i < lIndexes.size(); i++) {
				lContext.setIndividual(lContext.getEnvironment().getPopulation()[lIndexes[i]]);
				// Processing clock observers
				for (unsigned int j = 0; j < lContext.getClockObservers().mProcessIndividual.size(); j++) {
					lContext.getClockObservers().mProcessIndividual[j]->execute(lContext);
				}

				// Processing current individual FIFO
				while (lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].empty() == false) {
					lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].front()->execute(lContext);
					lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].pop();
				}
				lIndividualWaitingQMaps[lIndexes[i]].erase(lContext.getClock().getValue());

				// Keeping track of processes pushed by individual at index i
				if (lContext.getPushList().empty() == false) {
					inThread->waitBlackBoard();
					inThread->getBlackBoard()[lIndexes[i]] = lContext.getPushList();
					inThread->postBlackBoard();
					lContext.getPushList().clear();
				}
			}
		} catch (SCHNAPS::Core::AssertException e) {
			e.explain(std::cout);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::processClockStep(SCHNAPS::Simulation::Thread*)");
}

void Simulator::processSubStep(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
		try {
			SimulationContext& lContext = inThread->getContext();
			const std::vector<unsigned int>& lIndexes = inThread->getIndexes();

			std::vector<std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();

			for (unsigned int i = 0; i < lIndexes.size(); i++) {
				lContext.setIndividual(lContext.getEnvironment().getPopulation()[lIndexes[i]]);
				// Processing current individual FIFO
				while (lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].empty() == false) {
					lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].front()->execute(lContext);
					lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].pop();
				}
				lIndividualWaitingQMaps[lIndexes[i]].erase(lContext.getClock().getValue());
				// Keeping track of processes pushed by individual at index i
				if (lContext.getPushList().empty() == false) {
					inThread->waitBlackBoard();
					inThread->getBlackBoard()[lIndexes[i]] = lContext.getPushList();
					inThread->postBlackBoard();
					lContext.getPushList().clear();
				}
			}
		} catch (SCHNAPS::Core::AssertException e) {
			e.explain(std::cout);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::processSubStep(SCHNAPS::Simulation::Thread*)");
}

// Private functions
void Simulator::init() {
	schnaps_StackTraceBeginM();
		// Create one context per thread + sub threads
		unsigned int lNbThreads_new = SCHNAPS::Core::castHandleT<SCHNAPS::Core::UInt>(mSystem->getParameters()["threads.simulator"])->getValue();
		unsigned int lNbThreads_old = mContext.size();

		if (lNbThreads_old < lNbThreads_new) {
			for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
				if (i == 0) {
					mContext.push_back(new SimulationContext(mSystem, mClock, mEnvironment));
				} else {
					mContext.push_back(mContext[i-1]->deepCopy());
				}
				mContext.back()->setThreadNb(i);

				mSubThreads.push_back(new SimulationThread(mParallel, mSequential, mBlackBoardWrt, mContext.back(), mBlackBoard, mWaitingQMaps));
				mSequential->wait();
			}
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readInput(PACC::XML::ConstIterator)");
}

void Simulator::readInput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading input\n");
#endif
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Input") {
			std::ostringstream lOSS;
			lOSS << "tag <Input> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		PACC::XML::ConstIterator lChild = inIter->getFirstChild();

		// Read environment
		mEnvironment->readWithSystem(lChild++, *mSystem);

		// Generate initial population
		mPopulationManager->readWithSystem(lChild, *mSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readInput(PACC::XML::ConstIterator)");
}

void Simulator::readSimulation(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading simulation\n");
#endif
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Simulation") {
			std::ostringstream lOSS;
			lOSS << "tag <Simulation> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		PACC::XML::ConstIterator lChild = inIter->getFirstChild();

		// Read randomizer info to use in simulation
		readRandomizerInfo(lChild++);

		// Read clock
		mClock->readWithSystem(lChild++, *mSystem);

		PACC::XML::ConstIterator lChildSubThread;

		for (unsigned int i = 0; i < mContext.size(); i++) {
			lChildSubThread = lChild;

			// Read processes
			mContext[i]->readProcesses(lChildSubThread++);

			// Read scenarios
			mContext[i]->readScenarios(lChildSubThread++);

			// Read observers
			mContext[i]->readObservers(lChildSubThread);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readSimulation(PACC::XML::ConstIterator)");
}

void Simulator::readOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading output\n");
#endif
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Output") {
			std::ostringstream lOSS;
			lOSS << "tag <Output> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		PACC::XML::ConstIterator lChild = inIter->getFirstChild();
		readEnvironmentOutput(lChild++);
		readPopulationOutput(lChild);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readOutput(PACC::XML::ConstIterator)");
}

void Simulator::readRandomizerInfo(PACC::XML::ConstIterator inIter) {
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

		unsigned int lThreadsSimulator = SCHNAPS::Core::castHandleT<SCHNAPS::Core::UInt>(mSystem->getParameters()["threads.simulator"])->getValue();
		mRandomizerInitSeed.clear();
		mRandomizerInitState.clear();
		mRandomizerInitSeed.resize(lThreadsSimulator, 0);
		mRandomizerInitState.resize(lThreadsSimulator, "");

		if ((inIter->getChildCount() != 0) && (inIter->getChildCount() != lThreadsSimulator)) {
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readRandomizerInfo(PACC::XML::ConstIterator)");
}

void Simulator::readEnvironmentOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Environment") {
			std::ostringstream lOSS;
			lOSS << "tag <Environment> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		mOutputParameters.mEnvironment.clear();
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "Variable") {
					std::ostringstream lOSS;
					lOSS << "tag <Variable> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				if (lChild->getAttribute("label").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "variable label for environment output expected!");
				}

				mOutputParameters.mEnvironment.push_back(lChild->getAttribute("label"));
			}
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readEnvironmentOutput(PACC::XML::ConstIterator)");
}

void Simulator::readPopulationOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Population") {
			std::ostringstream lOSS;
			lOSS << "tag <Population> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		mOutputParameters.mPopulation.clear();
		for (PACC::XML::ConstIterator lChild_i = inIter->getFirstChild(); lChild_i; lChild_i++) {
			if (lChild_i->getType() == PACC::XML::eData) {
				if (lChild_i->getValue() != "SubPopulation") {
					std::ostringstream lOSS;
					lOSS << "tag <SubPopulation> expected, but ";
					lOSS << "got tag <" << lChild_i->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild_i, lOSS.str());
				}
				if (lChild_i->getAttribute("profile").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild_i, "sub-population profile for output population expected!");
				}

				for (PACC::XML::ConstIterator lChild_j = lChild_i->getFirstChild(); lChild_j; lChild_j++) {
					if (lChild_j->getType() == PACC::XML::eData) {
						if (lChild_j->getValue() != "Variable") {
							std::ostringstream lOSS;
							lOSS << "tag <Variable> expected, but ";
							lOSS << "got tag <" << lChild_j->getValue() << "> instead!";
							throw schnaps_IOExceptionNodeM(*lChild_j, lOSS.str());
						}
						if (lChild_j->getAttribute("label").empty()) {
							throw schnaps_IOExceptionNodeM(*lChild_j, "variable label for sub-population output expected!");
						}

						mOutputParameters.mPopulation[lChild_i->getAttribute("profile")].push_back(lChild_j->getAttribute("label"));
					}
				}
			}
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::readPopulationOutput(PACC::XML::ConstIterator)");
}

void Simulator::writeInput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Input");
		mEnvironment->write(ioStreamer, inIndent);
		mPopulationManager->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writeInput(PACC::XML::Streamer&, bool)");
}

void Simulator::writeSimulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Simulation");
		writeRandomizerInfo(ioStreamer, inIndent);
		mClock->write(ioStreamer, inIndent);
		mContext[0]->writeProcesses(ioStreamer, inIndent);
		mContext[0]->writeScenarios(ioStreamer, inIndent);
		mContext[0]->writeObservers(ioStreamer, inIndent);
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writeSimulationPACC::XML::Streamer&, bool)");
}

void Simulator::writeOutput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Output");
		writeEnvironmentOutput(ioStreamer, inIndent);
		writePopulationOutput(ioStreamer, inIndent);
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writeOutput(PACC::XML::Streamer&, bool)");
}

void Simulator::writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("RandomizerInfo");
		for (unsigned int i = 0; i < mRandomizerInitState.size(); i++) {
			ioStreamer.openTag("Randomizer");
			ioStreamer.insertAttribute("seed", mRandomizerInitSeed[i]);
			ioStreamer.insertAttribute("state", mRandomizerInitState[i]);
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writeRandomizerInfo(PACC::XML::Streamer&, bool)");
}

void Simulator::writeEnvironmentOutput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Environment");
		for (std::vector<std::string>::const_iterator lIt = mOutputParameters.mEnvironment.begin(); lIt != mOutputParameters.mEnvironment.end(); lIt++) {
			ioStreamer.openTag("Variable");
			ioStreamer.insertAttribute("label", *lIt);
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writeEvironmentOutput(PACC::XML::Streamer&, bool)");
}

void Simulator::writePopulationOutput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag("Population");
		for (std::map<std::string, std::vector<std::string> >::const_iterator lIt_i = mOutputParameters.mPopulation.begin(); lIt_i != mOutputParameters.mPopulation.end(); lIt_i++) {
			ioStreamer.openTag("SubPopulation");
			ioStreamer.insertAttribute("profile", lIt_i->first);
			for (std::vector<std::string>::const_iterator lIt_j = lIt_i->second.begin(); lIt_j != lIt_i->second.end(); lIt_j++) {
				ioStreamer.openTag("Variable");
				ioStreamer.insertAttribute("label", *lIt_j);
				ioStreamer.closeTag();
			}
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::writePopulationOutput(PACC::XML::Streamer&, bool)");
}

void Simulator::printInput(std::ostream& ioStream, unsigned int inLowerBound) const {
	schnaps_StackTraceBeginM();
		Individual::Handle lIndividual;
		std::string lPrefix = "NONE";
		std::map<std::string, std::vector<std::string> >::const_iterator lSubPopulationIt;

		for (unsigned int i = inLowerBound; i < mEnvironment->getPopulation().size(); i++) {
			lIndividual = mEnvironment->getPopulation()[i];
			if (lIndividual->getID().find(lPrefix) == std::string::npos) {
				lPrefix = lIndividual->getPrefix();
				lSubPopulationIt = mOutputParameters.mPopulation.find(mPopulationManager->getPrefixes().find(lPrefix)->second.mProfile);
			}
			lIndividual->print(ioStream, lSubPopulationIt->second);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::printInput(std::ostream&, unsigned int) const");
}

void Simulator::printOutput(std::ostream& ioStream) const {
	schnaps_StackTraceBeginM();
		// Print environment
		mEnvironment->print(ioStream, mOutputParameters.mEnvironment);

		// Print individuals
		Individual::Handle lIndividual;
		std::string lPrefix = "NONE";
		std::map<std::string, std::vector<std::string> >::const_iterator lSubPopulationIt;

		for (unsigned int i = 0; i < mEnvironment->getPopulation().size(); i++) {
			lIndividual = mEnvironment->getPopulation()[i];
			if (lIndividual->getID().find(lPrefix) == std::string::npos) {
				lPrefix = lIndividual->getPrefix();
				lSubPopulationIt = mOutputParameters.mPopulation.find(mPopulationManager->getPrefixes().find(lPrefix)->second.mProfile);
			}
			lIndividual->print(ioStream, lSubPopulationIt->second);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::printOutput(std::ostream&) const");
}

void Simulator::printSummary(std::ostream& ioStream) const {
	schnaps_StackTraceBeginM();
		PACC::XML::Streamer ioStreamer(ioStream);

		ioStreamer.openTag("Summary");

		// Print environment
		ioStreamer.openTag("Environment");
		ioStreamer.insertAttribute("prefix", "E");
		for (unsigned int i = 0; i < mOutputParameters.mEnvironment.size(); i++) {
			ioStreamer.openTag("Column");
			ioStreamer.insertAttribute("label", mOutputParameters.mEnvironment[i]);
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();

		// Print individuals
		std::map<std::string, std::vector<std::string> >::const_iterator lProfileIt;
		ioStreamer.openTag("Population");
		for (std::map<std::string, Source>::const_iterator lIt = mPopulationManager->getPrefixes().begin(); lIt != mPopulationManager->getPrefixes().end(); lIt++) {
			ioStreamer.openTag("SubPopulation");
			ioStreamer.insertAttribute("prefix", lIt->first);
			ioStreamer.insertAttribute("profile", lIt->second.mProfile);
			ioStreamer.insertAttribute("size", lIt->second.mSize);
			lProfileIt = mOutputParameters.mPopulation.find(lIt->second.mProfile);
			for (unsigned int i = 0; i < lProfileIt->second.size(); i++) {
				ioStreamer.openTag("Column");
				ioStreamer.insertAttribute("label", lProfileIt->second[i]);
				ioStreamer.closeTag();
			}
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();

		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::printSummary(std::ostream&) const");
}
