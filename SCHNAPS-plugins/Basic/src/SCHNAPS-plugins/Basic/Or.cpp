/*
 * Or.cpp
 *
 *  Created on: 2009-02-26
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
 *  \brief Construct a new boolean OR primitive.
 */
Or::Or() :
		Primitive()	// Unknown number of children
{}

/*!
 *  \brief Construct a new boolean OR primitive.
 */
Or::Or(const Or& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

Or& Or::operator=(const Or& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("core::Or& core::Or::operator=(const core::Or&)");
}

core::AnyType::Handle Or::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 0);
		bool lResult = false;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResult = lResult || core::castHandleT<core::Bool>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new core::Bool(lResult);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Or::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Or::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Or::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& Or::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Or::getReturnType(unsigned int, core::ExecutionContext&) const");
}
