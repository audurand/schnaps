/*
 * IsGreater.cpp
 *
 *  Created on: 2010-04-03
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
 *  \brief Construct a new IsGreater primitive.
 */
IsGreater::IsGreater() :
		Primitive(2)
{}

/*!
 *  \brief Construct a new IsGreater primitive.
 */
IsGreater::IsGreater(const IsGreater& inOriginal) :
		Primitive(2)
{}

IsGreater& IsGreater::operator=(const IsGreater& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::IsGreater& core::IsGreater::operator=(const core::IsGreater&)");
}

core::AnyType::Handle IsGreater::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext));
		core::Number::Handle lArg2 = core::castHandleT<core::Number>(getArgument(inIndex, 1, ioContext));
		return new core::Bool(lArg2->isLess(*lArg1));
	schnaps_StackTraceEndM("core::AnyType::Handle core::IsGreater::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& IsGreater::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<2);
		if (inN == 0) {
			const static std::string lType("core::Number");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::IsGreater::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& IsGreater::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::IsGreater::getReturnType(unsigned int, core::ExecutionContext&) const");
}
