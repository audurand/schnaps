/*
 * IsBetween.cpp
 *
 *  Created on: 2010-08-19
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
 *  \brief Construct a new IsBetween primitive.
 */
IsBetween::IsBetween() :
		Primitive(3)
{}

/*!
 *  \brief Construct a new IsBetween primitive.
 */
IsBetween::IsBetween(const IsBetween& inOriginal) :
		Primitive(3)
{}

IsBetween& IsBetween::operator=(const IsBetween& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("Core::IsBetween& SCHNAPS::Plugins::Operators::IsBetween::operator=(const Core::IsBetween&)");
}

Core::AnyType::Handle IsBetween::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext)); // Value to compare
		Core::Number::Handle lArg2 = Core::castHandleT<Core::Number>(getArgument(inIndex, 1, ioContext)); // Min
		Core::Number::Handle lArg3 = Core::castHandleT<Core::Number>(getArgument(inIndex, 2, ioContext)); // Max
		return new Core::Bool((lArg2->isLess(*lArg1) || lArg1->isEqual(*lArg2)) && (lArg1->isLess(*lArg3) || lArg1->isEqual(*lArg3)));
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsBetween::execute(unsigned int, Core::ExecutionContext&)");
}

const std::string& IsBetween::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<3);
		if (inN == 0) {
			const static std::string lType("Core::Number");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetween::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& IsBetween::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetween::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
