/*
 * Or.cpp
 *
 *  Created on: 2009-02-26
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 *  \brief Construct a new boolean OR primitive.
 */
Or::Or() :
		Primitive()	// unknown number of children
{}

/*!
 *  \brief Construct a new boolean OR primitive.
 */
Or::Or(const Or& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

Or& Or::operator=(const Or& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("Core::Or& SCHNAPS::Plugins::Operators::Or::operator=(const Core::Or&)");
}

Core::AnyType::Handle Or::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 0);
		bool lResult = false;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResult = lResult || Core::castHandleT<Core::Bool>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new Core::Bool(lResult);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::Or::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& Or::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Or::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& Or::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Or::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
