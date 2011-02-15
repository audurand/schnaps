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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 *  \brief Construct a new token terminal primitive that refers to the clock.
 */
Clock::Clock() :
	Primitive(0) {
}

Core::AnyType::Handle Clock::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
		return new Core::ULong(lContext.getClock().getValue());
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::Clock::execute(unsigned int, Core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive TokenT.
 *  \param  ioContext Execution context.
 *  \return Hierarchical typing tagging the data type returned.
 */
const std::string& Clock::getReturnType(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("ULong");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Clock::getReturnType(Core::ExecutionContext&) const");
}
