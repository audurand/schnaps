/*
 * IsEqual.cpp
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
IsEqual::IsEqual() :
	Primitive(2)
{}

/*!
 * \brief Construct an equality comparison operator as a copy of an original.
 * \param inOriginal A const reference to the original equality comparison operator.
 */
IsEqual::IsEqual(const IsEqual& inOriginal) :
	Primitive(2)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
IsEqual& IsEqual::operator=(const IsEqual& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::IsEqual& SCHNAPS::Plugins::Operators::IsEqual::operator=(const SCHNAPS::Plugins::Operators::IsEqual&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle IsEqual::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Atom::Handle lArg1 = Core::castHandleT<Core::Atom>(getArgument(inIndex, 0, ioContext));
	Core::Atom::Handle lArg2 = Core::castHandleT<Core::Atom>(getArgument(inIndex, 1, ioContext));
	return new Core::Bool(lArg1->isEqual(*lArg2));
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsEqual::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& IsEqual::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	if (inN == 0) {
		const static std::string lType("Core::Atom");
		return lType;
	}
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsEqual::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IsEqual::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsEqual::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
