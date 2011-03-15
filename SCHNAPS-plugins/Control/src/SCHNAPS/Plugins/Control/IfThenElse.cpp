/*
 * IfThenElse.cpp
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 * \brief Default constructor.
 */
IfThenElse::IfThenElse() :
	Primitive(3)
{}

/*!
 * \brief Construct a If...Then...Else primitive as copy of an original.
 * \param inOriginal A const reference to the original If...Then...Else primitive.
 */
IfThenElse::IfThenElse(const IfThenElse& inOriginal) :
	Primitive(3)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
IfThenElse& IfThenElse::operator=(const IfThenElse& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::IfThenElse& SCHNAPS::Plugins::Control::IfThenElse::operator=(const SCHNAPS::Plugins::Control::IfThenElse&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
SCHNAPS::Core::AnyType::Handle IfThenElse::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	SCHNAPS::Core::Bool::Handle lArg1 = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(getArgument(inIndex, 0, ioContext));
	// if
	if (lArg1->getValue()) {
		// then
		SCHNAPS::Core::AnyType::Handle lArg2 = getArgument(inIndex, 1, ioContext);
		return lArg2;
	}
	// else
	SCHNAPS::Core::AnyType::Handle lArg3 = getArgument(inIndex, 2, ioContext);
	return lArg3;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::IfThenElse::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& IfThenElse::getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 2);
	if (inN == 0) {
		const static std::string lType("SCHNAPS::Core::Bool");
		return lType;
	}
	else if (inN == 1) {
		const static std::string lType("Any");
		return lType;
	}
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::IfThenElse::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IfThenElse::getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));
	return lCommonType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::IfThenElse::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
