/*
 * Parallel.cpp
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
Parallel::Parallel() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct a primitive for executing branches "in parallel" as a copy of an original.
 * \param inOriginal A const reference to the original primitive for executing branches "in parallel".
 */
Parallel::Parallel(const Parallel& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Parallel& Parallel::operator=(const Parallel& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::Parallel& SCHNAPS::Plugins::Control::Parallel::operator=(const SCHNAPS::Plugins::Control::Parallel&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Parallel::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Core::Vector::Handle lResults = new Core::Vector();
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResults->push_back(getArgument(inIndex, i, ioContext));
		}
		return lResults;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::Parallel::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Parallel::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Parallel::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Parallel::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Vector");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Parallel::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
