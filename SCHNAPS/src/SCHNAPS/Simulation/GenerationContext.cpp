/*
 * GenerationContext.cpp
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
GenerationContext::GenerationContext() :
	ExecutionContext(),
	mGenProfile(NULL)
{
	mName = "GenerationContext";
}

/*!
 * \brief Construct a generation context as a copy of an original.
 * \param inOriginal A const reference to the original generation context.
 */
GenerationContext::GenerationContext(const GenerationContext& inOriginal) :
	ExecutionContext(inOriginal),
	mGenProfile(inOriginal.mGenProfile)
{
	mName = "GenerationContext";
}

/*!
 * \brief Construct a generation context with specific system, clock, and environment.
 * \param inSystem A handle to the system.
 * \param inClock A handle to the clock.
 * \param inEnvironment A handle to the environment.
 */
GenerationContext::GenerationContext(Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
	ExecutionContext(inSystem, inClock, inEnvironment),
	mGenProfile(NULL)
{
	mName = "GenerationContext";
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
GenerationContext& GenerationContext::operator=(const GenerationContext& inOriginal) {
	mSystem = inOriginal.mSystem;
	mClock = inOriginal.mClock;
	mEnvironment = inOriginal.mEnvironment;
	mGenProfile = inOriginal.mGenProfile;
	mIndividual = inOriginal.mIndividual;
	return *this;
}

/*!
 * \brief  Return a handle to a deep copy of the generation context.
 * \return A handle to a deep copy of the generation context.
 */
GenerationContext::Handle GenerationContext::deepCopy() const {
	schnaps_StackTraceBeginM();
	GenerationContext::Handle lCopy = new GenerationContext(mSystem, mClock, mEnvironment);

	// set profile as a copy of the current one
	lCopy->setGenProfile(Core::castHandleT<GenProfile>(mGenProfile->deepCopy(*mSystem)));

	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::GenerationContext& SCHNAPS::Simulation::GenerationContext::operator=(const SCHNAPS::Simulation::GenerationContext&)");
}
