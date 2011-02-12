/*
 * Mult.cpp
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
 *  \brief Construct a new multiplication primitive.
 */
Mult::Mult() :
		Primitive(2)
{}

/*!
 *  \brief Construct a new multiplication primitive.
 */
Mult::Mult(const Mult& inOriginal) :
		Primitive(2)
{}

Mult& Mult::operator=(const Mult& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Mult& core::Mult::operator=(const core::Mult&)");
}

/*!
 *  \brief Execute the characteristic operation of multiplication.
 *  \param ioContext Execution context.
 *  \param inIndex The index of the node in the primitive tree.
 *  \return Result of the execution.
 */
core::AnyType::Handle Mult::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext));
		core::Number::Handle lArg2 = core::castHandleT<core::Number>(getArgument(inIndex, 1, ioContext));
		lArg1->mult(*lArg2);
		return lArg1;
	schnaps_StackTraceEndM("core::AnyType::Handle core::Mult::execute( unsigned int, core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Mult.
 *  \param  inIndex Index of the current primitive.
 *  \param  inN Index of the argument to get the type.
 *  \param  ioContext Execution context.
 *  \return Type of the argument's node type.
 */
const std::string& Mult::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<2);
		unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Mult::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive Mult.
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return Type of the primitive return type.
 */
const std::string& Mult::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return getArgType(inIndex, 0, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Mult::getReturnType(unsigned int, core::ExecutionContext&) const");
}
