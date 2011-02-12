/*
 * Variable.cpp
 *
 *  Created on: 2009-03-17
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
 *  \brief Construct a new token terminal primitive that refers to the clock.
 */
Clock::Clock() :
	Primitive(0) {
}

core::AnyType::Handle Clock::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);
		return new core::ULong(lContext.getClock().getValue());
	schnaps_StackTraceEndM("core::AnyType::Handle basic::Clock::execute(unsigned int, core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive TokenT.
 *  \param  ioContext Execution context.
 *  \return Hierarchical typing tagging the data type returned.
 */
const std::string& Clock::getReturnType(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("ULong");
		return lType;
	schnaps_StackTraceEndM("const std::string& basic::Clock::getReturnType(core::ExecutionContext&) const");
}
