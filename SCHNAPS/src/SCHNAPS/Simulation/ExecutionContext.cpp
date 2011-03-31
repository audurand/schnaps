/*
 * ExecutionContext.cpp
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
ExecutionContext::ExecutionContext() :
		Core::ExecutionContext(),
		mClock(NULL),
		mEnvironment(NULL),
		mIndividual(NULL)
{}

/*!
 * \brief Construct a simulation execution context as a copy of an original.
 * \param inOriginal The original simulation execution context.
 */
ExecutionContext::ExecutionContext(const ExecutionContext& inOriginal) :
		Core::ExecutionContext(inOriginal),
		mClock(inOriginal.mClock),
		mEnvironment(inOriginal.mEnvironment),
		mIndividual(inOriginal.mIndividual),
		mLocalVariables(inOriginal.mLocalVariables)
{}

/*!
 * \brief Construct a simulation context with specific characteristics.
 * \param inSystem A handle to the system.
 * \param inClock A handle to the clock.
 * \param inEnvironment A handle to the environment.
 */
ExecutionContext::ExecutionContext(const Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment) :
		Core::ExecutionContext(inSystem),
		mClock(inClock),
		mEnvironment(inEnvironment),
		mIndividual(NULL)
{}

/*!
 * \brief Erase all local variables.
 */
void ExecutionContext::clearLocalVariables() {
	schnaps_StackTraceBeginM();
	mLocalVariables.clear();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ExecutionContext::clearLocalVariables()");
}

/*!
 * \brief Insert a local variable.
 * \param inLabel A const reference to the label of the local variable.
 * \param inValue A handle to the value of the local variable.
 * \throw SCHNAPS::Core::RunTimeException if the local variable already exists.
 */
void ExecutionContext::insertLocalVariable(const std::string& inLabel, Core::AnyType::Handle inValue) {
	schnaps_StackTraceBeginM();
	if (mLocalVariables.find(inLabel) != mLocalVariables.end()) {
		std::ostringstream lOSS;
		lOSS << "The local variable '" << inLabel << "' already exists; ";
		lOSS << "could not insert it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mLocalVariables.insert(std::pair<std::string, Core::AnyType::Handle>(inLabel.c_str(), inValue));
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ExecutionContext::insertLocalVariable(const std::string&, SCHNAPS::Core::Atom::Handle) const");
}
