/*
 * And.cpp
 *
 *  Created on: 2009-02-19
 *  Updated on: 2010-12-22
 *      Author: Audrey Durand
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
 *  \brief Construct a new boolean AND primitive.
 */
And::And() :
		Primitive()	// Unknown number of children
{}

/*!
 *  \brief Construct a new boolean AND primitive.
 */
And::And(const And& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

And& And::operator=(const And& inOriginal) {
	schnaps_StackTraceBeginM();
		setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("core::And& core::And::operator=(const core::And&)");
}

core::AnyType::Handle And::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 0);
		bool lResult = true;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResult = lResult && core::castHandleT<core::Bool>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new core::Bool(lResult);
	schnaps_StackTraceEndM("core::AnyType::Handle core::And::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& And::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::And::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& And::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const
{
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::And::getReturnType(unsigned int, core::ExecutionContext&) const");
}
