/*
 * Serialize.cpp
 *
 *  Created on: 2010-02-01
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
 *  \brief Construct a new absolute value primitive.
 */
Serialize::Serialize() :
		Primitive(1)
{}

/*!
 *  \brief Construct a new absolute value primitive.
 */
Serialize::Serialize(const Serialize& inOriginal) :
		Primitive(1)
{}

Serialize& Serialize::operator=(const Serialize& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Serialize& core::Serialize::operator=(const core::Serialize&)");
}

/*!
 *  \brief Execute the characteristic primitive operation.
 *  \param ioContext Execution context.
 *  \param inIndex The index of the node in the primitive tree.
 *  \return Result of the execution.
 */
core::AnyType::Handle Serialize::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return new core::String(getArgument(inIndex, 0, ioContext)->writeStr().c_str());
	schnaps_StackTraceEndM("core::AnyType::Handle core::Serialize::execute(unsigned int, core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the tag of the type of data needed as input for the primitive Serialize.
 *  \param  inIndex Index of the current primitive.
 *  \param  inN Index of the argument to get the type.
 *  \param  ioContext Execution context.
 *  \return Type of the argument's node type.
 */
const std::string& Serialize::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Serialize::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive Serialize.
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return Type of the primitive return type.
 */
const std::string& Serialize::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
			const static std::string lType("String");
			return lType;
	schnaps_StackTraceEndM("const std::string& core::Serialize::getReturnType(unsigned int, core::ExecutionContext&) const");
}
