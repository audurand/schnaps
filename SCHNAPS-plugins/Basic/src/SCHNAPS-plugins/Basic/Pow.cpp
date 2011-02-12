/*
 * Pow.cpp
 *
 *  Created on: 2010-08-26
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
 *  \brief Construct a new exponent primitive.
 */
Pow::Pow() :
		Primitive(2)
{}

/*!
 *  \brief Construct a new exponent primitive.
 */
Pow::Pow(const Pow& inOriginal) :
		Primitive(2)
{}

Pow& Pow::operator=(const Pow& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Pow& core::Pow::operator=(const core::Pow&)");
}

core::AnyType::Handle Pow::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext));
		core::Number::Handle lArg2 = core::castHandleT<core::Number>(getArgument(inIndex, 1, ioContext));
		lArg1->pow(*lArg2);
		return lArg1;
	schnaps_StackTraceEndM("core::AnyType::Handle core::Pow::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Pow::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<2);
		if (inN == 0) {
			const static std::string lType("Number");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Pow::getArgType(unsigned int , unsigned int, core::ExecutionContext&) const");
}

const std::string& Pow::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Pow::getReturnType(unsigned int, core::ExecutionContext&) const");
}
