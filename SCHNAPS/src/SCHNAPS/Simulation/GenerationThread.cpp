/*
 * GenerationThread.cpp
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
	// wait for initialization by main thread
	mParallel->lock();
	mSequential->post();
	mParallel->wait();
	mParallel->unlock();
	
	// process
	Generator::buildIndividuals(this);

	// done
	mParallel->lock();
	mSequential->post();
	mParallel->unlock();
}
