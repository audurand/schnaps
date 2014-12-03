/*
 * AndComplex.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2014 by Audrey DurAndComplex
 *
 * This program is free software: you can redistribute it AndComplex/or modify
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
AndComplex::AndComplex() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct logical AND primitive as a copy of an original.
 * \param inOriginal A const reference to the original logical AND primitive.
 */
AndComplex::AndComplex(const AndComplex& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
AndComplex& AndComplex::operator=(const AndComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::AndComplex& SCHNAPS::Plugins::Operators::AndComplex::operator=(const SCHNAPS::Plugins::Operators::AndComplex&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A hAndComplexle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the number of arguments is equal to zero.
 */
Core::AnyType::Handle AndComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() < 1) {
		std::stringstream lOSS;
		lOSS << "The number of arguments is equal to 0; ";
		lOSS << "could not perform the logical AndComplex.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	
	bool lResult = true;
	Core::Bool::Handle lArg;
	for (unsigned int i = 0; i < getNumberArguments(); i++) {
	    lArg = Core::castHandleT<Core::Bool>(getArgument(inIndex, i, ioContext));
		lResult = lResult && lArg->getValue();
	}
	return new Core::Bool(lResult);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::AndComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& AndComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::AndComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& AndComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::AndComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
