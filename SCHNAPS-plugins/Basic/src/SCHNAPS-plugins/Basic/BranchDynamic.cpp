/*
 * BranchDynamic.cpp
 *
 *  Created on: 2010-01-12
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
 *  \brief Construct a new primitive that returns the result of a BranchDynamic.
 */
BranchDynamic::BranchDynamic() :
		Primitive()	// Unknown number of children
{}

BranchDynamic::BranchDynamic(const BranchDynamic& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

BranchDynamic& BranchDynamic::operator=(const BranchDynamic& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::BranchDynamic& core::BranchDynamic::operator=(const core::BranchDynamic&)");
}

core::AnyType::Handle BranchDynamic::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		// Retrieve children index to execute.
		core::UInt::Handle lN = core::castHandleT<core::UInt>(getArgument(inIndex, 0, ioContext));
		// Return executed children result.
		return getArgument(inIndex, lN->getValue(), ioContext);
	schnaps_StackTraceEndM("core::AnyType::Handle core::BranchDynamic::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& BranchDynamic::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		if (inN == 0) { // Must return children index to execute.
			const static std::string lType("core::UInt");
			return lType;
		} else if (inN == 1) {
			const static std::string lType("Any");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::BranchDynamic::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& BranchDynamic::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() < 2) {
			unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
			return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
		}

		std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));

		for (unsigned int i = 2; i < getNumberArguments(); i++) {
			lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
		}

		const static std::string lCommonType_Final(lCommonType);
		return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& core::BranchDynamic::getReturnType(unsigned int, core::ExecutionContext&) const");
}
