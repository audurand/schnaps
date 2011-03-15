/*
 * Sequential.cpp
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
Sequential::Sequential() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct a primitive for sequentially executing branches as a copy of an original.
 * \param inOriginal A const reference to the original primitive for sequentially executing branches.
 */
Sequential::Sequential(const Sequential& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Sequential& Sequential::operator=(const Sequential& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::Sequential& SCHNAPS::Plugins::Control::Sequential::operator=(const SCHNAPS::Plugins::Control::Sequential&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Sequential::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() == 0) {
		return NULL;
	}

	for (unsigned int i = 0; i < getNumberArguments()-1; i++) {
		getArgument(inIndex, i, ioContext);
	}
	return getArgument(inIndex, getNumberArguments()-1, ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::Sequential::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Sequential::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Sequential::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Sequential::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() == 0) {
		const static std::string lType("Void");
		return lType;
	}
	return getArgType(inIndex, getNumberArguments()-1, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Sequential::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
