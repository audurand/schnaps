/*
 * GenerationThread.cpp
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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

GenerationThread::GenerationThread(PACC::Threading::Condition* inParallel,
									PACC::Threading::Semaphore* inSequential,
									GenerationContext::Handle inContext) :
		mParallel(inParallel),
		mSequential(inSequential),
		mContext(inContext),
		mIndividuals(new Individual::Bag())
{
	run();
}

GenerationThread::~GenerationThread() {
	wait();
}

void GenerationThread::main() {
	bool lDone = false;

	mParallel->lock();
	mSequential->post();
	mParallel->wait();
	mParallel->unlock();

	do {
		switch (mPosition) {
		case eGENERATION:
			Generator::buildIndividuals(this);

			mParallel->lock();
			mSequential->post();
			mParallel->wait();
			mParallel->unlock();
			break;
		default: // eEND;
			lDone = true;
			break;
		}
	} while (!lDone);

	// Ready for deletion
	mParallel->lock();
	mSequential->post();
	mParallel->unlock();
}
