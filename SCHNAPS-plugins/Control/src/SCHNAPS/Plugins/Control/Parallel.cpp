/*
 * Parallel.cpp
 *
 *  Created on: 2010-08-27
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new primitive that executes sequentially its children but returns a vector containing the result of each one (as if executed in parallel).
 */
Parallel::Parallel() :
		Primitive()	// unknown number of children
{}

/*!
 *  \brief Construct a new primitive that returns the result of a Parallel.
 */
Parallel::Parallel(const Parallel& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

Parallel& Parallel::operator=(const Parallel& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::Parallel& SCHNAPS::Plugins::Control::Parallel::operator=(const SCHNAPS::Plugins::Control::Parallel&)");
}

SCHNAPS::Core::AnyType::Handle Parallel::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::Vector::Handle lResults = new SCHNAPS::Core::Vector();
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResults->push_back(getArgument(inIndex, i, ioContext));
		}
		return lResults;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::Parallel::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

const std::string& Parallel::getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Parallel::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& Parallel::getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Vector");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Parallel::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
