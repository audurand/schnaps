/*
 * IsGreaterOrEqualComplex.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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
 * \brief Default constructor.
 */
IsGreaterOrEqualComplex::IsGreaterOrEqualComplex() :
		Primitive(2)
{}

/*!
 * \brief Construct a "is greater or equal" comparison operator as a copy of and original.
 * \param inOriginal A const reference to the original "is greater or equal" comparison operator.
 */
IsGreaterOrEqualComplex::IsGreaterOrEqualComplex(const IsGreaterOrEqualComplex& inOriginal) :
	Primitive(2)
{}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle IsGreaterOrEqualComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	Core::Number::Handle lArg2 = Core::castHandleT<Core::Number>(getArgument(inIndex, 1, ioContext));
	return new Core::Bool(lArg2->isLess(*lArg1) || lArg1->isEqual(*lArg2));
	schnaps_StackTraceEndM(" SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::Operators::IsGreaterOrEqualComplex::execute(unsigned int,  SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& IsGreaterOrEqualComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	if (inN == 0) {
		const static std::string lType("Core::Number");
		return lType;
	}
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Operators::IsGreaterOrEqualComplex::getArgType(unsigned int, unsigned int,  SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IsGreaterOrEqualComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Operators::IsGreaterOrEqualComplex::getReturnType(unsigned int,  SCHNAPS::Core::ExecutionContext&) const");
}
