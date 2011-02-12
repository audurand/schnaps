/*
 * SimulationThread.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

SimulationThread::SimulationThread(PACC::Threading::Condition* inParallel,
		PACC::Threading::Semaphore* inSequential,
		PACC::Threading::Semaphore* inBlackBoardWrt,
		SimulationContext::Handle inContext,
		BlackBoard::Handle inBlackBoard,
		WaitingQMaps::Handle inWaitingQMaps) :
			mParallel(inParallel),
			mSequential(inSequential),
			mBlackBoardWrt(inBlackBoardWrt),
			mContext(inContext),
			mBlackBoard(inBlackBoard),
			mWaitingQMaps(inWaitingQMaps)
{
	run();
}

SimulationThread::~SimulationThread() {
	wait();
}

void SimulationThread::main() {
	bool lDone = false;

	mParallel->lock();
	mSequential->post();
	mParallel->wait();
	mParallel->unlock();

	do {
		switch (mPosition) {
		case eSTEP:
			if (mIndexes.size() > 0) {
				Simulator::processClockStep(this);
			}
			if (mNewIndexes.size() > 0) {
				Simulator::processScenario(this);
				mIndexes.reserve(mIndexes.size()+mNewIndexes.size());
				mIndexes.insert(mIndexes.end(), mNewIndexes.begin(), mNewIndexes.end());
				mNewIndexes.clear();
			}
			mPosition = eSUBSTEP;

			mParallel->lock();
			mSequential->post();
			mParallel->wait();
			mParallel->unlock();
			break;
		case eSUBSTEP:
			Simulator::processSubStep(this);

			mParallel->lock();
			mSequential->post();
			mParallel->wait();
			mParallel->unlock();
			break;
		default: // eEND
			lDone = true;
			break;
		}
	} while (!lDone);

	// Ready for deletion
	mParallel->lock();
	mSequential->post();
	mParallel->unlock();
}
