/*
 * ExecutionContext.cpp
 *
 *  Created on: 2010-03-26
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

simulator::ExecutionContext::ExecutionContext() :
		core::ExecutionContext(),
		mName("ExecutionContext"),
		mClock(NULL),
		mEnvironment(NULL),
		mIndividual(NULL)
{}

simulator::ExecutionContext::ExecutionContext(const simulator::ExecutionContext& inOriginal) :
		core::ExecutionContext(inOriginal),
		mName("ExecutionContext"),
		mClock(inOriginal.mClock),
		mEnvironment(inOriginal.mEnvironment),
		mIndividual(inOriginal.mIndividual)
{}

simulator::ExecutionContext::ExecutionContext(const core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment) :
		core::ExecutionContext(inSystem),
		mName("ExecutionContext"),
		mClock(inClock),
		mEnvironment(inEnvironment),
		mIndividual(NULL)
{}
