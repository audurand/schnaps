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
	// initialize factory with elements
	mSystem->getFactory().insertAllocator("State", new SCHNAPS::Simulation::State::Alloc());
	mSystem->getFactory().insertAllocator("Individual", new SCHNAPS::Simulation::Individual::Alloc());
	mSystem->getFactory().insertAllocator("Environment", new SCHNAPS::Simulation::Environment::Alloc());
	mSystem->getFactory().insertAllocator("Population", new SCHNAPS::Simulation::Population::Alloc());
	mSystem->getFactory().insertAllocator("Generator", new SCHNAPS::Simulation::Generator::Alloc());
	mSystem->getFactory().insertAllocator("Process", new SCHNAPS::Simulation::Process::Alloc());
	mSystem->getFactory().insertAllocator("Clock", new SCHNAPS::Simulation::Clock::Alloc());

	// initialize required parameters
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

	// system must be read first
	mSystem->read(inIter++);

	// initialize simulator with read parameters
	refresh();

	// read simulation input configuration
	readInput(inIter++);

	// read simulation configuration
	readSimulation(inIter++);

	// read simulation output configuration
	readOutput(inIter);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::read(PACC::XML::ConstIterator)");
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

	// refresh simulator with up-to-date parameters
	refresh();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::configure(const std::string&)");
}

std::string Simulator::getConfiguration() {
	schnaps_StackTraceBeginM();
	return mSystem->getParameters().writeStr();
	schnaps_StackTraceEndM("std::string SCHNAPS::Simulation::Simulator::getConfiguration()");
}

void Simulator::simulate(std::string inScenarioLabel) {
	schnaps_StackTraceBeginM();
	// reset simulation structures
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

	// current position in simulation
	enum Position {eSTEP, eSUBSTEP};
	Position lPositionEnv;
	bool lSubStep;

	// population index bounds
	unsigned int lNewIndividuals_LowerBound = 0;

	// backup randomizer seeds
	SCHNAPS::Core::ULongArray lBackupSeed;
	SCHNAPS::Core::StringArray lBackupState;

	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		// set subthread on specific scenario
		mSubThreads[i]->setScenarioLabel(inScenarioLabel);

		// reset randomizer info
		lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
		lBackupState.push_back(mSystem->getRandomizer(i).getState());
		mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
	}

	// print info
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

		// add new individuals
		if (mEnvironment->getPopulation().addIndividuals(mPopulationManager->getIndividuals()) == true) {
			// resize waiting map FIFOs to include new individuals
			mWaitingQMaps->getIndividualsWaitingQMaps().resize(mEnvironment->getPopulation().size());

			// assign individuals to threads
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSubThreads[i]->addNewIndexes(lNewIndividuals_LowerBound, mEnvironment->getPopulation().size());
			}

			// print input if asked
			if (lPrintInput == true) {
				printInput(lOGZS, lNewIndividuals_LowerBound);
			}

			// reset new individuals bounds
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
					// process environment scenario
					if (mContext[0]->getScenarios().find(inScenarioLabel)->second.mProcessEnvironment != NULL) {
						mContext[0]->getScenarios().find(inScenarioLabel)->second.mProcessEnvironment->execute(*mContext[0]);
					}
				} else {
					// process clock observers
					for (unsigned int j = 0; j < mContext[0]->getClockObservers().mProcessEnvironment.size(); j++) {
						mContext[0]->getClockObservers().mProcessEnvironment[j]->execute(*mContext[0]);
					}
					// process current environment FIFO
					while (mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].empty() == false) {
						mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].front()->execute(*mContext[0]);
						mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].pop();
					}
				}
				lPositionEnv = eSUBSTEP;
				break;
			case eSUBSTEP:
				// process current environment FIFO
				while (mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].empty() == false) {
					mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].front()->execute(*mContext[0]);
					mWaitingQMaps->getEnvironmentWaitingQMap()[mClock->getValue()].pop();
				}
				mWaitingQMaps->getEnvironmentWaitingQMap().erase(mClock->getValue());
				break;
			default:
				break;
			}

			// push processes pushed by environment
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
			// process individuals
			mParallel->lock();
			mParallel->broadcast();
			mParallel->unlock();
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSequential->wait();
			}

			// push processes pushed by individuals
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

	// reset randomizers old info
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mRandomizerCurrentSeed[i] = mSystem->getRandomizer(i).getSeed();
		mRandomizerCurrentState[i] = mSystem->getRandomizer(i).getState();
		mSystem->getRandomizer(i).reset(lBackupSeed[i], lBackupState[i]);
	}

	// close log files
	if (lPrintLog) {
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mSystem->getLogger(i).close();
		}
	}

	// close print input file if opened
	if (lPrintInput) {
		lOGZS.close();
	}

	// print results
	if (lPrintOutput) {
		lSS.str("");
		lSS << lPrintPrefix << "Output.gz";
		lOGZS.open(lSS.str().c_str(), std::ios::out);
		printOutput(lOGZS);
		lOGZS.close();
	}

	// print summary
	if (lPrintInput || lPrintOutput) {
		lSS.str("");
		lSS << lPrintPrefix << "Summary.gz";
		lOGZS.open(lSS.str().c_str(), std::ios::out);
		printSummary(lOGZS);
		lOGZS.close();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::simulate(std::string, std::string, std::string)");
}

/*!
 * \brief Refresh simulator structure with up-to-date parameters.
 */
void Simulator::refresh() {
	schnaps_StackTraceBeginM();
	unsigned int lNbThreads_new = Core::castHandleT<Core::UInt>(mSystem->getParameters()["threads.simulator"])->getValue();
	unsigned int lNbThreads_old = mContext.size();
	
	// if no threads already, create one + context
	if (lNbThreads_old == 0) {
		mContext.push_back(new SimulationContext(mSystem, mClock, mEnvironment));
		mContext.back()->setThreadNb(0);

		mSubThreads.push_back(new SimulationThread(mParallel, mSequential, mBlackBoardWrt, mContext.back(), mBlackBoard, mWaitingQMaps));
		mSequential->wait();
		
		lNbThreads_old = 1;
	}

	// create one context per thread + sub threads
	if (lNbThreads_new > lNbThreads_old) {
		for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
			// create new threads as copies of the first one to copy process map.
			// TODO: does the simulator should own an original copy of the process map? 
			mContext.push_back(mContext[0]->deepCopy());
			mContext.back()->setThreadNb(i);

			mSubThreads.push_back(new SimulationThread(mParallel, mSequential, mBlackBoardWrt, mContext.back(), mBlackBoard, mWaitingQMaps));
			mSequential->wait();
		}
	} else if (lNbThreads_new < lNbThreads_old) {
		// TODO: remove unused contexts and subthreads
	}
	
	// update simulator randomizers information
	mRandomizerInitSeed.resize(lNbThreads_new, 0);
	mRandomizerInitState.resize(lNbThreads_new, "");

	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	
	// refresh individual generator
	mPopulationManager->getGenerator().refresh();
	
	// init system components
	mSystem->initComponents();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::refresh()");
}

void Simulator::clearRandomizer() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
		mRandomizerCurrentSeed[i] = 0;
		mRandomizerCurrentState[i] = "";
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::clearRandomizer()");
}

void Simulator::resetRandomizer() {
	schnaps_StackTraceBeginM();
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::resetRandomizer()");
}

void Simulator::processScenario(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
	try {
		SimulationContext& lContext = inThread->getContext();
		const std::vector<unsigned int>& lNewIndexes = inThread->getNewIndexes();

		// if there is a scenario for individuals
		if (lContext.getScenarios().find(inThread->getScenarioLabel())->second.mProcessIndividual != NULL) {
			// execute the scenario
			for (unsigned int i = 0; i < lNewIndexes.size(); i++) {
				lContext.setIndividual(lContext.getEnvironment().getPopulation()[lNewIndexes[i]]);
				lContext.getScenario(inThread->getScenarioLabel()).mProcessIndividual->execute(lContext);
				// keep track of processes pushed by individual at index i
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processScenario(SCHNAPS::Simulation::Thread*)");
}

void Simulator::processClockStep(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
	try {
		SimulationContext& lContext = inThread->getContext();
		const std::vector<unsigned int>& lIndexes = inThread->getIndexes();

		std::vector<std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();

		for (unsigned int i = 0; i < lIndexes.size(); i++) {
			lContext.setIndividual(lContext.getEnvironment().getPopulation()[lIndexes[i]]);
			// process clock observers
			for (unsigned int j = 0; j < lContext.getClockObservers().mProcessIndividual.size(); j++) {
				lContext.getClockObservers().mProcessIndividual[j]->execute(lContext);
			}

			// process current individual FIFO
			while (lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].empty() == false) {
				lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].front()->execute(lContext);
				lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].pop();
			}
			lIndividualWaitingQMaps[lIndexes[i]].erase(lContext.getClock().getValue());

			// keep track of processes pushed by individual at index i
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processClockStep(SCHNAPS::Simulation::Thread*)");
}

void Simulator::processSubStep(SimulationThread* inThread) {
	schnaps_StackTraceBeginM();
	try {
		SimulationContext& lContext = inThread->getContext();
		const std::vector<unsigned int>& lIndexes = inThread->getIndexes();

		std::vector<std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();

		for (unsigned int i = 0; i < lIndexes.size(); i++) {
			lContext.setIndividual(lContext.getEnvironment().getPopulation()[lIndexes[i]]);
			// process current individual FIFO
			while (lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].empty() == false) {
				lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].front()->execute(lContext);
				lIndividualWaitingQMaps[lIndexes[i]][lContext.getClock().getValue()].pop();
			}
			lIndividualWaitingQMaps[lIndexes[i]].erase(lContext.getClock().getValue());
			// keep track of processes pushed by individual at index i
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processSubStep(SCHNAPS::Simulation::Thread*)");
}

// private functions

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

	// read environment
	mEnvironment->readWithSystem(lChild++, *mSystem);

	// generate initial population
	mPopulationManager->readWithSystem(lChild, *mSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readInput(PACC::XML::ConstIterator)");
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

	// read randomizer info to use in simulation
	readRandomizerInfo(lChild++);

	// read clock
	mClock->readWithSystem(lChild++, *mSystem);

	PACC::XML::ConstIterator lChildSubThread;

	for (unsigned int i = 0; i < mContext.size(); i++) {
		lChildSubThread = lChild;

		// read processes
		mContext[i]->readProcesses(lChildSubThread++);

		// read scenarios
		mContext[i]->readScenarios(lChildSubThread++);

		// read observers
		mContext[i]->readObservers(lChildSubThread);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readSimulation(PACC::XML::ConstIterator)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readOutput(PACC::XML::ConstIterator)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readRandomizerInfo(PACC::XML::ConstIterator)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readEnvironmentOutput(PACC::XML::ConstIterator)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readPopulationOutput(PACC::XML::ConstIterator)");
}

void Simulator::writeInput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Input");
	mEnvironment->write(ioStreamer, inIndent);
	mPopulationManager->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeInput(PACC::XML::Streamer&, bool)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeSimulationPACC::XML::Streamer&, bool)");
}

void Simulator::writeOutput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Output");
	writeEnvironmentOutput(ioStreamer, inIndent);
	writePopulationOutput(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeOutput(PACC::XML::Streamer&, bool)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeRandomizerInfo(PACC::XML::Streamer&, bool)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeEvironmentOutput(PACC::XML::Streamer&, bool)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writePopulationOutput(PACC::XML::Streamer&, bool)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::printInput(std::ostream&, unsigned int) const");
}

void Simulator::printOutput(std::ostream& ioStream) const {
	schnaps_StackTraceBeginM();
	// print environment
	mEnvironment->print(ioStream, mOutputParameters.mEnvironment);

	// print individuals
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::printOutput(std::ostream&) const");
}

void Simulator::printSummary(std::ostream& ioStream) const {
	schnaps_StackTraceBeginM();
	PACC::XML::Streamer ioStreamer(ioStream);

	ioStreamer.openTag("Summary");

	// print environment
	ioStreamer.openTag("Environment");
	ioStreamer.insertAttribute("prefix", "E");
	for (unsigned int i = 0; i < mOutputParameters.mEnvironment.size(); i++) {
		ioStreamer.openTag("Column");
		ioStreamer.insertAttribute("label", mOutputParameters.mEnvironment[i]);
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();

	// print individuals
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::printSummary(std::ostream&) const");
}
