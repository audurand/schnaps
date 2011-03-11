/*
 * Or.cpp
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
Or::Or() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct a logical OR operator as a copy of an original.
 * \param inOriginal A const reference to the original logical OR operator.
 */
Or::Or(const Or& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Or& Or::operator=(const Or& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::Or& SCHNAPS::Plugins::Operators::Or::operator=(const SCHNAPS::Plugins::Operators::Or&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the number of arguments is too low.
 */
Core::AnyType::Handle Or::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() < 1) {
		std::stringstream lOSS;
		lOSS << "The number of arguments is too low; ";
		lOSS << "could not perform the logical OR.";
		schnaps_RunTimeExceptionM(lOSS.str());
	}
	bool lResult = false;
	for (unsigned int i = 0; i < getNumberArguments(); i++) {
		lResult = lResult || Core::castHandleT<Core::Bool>(getArgument(inIndex, i, ioContext))->getValue();
	}
	return new Core::Bool(lResult);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::Or::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Or::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Or::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Or::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Or::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
