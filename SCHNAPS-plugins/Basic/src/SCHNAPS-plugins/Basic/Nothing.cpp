/*
 * Nothing.cpp
 *
 *  Created on: 2009-11-20
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
 *  \brief Construct a new nothing terminal primitive.
 */
Nothing::Nothing() :
		Primitive(0)
{}

/*!
 *  \brief Construct a new nothing terminal primitive.
 */
Nothing::Nothing(const Nothing& inOriginal) :
		Primitive(0)
{}

Nothing& Nothing::operator=(const Nothing& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Nothing& core::Nothing::operator=(const core::Nothing&)");
}

core::AnyType::Handle Nothing::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle core::Nothing::execute(unsigned int, core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive Nothing.
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return Type of the primitive return type.
 */
const std::string& Nothing::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Nothing::getReturnType(unsigned int, core::ExecutionContext&) const");
}
