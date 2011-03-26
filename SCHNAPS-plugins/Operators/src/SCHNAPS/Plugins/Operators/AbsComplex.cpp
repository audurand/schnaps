/*
 * AbsComplex.cpp
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
AbsComplex::AbsComplex() :
	Primitive(1)
{}

/*!
 * \brief Construct an absolute value primitive as a copy of an original.
 * \param inOriginal A const reference to the original absolute value primitive.
 */
AbsComplex::AbsComplex(const AbsComplex& inOriginal) :
	Primitive(1)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
AbsComplex& AbsComplex::operator=(const AbsComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::AbsComplex& SCHNAPS::Plugins::Operators::AbsComplex::operator=(const SCHNAPS::Plugins::Operators::AbsComplex&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle AbsComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	return lArg1->abs();
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::AbsComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& AbsComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 0);
	const static std::string lType("Number");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::AbsComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}


/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& AbsComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::AbsComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
