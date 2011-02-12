/*
 * IsBetween.cpp
 *
 *  Created on: 2010-08-19
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
 *  \brief Construct a new IsBetween primitive.
 */
IsBetween::IsBetween() :
		Primitive(3)
{}

/*!
 *  \brief Construct a new IsBetween primitive.
 */
IsBetween::IsBetween(const IsBetween& inOriginal) :
		Primitive(3)
{}

IsBetween& IsBetween::operator=(const IsBetween& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::IsBetween& core::IsBetween::operator=(const core::IsBetween&)");
}

core::AnyType::Handle IsBetween::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext)); // Value to compare
		core::Number::Handle lArg2 = core::castHandleT<core::Number>(getArgument(inIndex, 1, ioContext)); // Min
		core::Number::Handle lArg3 = core::castHandleT<core::Number>(getArgument(inIndex, 2, ioContext)); // Max
		return new core::Bool((lArg2->isLess(*lArg1) || lArg1->isEqual(*lArg2)) && (lArg1->isLess(*lArg3) || lArg1->isEqual(*lArg3)));
	schnaps_StackTraceEndM("core::AnyType::Handle core::IsBetween::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& IsBetween::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<3);
		if (inN == 0) {
			const static std::string lType("core::Number");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::IsBetween::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& IsBetween::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::IsBetween::getReturnType(unsigned int, core::ExecutionContext&) const");
}
