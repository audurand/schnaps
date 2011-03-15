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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

SCHNAPS::Simulation::ExecutionContext::ExecutionContext() :
		SCHNAPS::Core::ExecutionContext(),
		mName("ExecutionContext"),
		mClock(NULL),
		mEnvironment(NULL),
		mIndividual(NULL)
{}

SCHNAPS::Simulation::ExecutionContext::ExecutionContext(const SCHNAPS::Simulation::ExecutionContext& inOriginal) :
		SCHNAPS::Core::ExecutionContext(inOriginal),
		mName("ExecutionContext"),
		mClock(inOriginal.mClock),
		mEnvironment(inOriginal.mEnvironment),
		mIndividual(inOriginal.mIndividual)
{}

SCHNAPS::Simulation::ExecutionContext::ExecutionContext(const SCHNAPS::Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment) :
		SCHNAPS::Core::ExecutionContext(inSystem),
		mName("ExecutionContext"),
		mClock(inClock),
		mEnvironment(inEnvironment),
		mIndividual(NULL)
{}
