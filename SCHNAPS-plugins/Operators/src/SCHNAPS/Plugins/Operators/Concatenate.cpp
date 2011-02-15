/*
 * Concatenate.cpp
 *
 *  Created on: 2010-06-24
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 *  \brief Construct a new primitive to concatenate strings.
 */
Concatenate::Concatenate() :
		Primitive()	// unknown number of children
{}

/*!
 *  \brief Construct a new primitive to concatenate strings.
 */
Concatenate::Concatenate(const Concatenate& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

Concatenate& Concatenate::operator=(const Concatenate& inOriginal) {
	schnaps_StackTraceBeginM();
		setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("Core::Concatenate& SCHNAPS::Plugins::Operators::Concatenate::operator=(const Core::Concatenate&)");
}

Core::AnyType::Handle Concatenate::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 1);
		std::stringstream lSS;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lSS << Core::castHandleT<Core::String>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new Core::String(lSS.str());
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::Concatenate::execute(unsigned int, Core::ExecutionContext&)");
}

const std::string& Concatenate::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("Core::String");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Concatenate::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}


const std::string& Concatenate::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const
{
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::String");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Concatenate::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
