/*
 * SimulationThread.hpp
 *
 *  Created on: 2010-04-10
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

#ifndef simulator_SimulationThread_hpp
#define simulator_SimulationThread_hpp

#include "PACC/PACC.hpp"

#include "SCHNAPS/Simulator/SimulationContext.hpp"
#include "SCHNAPS/Simulator/BlackBoard.hpp"
#include "SCHNAPS/Simulator/WaitingQMaps.hpp"

namespace simulator {

/*!
 *  \class SimulationThread Simulator/include/SimulationThread.hpp "Simulator/include/SimulationThread.hpp"
 *  \brief SimulationThread class.
 */
class SimulationThread: public core::Object, public PACC::Threading::Thread {
public:
	//! SimulationThread allocator type.
	typedef core::AllocatorT<SimulationThread, core::Object::Alloc> Alloc;
	//! SimulationThread handle type.
	typedef core::PointerT<SimulationThread, core::Object::Handle> Handle;
	//! SimulationThread bag type.
	typedef core::ContainerT<SimulationThread, core::Object::Bag> Bag;

	enum Position {eSTEP, eSUBSTEP, eEND};

	explicit SimulationThread(PACC::Threading::Condition* inParallel,
					PACC::Threading::Semaphore* inSequential,
					PACC::Threading::Semaphore* inBlackBoardWrt,
					SimulationContext::Handle inContext,
					BlackBoard::Handle inBlackBoard,
					WaitingQMaps::Handle inWaitingQMaps);
	~SimulationThread();

	void waitBlackBoard() {
		mBlackBoardWrt->wait();
	}

	void postBlackBoard() {
		mBlackBoardWrt->post();
	}

	void addNewIndexes(unsigned int inLowerBound, unsigned int inUpperBound) {
		unsigned int lThreadNb = mContext->getThreadNb();
		unsigned int lNbThreads = core::castHandleT<core::UInt>(mContext->getSystem().getParameters()["threads.simulator"])->getValue();

		for (unsigned int i = inLowerBound; i < inUpperBound; i++) {
			if (i % lNbThreads == lThreadNb) {
				mNewIndexes.push_back(i);
			}
		}
	}

	void setScenarioLabel(std::string inScenarioLabel) {
		mScenarioLabel = inScenarioLabel;
	}

	void setPosition(Position inPosition) {
		mPosition = inPosition;
	}

	void resetIndexes() {
		mIndexes.clear();
		mNewIndexes.clear();
	}

	SimulationContext& getContext() {
		schnaps_NonNullPointerAssertM(mContext);
		return *mContext;
	}

	BlackBoard& getBlackBoard() {
		schnaps_NonNullPointerAssertM(mBlackBoard);
		return *mBlackBoard;
	}

	WaitingQMaps& getWaitingQMaps() {
		schnaps_NonNullPointerAssertM(mWaitingQMaps);
		return *mWaitingQMaps;
	}

	const std::vector<unsigned int>& getIndexes() const {
		return mIndexes;
	}

	const std::vector<unsigned int>& getNewIndexes() const {
		return mNewIndexes;
	}

	const std::string& getScenarioLabel() const {
		return mScenarioLabel;
	}

protected:
	void main();

private:
	PACC::Threading::Condition* mParallel;
	PACC::Threading::Semaphore* mSequential;
	PACC::Threading::Semaphore* mBlackBoardWrt;

	SimulationContext::Handle mContext;
	BlackBoard::Handle mBlackBoard;
	WaitingQMaps::Handle mWaitingQMaps;

	std::vector<unsigned int> mIndexes;
	std::vector<unsigned int> mNewIndexes;

	std::string mScenarioLabel;
	Position mPosition;
};
} // end of simulator namespace

#endif /* simulator_SimulationThread_hpp */
