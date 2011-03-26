/*
 * IsBetweenComplex.cpp
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
IsBetweenComplex::IsBetweenComplex() :
	Primitive(3)
{}

/*!
 * \brief Construct a "is between" comparison primitive as a copy of an original.
 * \param inOriginal A const reference to the original "is between" comparison primitive.
 */
IsBetweenComplex::IsBetweenComplex(const IsBetweenComplex& inOriginal) :
	Primitive(3)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
IsBetweenComplex& IsBetweenComplex::operator=(const IsBetweenComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::IsBetweenComplex& SCHNAPS::Plugins::Operators::IsBetweenComplex::operator=(const SCHNAPS::Plugins::Operators::IsBetweenComplex&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle IsBetweenComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext)); // value to compare
	Core::Number::Handle lArg2 = Core::castHandleT<Core::Number>(getArgument(inIndex, 1, ioContext)); // min
	Core::Number::Handle lArg3 = Core::castHandleT<Core::Number>(getArgument(inIndex, 2, ioContext)); // max
	return new Core::Bool((lArg2->isLess(*lArg1) || lArg1->isEqual(*lArg2)) && (lArg1->isLess(*lArg3) || lArg1->isEqual(*lArg3)));
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsBetweenComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssterException if the argument index is out of bounds.
 */
const std::string& IsBetweenComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 2);
	if (inN == 0) {
		const static std::string lType("Core::Number");
		return lType;
	}
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetweenComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IsBetweenComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetweenComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
