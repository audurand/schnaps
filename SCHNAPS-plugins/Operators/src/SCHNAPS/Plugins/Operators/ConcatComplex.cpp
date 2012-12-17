/*
 * ConcatComplex.cpp
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
ConcatComplex::ConcatComplex() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct a string concatenation operator as a copy of an original.
 * \param inOriginal A const reference to the original string concatenation operator.
 */
ConcatComplex::ConcatComplex(const ConcatComplex& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ConcatComplex& ConcatComplex::operator=(const ConcatComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::ConcatComplex& SCHNAPS::Plugins::Operators::ConcatComplex::operator=(const SCHNAPS::Plugins::Operators::ConcatComplex&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the operator does not have enough arguments.
 */
Core::AnyType::Handle ConcatComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() < 2) {
		std::stringstream lOSS;
		lOSS << "The number of arguments is lower than 2; ";
		lOSS << "could not perform the string concatenation.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	std::stringstream lSS;
	for (unsigned int i = 0; i < getNumberArguments(); i++) {
		lSS << Core::castHandleT<Core::String>(getArgument(inIndex, i, ioContext))->getValue();
	}
	return new Core::String(lSS.str());
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::ConcatComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
const std::string& ConcatComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("String");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::ConcatComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
const std::string& ConcatComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("String");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::ConcatComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
