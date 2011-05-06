/*
 * SimulationThread.hpp
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

#ifndef SCHNAPS_Simulation_SimulationThread_hpp
#define SCHNAPS_Simulation_SimulationThread_hpp

#include "SCHNAPS/Simulation/SimulationContext.hpp"
#include "SCHNAPS/Simulation/BlackBoard.hpp"
#include "SCHNAPS/Simulation/WaitingQMaps.hpp"

#include "PACC/PACC.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class SimulationThread SCHNAPS/Simulation/SimulationThread.hpp "SCHNAPS/Simulation/SimulationThread.hpp"
 *  \brief Thread for executing the simulation over individuals.
 */
class SimulationThread: public Core::Object, public PACC::Threading::Thread {
public:
	//! SimulationThread allocator type.
	typedef Core::AllocatorT<SimulationThread, Core::Object::Alloc> Alloc;
	//! SimulationThread handle type.
	typedef Core::PointerT<SimulationThread, Core::Object::Handle> Handle;
	//! SimulationThread bag type.
	typedef Core::ContainerT<SimulationThread, Core::Object::Bag> Bag;

	//! The position of threads in the simulation process.
	enum Position {eSTEP, eSUBSTEP, eEND};

	explicit SimulationThread(PACC::Threading::Condition* inParallel,
		PACC::Threading::Semaphore* inSequential,
		PACC::Threading::Semaphore* inBlackBoardWrt,
		SimulationContext::Handle inContext,
		BlackBoard::Handle inBlackBoard,
		WaitingQMaps::Handle inWaitingQMaps);
	~SimulationThread();

	/*!
	 * \brief Wait for the blackboard to be available.
	 */
	void waitBlackBoard() {
		schnaps_StackTraceBeginM();
		mBlackBoardWrt->wait();
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::waitBlackBoard()");
	}

	/*!
	 * \brief Make the blackboard available to other threads.
	 */
	void postBlackBoard() {
		schnaps_StackTraceBeginM();
		mBlackBoardWrt->post();
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::postBlackBoard()");
	}

	/*!
	 * \brief Add new indexes of individuals to simulate between specific bounds, according to the current thread ID and the total number of threads.
	 * \param inLowerBound The lower bound of indexes.
	 * \param inUpperBound The upper bound of indexes.
	 */
	void addNewIndexes(unsigned int inLowerBound, unsigned int inUpperBound) {
		schnaps_StackTraceBeginM();
		unsigned int lThreadNb = mContext->getThreadNb();
		unsigned int lNbThreads = Core::castObjectT<const Core::UInt&>(mContext->getSystem().getParameters().getParameter("threads.simulator")).getValue();

		for (unsigned int i = inLowerBound; i < inUpperBound; i++) {
			if (i % lNbThreads == lThreadNb) {
				mNewIndexes.push_back(i);
			}
		}
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::addNewIndexes(unsigned int, unsigned int)");
	}

	/*!
	 * \brief Set the label of scenario to simulate.
	 * \param inScenarioLabel A const reference to the scenario label.
	 */
	void setScenarioLabel(const std::string& inScenarioLabel) {
		schnaps_StackTraceBeginM();
		mScenarioLabel = inScenarioLabel;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::setScenarioLabel(const std::string&)");
	}

	/*!
	 * \brief Set the position of thread in simulation.
	 * \param inPosition The position of thread in simulation.
	 */
	void setPosition(Position inPosition) {
		schnaps_StackTraceBeginM();
		mPosition = inPosition;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::setPosition(SCHNAPS::Simulation::SimulationThread::Position)");
	}

	/*!
	 * \brief Reset the indexes of individuals to simulate.
	 */
	void resetIndexes() {
		schnaps_StackTraceBeginM();
		mIndexes.clear();
		mNewIndexes.clear();
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationThread::resetIndexes()");
	}

	/*!
	 * \brief  Return a reference to the simulation context in use.
	 * \return A reference to the simulation context in use.
	 * \throw  SCHNAPS::Core::AssertException if the simulation context is NULL.
	 */
	SimulationContext& getContext() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mContext);
		return *mContext;
		schnaps_StackTraceEndM("SCHNAPS::Simulation::SimulationContext& SCHNAPS::Simulation::SimulationThread::getContext()");
	}

	/*!
	 * \brief  Return a reference to the blackboard.
	 * \return A reference to the blackboard.
	 * \throw  SCHNAPS::Core::AssertException if the blackboard is NULL.
	 */
	BlackBoard& getBlackBoard() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mBlackBoard);
		return *mBlackBoard;
		schnaps_StackTraceEndM("SCHNAPS::Simulation::BlackBoard& SCHNAPS::Simulation::SimulationThread::getBlackBoard()");
	}

	/*!
	 * \brief  Return a reference to the maps of waiting FIFOs (for each individual).
	 * \return A reference to the maps of waiting FIFOs (for each individual).
	 * \throw  SCHNAPS::Core::AssertException if the maps of waiting FIFOs is NULL.
	 */
	WaitingQMaps& getWaitingQMaps() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mWaitingQMaps);
		return *mWaitingQMaps;
		schnaps_StackTraceEndM("SCHNAPS::Simulation::WaitingQMaps& SCHNAPS::Simulation::SimulationThread::getWaitingQMaps()");
	}

	/*! 
	 * \brief  Return a const reference to the indexes of individuals to simulate.
	 * \return A const reference to the indexes of individuals to simulate.
	 */
	const std::vector<unsigned int>& getIndexes() const {
		schnaps_StackTraceBeginM();
		return mIndexes;
		schnaps_StackTraceEndM("const std::vector<unsigned int>& SCHNAPS::Simulation::SimulationThread::getIndexes() const");
	}

	/*! 
	 * \brief  Return a const reference to the indexes of newly added individuals to simulate.
	 * \return A const reference to the indexes of newly added individuals to simulate.
	 */
	const std::vector<unsigned int>& getNewIndexes() const {
		schnaps_StackTraceBeginM();
		return mNewIndexes;
		schnaps_StackTraceEndM("const std::vector<unsigned int>& SCHNAPS::Simulation::SimulationThread::getNewIndexes() const");
	}

	/*!
	 * \brief  Return a const reference to the label of scenario to simulate.
	 * \return A const reference to the label of scenario to simulate.
	 */
	const std::string& getScenarioLabel() const {
		schnaps_StackTraceBeginM();
		return mScenarioLabel;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::SimulationThread::getScenarioLabel() const");
	}

protected:
	void main();

private:
	PACC::Threading::Condition* mParallel;		//!< A pointer to the condition for starting parallel execution.
	PACC::Threading::Semaphore* mSequential;	//!< A pointer to the semaphore for triggering sequential execution (by main thread).
	PACC::Threading::Semaphore* mBlackBoardWrt;	//!< A pointer to the semaphore for waiting after blackboard.

	SimulationContext::Handle mContext;			//!< A handle to the simulation context in use for tree evaluation.
	BlackBoard::Handle mBlackBoard;				//!< A handle to the blackboard for process push.
	WaitingQMaps::Handle mWaitingQMaps;			//!< A handle to maps of waiting FIFOs for each individual.

	std::vector<unsigned int> mIndexes;			//!< Indexes of individuals tu simulate.
	std::vector<unsigned int> mNewIndexes;		//!< Indexes of newly added individuals to simulate.

	std::string mScenarioLabel;					//!< The label of scenario to simulate.
	Position mPosition;							//!< The position of threads in execution.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_SimulationThread_hpp */
