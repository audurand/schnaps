/*
 * And.cpp
 *
 *  Created on: 2009-02-19
 *  Updated on: 2010-12-22
 *      Author: Audrey Durand
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
 *  \brief Construct a new boolean AND primitive.
 */
And::And() :
		Primitive()	// unknown number of children
{}

/*!
 *  \brief Construct a new boolean AND primitive.
 */
And::And(const And& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

And& And::operator=(const And& inOriginal) {
	schnaps_StackTraceBeginM();
		setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("Core::And& SCHNAPS::Plugins::Operators::And::operator=(const Core::And&)");
}

Core::AnyType::Handle And::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(getNumberArguments() > 0);
		bool lResult = true;
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lResult = lResult && Core::castHandleT<Core::Bool>(getArgument(inIndex, i, ioContext))->getValue();
		}
		return new Core::Bool(lResult);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::And::execute(unsigned int, Core::ExecutionContext&)");
}

const std::string& And::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::And::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& And::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const
{
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::And::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
