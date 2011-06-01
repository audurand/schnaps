/*
 * UpdateThread.hpp
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

#ifndef SCHNAPS_Plugins_Learning_UpdateThread_hpp
#define SCHNAPS_Plugins_Learning_UpdateThread_hpp

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/PACC.hpp"

#include <list>

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class UpdateThread SCHNAPS/Plugins/Learning/UpdateThread.hpp "SCHNAPS/Plugins/Learning/UpdateThread.hpp"
 *  \brief Thread for updating the knowledge with previous tries and logged data.
 */
class UpdateThread: public Core::Object, public PACC::Threading::Thread {
	struct Log {
		std::string mDecisionNode;
		std::string mState;
		unsigned int mActionID;
		double mReward;
		
		Log(const std::string& inDecisionNode, const std::string& inState, unsigned int inActionID, double inReward) :
			mDecisionNode(inDecisionNode.c_str()),
			mState(inState.c_str()),
			mActionID(inActionID),
			mReward(inReward)
		{}
	};
	
public:
	//! UpdateThread allocator type.
	typedef Core::AllocatorT<UpdateThread, Core::Object::Alloc> Alloc;
	//! UpdateThread handle type.
	typedef Core::PointerT<UpdateThread, Core::Object::Handle> Handle;
	//! UpdateThread bag type.
	typedef Core::ContainerT<UpdateThread, Core::Object::Bag> Bag;

	//! The position of threads in the simulation process.
	enum Position {eREADANDUPDATE, eUPDATE, eEND};

	explicit UpdateThread(PACC::Threading::Condition* inParallel,
		PACC::Threading::Semaphore* inSequential,
		const std::string& inFileName);
	~UpdateThread();

	/*!
	 * \brief Set the decision maker to update.
	 * \param inDecisionMaker A handle to the decision maker to update.
	 */
	void setDecisionMaker(DecisionMaker::Handle inDecisionMaker) {
		schnaps_StackTraceBeginM();
		mDecisionMaker = inDecisionMaker;
		schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::UpdateThread::setDecisionMaker(SCHNAPS::Plugins::Learning::DecisionMaker::Handle)");
	}

	/*!
	 * \brief Set the position of thread in update process.
	 * \param inPosition The position of thread in update process.
	 */
	void setPosition(Position inPosition) {
		schnaps_StackTraceBeginM();
		mPosition = inPosition;
		schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::UpdateThread::setPosition(SCHNAPS::Plugins::Learning::UpdateThread::Position)");
	}

	/*!
	 * \brief  Return a reference to the list of logs read by this thread.
	 * \return A reference to the list of logs read by this thread.
	 */
	std::list<Log>& getLogList() {
		schnaps_StackTraceBeginM();
		return mLogList;
		schnaps_StackTraceEndM("SCHNAPS::Plugins::Learning::std::list<SCHNAPS::Plugins::Learning::UpdateThread::Log>& SCHNAPS::Plugins::Learning::UpdateThread::getLogList()");
	}

	/*!
	 * \brief  Return a handle to the decision maker to update.
	 * \return A handle to the decision maker to update.
	 */
	DecisionMaker::Handle getDecisionMaker() {
		schnaps_StackTraceBeginM();
		return mDecisionMaker;
		schnaps_StackTraceEndM("SCHNAPS::Plugins::Learning::DecisionMaker::Handle SCHNAPS::Plugins::Learning::UpdateThread::getDecisionMaker()");
	}

protected:
	void main();

private:
	PACC::Threading::Condition* mParallel;		//!< A pointer to the condition for starting parallel execution.
	PACC::Threading::Semaphore* mSequential;	//!< A pointer to the semaphore for triggering sequential execution (by main thread).

	std::string mFileName;						//!< The name of log input file from which to update.
	std::list<Log> mLogList;					//!< The list of logs read by this thread.
	DecisionMaker::Handle mDecisionMaker;		//!< A handle to the decision maker to update.

	Position mPosition;							//!< The position of threads in execution.
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_UpdateThread_hpp */
