/*
 * MultComplex.cpp
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
MultComplex::MultComplex() :
	Primitive(2)
{}

/*!
 * \brief Construct a multiplication operator as a copy of an original.
 * \param inOriginal A const reference to the original multiplication operator.
 */
MultComplex::MultComplex(const MultComplex& inOriginal) :
	Primitive(2)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
MultComplex& MultComplex::operator=(const MultComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::MultComplex& SCHNAPS::Plugins::Operators::MultComplex::operator=(const SCHNAPS::Plugins::Operators::MultComplex&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle MultComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	Core::Number::Handle lArg2 = Core::castHandleT<Core::Number>(getArgument(inIndex, 1, ioContext));
	return lArg1->mult(*lArg2);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::MultComplex::execute( unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& MultComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	if (inN == 0) {
		const static std::string lType("Number");
		return lType;
	}
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::MultComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& MultComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	return getArgType(inIndex, 0, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::MultComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
