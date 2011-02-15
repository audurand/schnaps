/*
 * IfThenElse.cpp
 *
 *  Created on: 2009-03-10
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse() :
		Primitive(3)
{}

/*!
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse(const IfThenElse& inOriginal) :
		Primitive(3)
{}

IfThenElse& IfThenElse::operator=(const IfThenElse& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::IfThenElse& SCHNAPS::Plugins::Control::IfThenElse::operator=(const SCHNAPS::Plugins::Control::IfThenElse&)");
}

SCHNAPS::Core::AnyType::Handle IfThenElse::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::Bool::Handle lArg1 = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(getArgument(inIndex, 0, ioContext));	// If
		if (lArg1->getValue()) {
			SCHNAPS::Core::AnyType::Handle lArg2 = getArgument(inIndex, 1, ioContext);	// Then
			return lArg2;
		}
		SCHNAPS::Core::AnyType::Handle lArg3 = getArgument(inIndex, 2, ioContext);	// Else
		return lArg3;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::IfThenElse::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& IfThenElse::getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<3);
		if (inN == 0) {
			const static std::string lType("SCHNAPS::Core::Bool");
			return lType;
		}
		else if (inN == 1) {
			const static std::string lType("Any");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::IfThenElse::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& IfThenElse::getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));
		return lCommonType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::IfThenElse::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
