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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

/*!
 *  \brief Construct a new primitive to concatenate strings.
 */
Concatenate::Concatenate() :
		Primitive()	// Unknown number of children
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
	schnaps_StackTraceEndM("core::Concatenate& core::Concatenate::operator=(const core::Concatenate&)");
}

core::AnyType::Handle Concatenate::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 1);
		std::stringstream lSS;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lSS << core::castHandleT<core::String>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new core::String(lSS.str());
	schnaps_StackTraceEndM("core::AnyType::Handle core::Concatenate::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& Concatenate::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("core::String");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Concatenate::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}


const std::string& Concatenate::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const
{
	schnaps_StackTraceBeginM();
		const static std::string lType("core::String");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Concatenate::getReturnType(unsigned int, core::ExecutionContext&) const");
}
