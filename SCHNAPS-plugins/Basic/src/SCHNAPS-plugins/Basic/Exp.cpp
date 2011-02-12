/*
 * Exp.cpp
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
 *  \brief Construct a new exponent primitive.
 */
Exp::Exp() :
		Primitive(1)
{}

/*!
 *  \brief Construct a new exponent primitive.
 */
Exp::Exp(const Exp& inOriginal) :
		Primitive(1)
{}

Exp& Exp::operator=(const Exp& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Exp& core::Exp::operator=(const core::Exp&)");
}

core::AnyType::Handle Exp::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext));
		lArg1->exp();
		return lArg1;
	schnaps_StackTraceEndM("core::AnyType::Handle core::Exp::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Exp::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
		const static std::string lType("Number");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Exp::getArgType(unsigned int , unsigned int, core::ExecutionContext&) const");
}

const std::string& Exp::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Exp::getReturnType(unsigned int, core::ExecutionContext&) const");
}
