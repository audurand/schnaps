/*
 * Simulator.cpp
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

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Default constructor.
 */
Simulator::Simulator() :
	mSystem(new Core::System()),
	mClock(new Clock()),
	mEnvironment(new Environment()),
	mPopulationManager(new PopulationManager(mSystem, mClock, mEnvironment)),
	mBlackBoard(new BlackBoard()),
	mWaitingQMaps(new WaitingQMaps()),
	mParallel(new PACC::Threading::Condition()),
	mSequential(new PACC::Threading::Semaphore(0)),
	mBlackBoardWrt(new PACC::Threading::Semaphore(1))
{
	// initialize factory with simulation elements
	mSystem->getFactory().insertAllocator("State", new State::Alloc());
	mSystem->getFactory().insertAllocator("Individual", new Individual::Alloc());
	mSystem->getFactory().insertAllocator("Environment", new Environment::Alloc());
	mSystem->getFactory().insertAllocator("Population", new Population::Alloc());
	mSystem->getFactory().insertAllocator("Generator", new Generator::Alloc());
	mSystem->getFactory().insertAllocator("Process", new Process::Alloc());
	mSystem->getFactory().insertAllocator("Clock", new Clock::Alloc());

	// initialize required parameters
	mSystem->getParameters().insertParameter("print.prefix", new Core::String(""));
	mSystem->getParameters().insertParameter("print.input", new Core::Bool(false));
	mSystem->getParameters().insertParameter("print.output", new Core::Bool(true));
	mSystem->getParameters().insertParameter("print.log", new Core::Bool(true));
	mSystem->getParameters().insertParameter("print.conf", new Core::Bool(false));
	mSystem->getParameters().insertParameter("threads.simulator", new Core::UInt(1));
	mSystem->getParameters().insertParameter("threads.generator", new Core::UInt(1));
	
	// create default context
	mContext.push_back(new SimulationContext(mSystem, mClock, mEnvironment));
	mContext.back()->setThreadNb(0);
}

/*!
 * \brief Destructor.
 */
Simulator::~Simulator() {
	// ensure to terminate all subthreads
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSubThreads[i]->setPosition(SimulationThread::eEND);
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

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Simulator::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	mSystem->write(ioStreamer, inIndent);
	writeInput(ioStreamer, inIndent);
	writeSimulation(ioStreamer, inIndent);
	writeOutput(ioStreamer, inIndent);
}

/*!
 * \brief Configure the simulator using specific parameters.
 * \param inParameters A const reference to parameters.
 */
void Simulator::configure(const std::string& inParameters) {
	schnaps_StackTraceBeginM();
	mSystem->getParameters().readStr(inParameters);

	// refresh simulator with up-to-date parameters
	refresh();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::configure(const std::string&)");
}

/*!
 * \brief 	Return the current simulator configuration.
 * \return The current simulator configuration as a string.
 */
std::string Simulator::getConfiguration() {
	schnaps_StackTraceBeginM();
	return mSystem->getParameters().writeStr();
	schnaps_StackTraceEndM("std::string SCHNAPS::Simulation::Simulator::getConfiguration()");
}

/*!
 * \brief Execute the simulation of specific scenario.
 * \param inScenarioLabel A const reference to the label of scenario to simulate.
 */
void Simulator::simulate(const std::string& inScenarioLabel) {
	schnaps_StackTraceBeginM();
	// reset simulation structures
	mClock->reset();
	mEnvironment->reset();
	mPopulationManager->getPrefixes().clear();
	mBlackBoard->clear();
	mWaitingQMaps->getEnvironmentWaitingQMap().clear();
	mWaitingQMaps->getIndividualsWaitingQMaps().clear();

	// backup randomizer seeds
	Core::ULongArray lBackupSeed;
	Core::StringArray lBackupState;
	
	for (unsigned int i = 0; i < mContext.size(); i++) {
		mContext[i]->reset();
		
		// create subthreads
		mSubThreads.push_back(new SimulationThread(mParallel, mSequential, mBlackBoardWrt, mContext[i], mBlackBoard, mWaitingQMaps));
		mSequential->wait();
		mSubThreads.back()->resetIndexes();
		mSubThreads.back()->setScenarioLabel(inScenarioLabel);

		// reset randomizer info
		lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
		lBackupState.push_back(mSystem->getRandomizer(i).getState());
		mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
	}

	// current position in simulation
	enum Position {eSTEP, eSUBSTEP};
	Position lPositionEnv;
	bool lSubStep;

	// population index bounds
	unsigned int lNewIndividuals_LowerBound = 0;

	// print info
	std::string lPrintPrefix = Core::castObjectT<const Core::String&>(mSystem->getParameters().getParameter("print.prefix")).getValue();
	bool lPrintInput = Core::castObjectT<const Core::Bool&>(mSystem->getParameters().getParameter("print.input")).getValue();
	bool lPrintOutput = Core::castObjectT<const Core::Bool&>(mSystem->getParameters().getParameter("print.output")).getValue();
	bool lPrintLog = Core::castObjectT<const Core::Bool&>(mSystem->getParameters().getParameter("print.log")).getValue();
	ogzstream lOGZS;
	std::stringstream lSS;

	if (lPrintInput) {
		lSS.str("");
		lSS << lPrintPrefix << "Input.gz";
		lOGZS.open(lSS.str().c_str(), std::ios::out);
		printEnvironment(lOGZS);
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
		std::cout << "Time " << mClock->getValue() << "\n";
#endif

		lPositionEnv = eSTEP;
		for (unsigned int i = 0; i < mSubThreads.size(); i++) {
			mSubThreads[i]->setPosition(SimulationThread::eSTEP);
		}

		// add new individuals
		if (mEnvironment->getPopulation().addIndividuals(mPopulationManager->getIndividuals()) == true) {
			// resize waiting map FIFOs to include new individuals
			for (unsigned int i = lNewIndividuals_LowerBound; i < mEnvironment->getPopulation().size(); i++) {
				mWaitingQMaps->getIndividualsWaitingQMaps()[i];
			}

			// assign individuals to threads
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSubThreads[i]->addNewIndexes(lNewIndividuals_LowerBound, mEnvironment->getPopulation().size());
			}

			// print input if asked
			if (lPrintInput == true) {
				printIndividuals(lOGZS, lNewIndividuals_LowerBound, mEnvironment->getPopulation().size()-1);
			}

			// reset new individuals bounds
			lNewIndividuals_LowerBound = mEnvironment->getPopulation().size();
		}

		do {
			lSubStep = false;

#ifdef SCHNAPS_FULL_DEBUG
			std::cout << "Processing environment\n";
#endif
			mContext[0]->setIndividual(mEnvironment);
			switch (lPositionEnv) {
			case eSTEP:
				if (mClock->getValue() == 0) {
					// process environment scenario
					if (mContext[0]->getScenario(inScenarioLabel).mProcessEnvironment != NULL) {
						mContext[0]->getScenario(inScenarioLabel).mProcessEnvironment->execute(*mContext[0]);
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
			
			// if the environment has been set idle, end the simulation
			if (mEnvironment->isActive() == false) {
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
						for (std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > >::iterator lIt_i = mWaitingQMaps->getIndividualsWaitingQMaps().begin(); lIt_i != mWaitingQMaps->getIndividualsWaitingQMaps().end(); lIt_i++) {
							(lIt_i->second)[mContext[0]->getPushList().front().mTime].push(new ProcessPushed(mContext[0]->getPushList().front().mProcess));
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
			std::cout << "Processing individuals\n";
#endif
			// process individuals
			mParallel->lock();
			mParallel->broadcast();
			mParallel->unlock();
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				mSequential->wait();
			}
			
			// erase idle individual waiting queues
			for (unsigned int i = 0; i < mSubThreads.size(); i++) {
				for (std::list<unsigned int>::iterator lIt_i = mSubThreads[i]->getEraseIndexes().begin(); lIt_i != mSubThreads[i]->getEraseIndexes().end(); lIt_i++) {
					mWaitingQMaps->getIndividualsWaitingQMaps().erase(*lIt_i);
				}
			}

			// push processes pushed by individuals
			if (mBlackBoard->empty() == false) {
				lSubStep = true;
			}
			for (BlackBoard::iterator lIt_i = mBlackBoard->begin(); lIt_i != mBlackBoard->end(); lIt_i++) {
				while (lIt_i->second.empty() == false) {
					switch (lIt_i->second.front().mTarget) {
					case Process::eCurrent:
						mWaitingQMaps->getIndividualsWaitingQMaps()[lIt_i->first][lIt_i->second.front().mTime].push(
								new ProcessPushed(lIt_i->second.front().mProcess));
						break;
					case Process::eEnvironment:
						mWaitingQMaps->getEnvironmentWaitingQMap()[lIt_i->second.front().mTime].push(
								new ProcessPushed(lIt_i->second.front().mProcess));
						break;
					case Process::eIndividuals:
						for (std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > >::iterator lIt_j = mWaitingQMaps->getIndividualsWaitingQMaps().begin(); lIt_j != mWaitingQMaps->getIndividualsWaitingQMaps().end(); lIt_j++) {
							(lIt_j->second)[mContext[0]->getPushList().front().mTime].push(new ProcessPushed(mContext[0]->getPushList().front().mProcess));
						}
						break;
					default:
						throw schnaps_InternalExceptionM("Undefined process target!");
						break;
					}
					lIt_i->second.pop_front();
				} // while (lIt->second.empty() == false
			} // for each BlackBoard::PushTracker
			mBlackBoard->clear();
		} while (lSubStep == true);
	} while (mClock->step(*mContext[0]) && mEnvironment->isActive());
	
	// terminate threads
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSubThreads[i]->setPosition(SimulationThread::eEND);
	}
	mParallel->lock();
	mParallel->broadcast();
	mParallel->unlock();

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
		printEnvironment(lOGZS);
		printIndividuals(lOGZS, 0, mEnvironment->getPopulation().size()-1);
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
	
	// destroy threads
	mSubThreads.clear();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::simulate(std::string, std::string, std::string)");
}

/*!
 * \brief Refresh simulator structure with up-to-date parameters.
 */
void Simulator::refresh() {
	schnaps_StackTraceBeginM();
	unsigned int lNbThreads_new = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.simulator")).getValue();
	if (lNbThreads_new < 1) {
		std::ostringstream lOSS;
		lOSS << "The number of simulation threads must be higher or equal to 1 (current new value = " << lNbThreads_new << ");";
		lOSS << "the number of simulation threads could not be set.\n";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	
	unsigned int lNbThreads_old = mContext.size();

	// create one context per thread
	if (lNbThreads_new > lNbThreads_old) {
		for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
			// create new context as copy of the first one to copy process map
			// TODO: does the simulator should own an original copy of the process map? 
			mContext.push_back(mContext[0]->deepCopy());
			mContext.back()->setThreadNb(i);
			
			// add simulator randomizer information
			mRandomizerInitSeed.push_back(0);
			mRandomizerInitState.push_back("");
		}
	} else if (lNbThreads_new < lNbThreads_old) {
		for (unsigned int i = lNbThreads_old; i > lNbThreads_new; i--) {
			// remove unused context
			mContext.pop_back();
			
			// remove unused randomizer information
			mRandomizerInitSeed.pop_back();
			mRandomizerInitState.pop_back();
		}
	}
	
	// update simulator randomizers information
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	
	// refresh individual generator
	mPopulationManager->getGenerator().refresh();
	
	// init system components
	mSystem->initComponents();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::refresh()");
}

/*!
 * \brief Clear the randomizer values for seeds and states.
 */
void Simulator::clearRandomizer() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
		mRandomizerCurrentSeed[i] = 0;
		mRandomizerCurrentState[i] = "";
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::clearRandomizer()");
}

/*!
 * \brief Reset the randomizer values for seeds and states to initial values.
 */
void Simulator::resetRandomizer() {
	schnaps_StackTraceBeginM();
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::resetRandomizer()");
}

/*!
 * \brief Scenario processing by a specific thread.
 * \param inThread A handle to the executing thread.
 */
void Simulator::processScenario(SimulationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
	SimulationContext& lContext = inThread->getContext();
	
	Individual::Handle lIndividual;
	
	std::list<unsigned int>& lNewIndexes = inThread->getNewIndexes();
	std::list<unsigned int>& lEraseIndexes = inThread->getEraseIndexes();
	
	lEraseIndexes.clear();

	// if there is a scenario for individuals
	if (lContext.getScenario(inThread->getScenarioLabel()).mProcessIndividual != NULL) {
		// execute the scenario
		std::list<unsigned int>::iterator lIt_i = lNewIndexes.begin();
		while (lIt_i != lNewIndexes.end()) {
			lIndividual = lContext.getEnvironment().getPopulation()[*lIt_i];
			
			lContext.setIndividual(lIndividual);
			
			// process scenario
			lContext.getScenario(inThread->getScenarioLabel()).mProcessIndividual->execute(lContext);
			
			// keep track of processes pushed by individual at index *lIt_i
			if (lContext.getPushList().empty() == false) {
				inThread->waitBlackBoard();
				inThread->getBlackBoard()[*lIt_i] = lContext.getPushList();
				inThread->postBlackBoard();
				lContext.getPushList().clear();
			}
		
			// if individual is still active, go to next individual
			if (lIndividual->isActive()) {
				lIt_i++;
			// if individual has been set idle, remove its index from thread charge
			} else {
				lEraseIndexes.push_back(*lIt_i);
				lIt_i = lNewIndexes.erase(lIt_i);
			}
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processScenario(SCHNAPS::Simulation::SimulationThread::Handle)");
}

/*!
 * \brief Clock step processing by a specific thread.
 * \param inThread A handle to the executing thread.
 */
void Simulator::processClockStep(SimulationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
	SimulationContext& lContext = inThread->getContext();
	
	Individual::Handle lIndividual;
	
	std::list<unsigned int>& lIndexes = inThread->getIndexes();
	std::list<unsigned int>& lEraseIndexes = inThread->getEraseIndexes();
	std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();
	
	lEraseIndexes.clear();

	std::list<unsigned int>::iterator lIt_i = lIndexes.begin();
	while (lIt_i != lIndexes.end()) {
		lIndividual = lContext.getEnvironment().getPopulation()[*lIt_i];
		
		lContext.setIndividual(lIndividual);
		
		// process clock observers
		for (unsigned int j = 0; j < lContext.getClockObservers().mProcessIndividual.size(); j++) {
			lContext.getClockObservers().mProcessIndividual[j]->execute(lContext);
		}

		// process current individual FIFO until it is empty or individual is set idle
		while ((lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].empty() == false) && (lIndividual->isActive())) {
			lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].front()->execute(lContext);
			lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].pop();
		}
		lIndividualWaitingQMaps[*lIt_i].erase(lContext.getClock().getValue());

		// keep track of processes pushed by individual at index *lIt_i
		if (lContext.getPushList().empty() == false) {
			inThread->waitBlackBoard();
			inThread->getBlackBoard()[*lIt_i] = lContext.getPushList();
			inThread->postBlackBoard();
			lContext.getPushList().clear();
		}
		
		// if individual is still active, go to next individual
		if (lIndividual->isActive()) {
			lIt_i++;
		// if individual has been set idle, remove its index from thread charge
		} else {
			lEraseIndexes.push_back(*lIt_i);
			lIt_i = lIndexes.erase(lIt_i);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processClockStep(SCHNAPS::Simulation::SimulationThread::Handle)");
}

/*!
 * \brief Sub clock step processing by a specific thread.
 * \param inThread A handle to the executing thread.
 */
void Simulator::processSubStep(SimulationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
	SimulationContext& lContext = inThread->getContext();
	
	Individual::Handle lIndividual;
	
	std::list<unsigned int>& lIndexes = inThread->getIndexes();
	std::list<unsigned int>& lEraseIndexes = inThread->getEraseIndexes();
	std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > >& lIndividualWaitingQMaps = inThread->getWaitingQMaps().getIndividualsWaitingQMaps();
	
	lEraseIndexes.clear();

	std::list<unsigned int>::iterator lIt_i = lIndexes.begin();
	while (lIt_i != lIndexes.end()) {
		lIndividual = lContext.getEnvironment().getPopulation()[*lIt_i];
		
		lContext.setIndividual(lIndividual);
		
		// process current individual FIFO
		while ((lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].empty() == false) && (lIndividual->isActive())) {
			lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].front()->execute(lContext);
			lIndividualWaitingQMaps[*lIt_i][lContext.getClock().getValue()].pop();
		}
		lIndividualWaitingQMaps[*lIt_i].erase(lContext.getClock().getValue());
		
		// keep track of processes pushed by individual at index *lIt_i
		if (lContext.getPushList().empty() == false) {
			inThread->waitBlackBoard();
			inThread->getBlackBoard()[*lIt_i] = lContext.getPushList();
			inThread->postBlackBoard();
			lContext.getPushList().clear();
		}
		
		// if individual is still active, go to next individual
		if (lIndividual->isActive()) {
			lIt_i++;
		// if individual has been set idle, remove its index from thread charge
		} else {
			lEraseIndexes.push_back(*lIt_i);
			lIt_i = lIndexes.erase(lIt_i);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::processSubStep(SCHNAPS::Simulation::SimulationThread::Handle)");
}

// private functions

/*!
 * \brief Read input section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Simulator::readInput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "Reading input\n";
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

/*!
 * \brief Read simulation section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Simulator::readSimulation(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Simulation") {
		std::ostringstream lOSS;
		lOSS << "tag <Simulation> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "Reading simulation\n";
#endif

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

/*!
 * \brief Read output section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Simulator::readOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Output") {
		std::ostringstream lOSS;
		lOSS << "tag <Output> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "Reading output\n";
#endif

	PACC::XML::ConstIterator lChild = inIter->getFirstChild();
	readEnvironmentOutput(lChild++);
	readPopulationOutput(lChild);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::readOutput(PACC::XML::ConstIterator)");
}

/*!
 * \brief Read randomizer information in simulation section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
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

	unsigned int lThreadsSimulator = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.simulator")).getValue();
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

/*!
 * \brief Read environment information in output section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Simulator::readEnvironmentOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Environment") {
		std::ostringstream lOSS;
		lOSS << "tag <Environment> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "- environment\n";
#endif

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

/*!
 * \brief Read population information in output section of configuration file.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 * \throw SCHNAPS::Core::IOException if sub-population profile is missing.
 * \throw SCHNAPS::Core::IOException if variable label is missing.
 */
void Simulator::readPopulationOutput(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Population") {
		std::ostringstream lOSS;
		lOSS << "tag <Population> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "- population\n";
#endif

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

/*!
 * \brief Write input section to configuration file.
 */
void Simulator::writeInput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Input");
	mEnvironment->write(ioStreamer, inIndent);
	mPopulationManager->write(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeInput(PACC::XML::Streamer&, bool)");
}

/*!
 * \brief Write simulation section to configuration file.
 */
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

/*!
 * \brief Write output section to configuration file.
 */
void Simulator::writeOutput(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Output");
	writeEnvironmentOutput(ioStreamer, inIndent);
	writePopulationOutput(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::writeOutput(PACC::XML::Streamer&, bool)");
}

/*!
 * \brief Write randomizer information in simulation section to configuration file.
 */
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

/*!
 * \brief Write environment information in output section to configuration file.
 */
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

/*!
 * \brief Write population information in output section to configuration file.
 */
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

/*!
 * \brief Print the current state of the environment.
 */
void Simulator::printEnvironment(std::ostream& ioStream) const {
	schnaps_StackTraceBeginM();
	mEnvironment->print(ioStream, mOutputParameters.mEnvironment);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::printEnvironment(std::ostream&) const");
}

/*!
 * \brief Print the current state of individuals in population, between specific indexes.
 * \throw SCHNAPS::Core::AssertException if specified upper index is higher than population size.
 */
void Simulator::printIndividuals(std::ostream& ioStream, unsigned int inLowerIndex, unsigned int inUpperIndex) const {
	schnaps_StackTraceBeginM();
	Individual::Handle lIndividual;
	std::string lPrefix = "NONE";
	std::map<std::string, std::vector<std::string> >::const_iterator lSubPopulationIt;
	
	schnaps_UpperBoundCheckAssertM(inUpperIndex, mEnvironment->getPopulation().size()-1);

	for (unsigned int i = inLowerIndex; i < inUpperIndex+1; i++) {
		lIndividual = mEnvironment->getPopulation()[i];
		if (lIndividual->getID().find(lPrefix) == std::string::npos) {
			lPrefix = lIndividual->getPrefix();
			lSubPopulationIt = mOutputParameters.mPopulation.find(mPopulationManager->getPrefixes().find(lPrefix)->second.mProfile);
		}
		lIndividual->print(ioStream, lSubPopulationIt->second);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Simulator::printIndividuals(std::ostream&, unsigned int, unsigned int) const");
}

/*!
 * \brief Print the summary that describes the output variables and order.
 */
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
