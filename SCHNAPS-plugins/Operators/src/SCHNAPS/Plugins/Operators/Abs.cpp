/*
 * Abs.cpp
 *
 *  Created on: 2009-02-24
 *  Updated on: 2010-04-06
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
 *  \brief Construct a new absolute value primitive.
 */
Abs::Abs() :
		Primitive(1)
{}

Abs::Abs(const Abs& inOriginal) :
		Primitive(1)
{}

Abs& Abs::operator=(const Abs& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("Core::Abs& SCHNAPS::Plugins::Operators::Abs::operator=(const Core::Abs&)");
}

Core::AnyType::Handle Abs::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) {
	schnaps_StackTraceBeginM();
		Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
		lArg1->abs();
		return lArg1;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::Abs::execute(unsigned int, Core::ExecutionContext&)");
}

const std::string& Abs::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
		const static std::string lType("Number");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Abs::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& Abs::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Abs::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
