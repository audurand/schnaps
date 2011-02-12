/*
 * IfThenElse.cpp
 *
 *  Created on: 2009-03-10
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
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse() :
		Primitive(3)
{}

/*!
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse(const IfThenElse& inOriginal) :
		Primitive(3)
{}

IfThenElse& IfThenElse::operator=(const IfThenElse& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::IfThenElse& core::IfThenElse::operator=(const core::IfThenElse&)");
}

core::AnyType::Handle IfThenElse::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Bool::Handle lArg1 = core::castHandleT<core::Bool>(getArgument(inIndex, 0, ioContext));	// If
		if (lArg1->getValue()) {
			core::AnyType::Handle lArg2 = getArgument(inIndex, 1, ioContext);	// Then
			return lArg2;
		}
		core::AnyType::Handle lArg3 = getArgument(inIndex, 2, ioContext);	// Else
		return lArg3;
	schnaps_StackTraceEndM("core::AnyType::Handle core::IfThenElse::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& IfThenElse::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<3);
		if (inN == 0) {
			const static std::string lType("core::Bool");
			return lType;
		}
		else if (inN == 1) {
			const static std::string lType("Any");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::IfThenElse::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& IfThenElse::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));
		return lCommonType;
	schnaps_StackTraceEndM("const std::string& core::IfThenElse::getReturnType(unsigned int, core::ExecutionContext&) const");
}
