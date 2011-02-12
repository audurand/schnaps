/*
 * Not.cpp
 *
 *  Created on: 2010-02-20
 *  Author: Audrey Durand
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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

/*!
 *  \brief Construct a new boolean NOT primitive.
 */
Not::Not() :
		Primitive(1)
{}

Not::Not(const Not& inOriginal) :
		Primitive(1)
{}

Not& Not::operator=(const Not& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Not& core::Not::operator=(const core::Not&)");
}

/*!
 *  \brief Execute the characteristic operation of NOT on a boolean.
 *  \param ioContext Execution context.
 *  \param inIndex The index of the node in the primitive tree.
 *  \return Result of the execution.
 */
core::AnyType::Handle Not::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Bool::Handle lArg1 = core::castHandleT<core::Bool>(getArgument(inIndex, 0, ioContext));
		return new core::Bool(!lArg1->getValue());
	schnaps_StackTraceEndM("core::AnyType::Handle core::Not::execute(unsigned int, core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Not.
 *  \param  inIndex Index of the current primitive.
 *  \param  inN Index of the argument to get the type.
 *  \param  ioContext Execution context.
 *  \return Type of the argument's node type.
 */
const std::string& Not::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Not::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}


/*!
 *  \brief  Return the tag of the type of data return by primitive Not.
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return Type of the primitive return type.
 */
const std::string& Not::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Not::getReturnType(unsigned int, core::ExecutionContext&) const");
}
