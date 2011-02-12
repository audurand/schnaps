/*
 * GenerationContext.cpp
 *
 *  Created on: 2010-03-29
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

GenerationContext::GenerationContext() :
	ExecutionContext(),
	mGenProfile(NULL)
{
	mName = "GenerationContext";
}

GenerationContext::GenerationContext(const GenerationContext& inOriginal) :
	ExecutionContext(inOriginal),
	mGenProfile(inOriginal.mGenProfile)
{
	mName = "GenerationContext";
}

GenerationContext::GenerationContext(core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
	ExecutionContext(inSystem, inClock, inEnvironment),
	mGenProfile(NULL)
{
	mName = "GenerationContext";
}

GenerationContext& GenerationContext::operator=(const GenerationContext& inOriginal) {
	schnaps_StackTraceBeginM();
		mSystem = inOriginal.mSystem;
		mClock = inOriginal.mClock;
		mEnvironment = inOriginal.mEnvironment;
		mGenProfile = inOriginal.mGenProfile;
		mIndividual = inOriginal.mIndividual;
		return *this;
	schnaps_StackTraceEndM("simulator::GenerationContext& simulator::GenerationContext::operator=(const simulator::GenerationContext&)");
}

GenerationContext::Handle GenerationContext::deepCopy() const {
	schnaps_StackTraceBeginM();
		GenerationContext::Handle lGenerationContext = new GenerationContext(mSystem, mClock, mEnvironment);

		// Set profile as a copy of the current one
		lGenerationContext->setGenProfile(mGenProfile->deepCopy(*mSystem));

		return lGenerationContext;
	schnaps_StackTraceEndM("simulator::GenerationContext& simulator::GenerationContext::operator=(const simulator::GenerationContext&)");
}
