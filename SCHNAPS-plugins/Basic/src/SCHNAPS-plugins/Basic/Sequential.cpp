/*
 * Sequential.cpp
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

Sequential::Sequential() :
		Primitive()	// Unknown number of children
{}

Sequential::Sequential(const Sequential& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

Sequential& Sequential::operator=(const Sequential& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("core::Sequential& core::Sequential::operator=(const core::Sequential&)");
}

core::AnyType::Handle Sequential::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() == 0) {
			return NULL;
		}

		for (unsigned int i = 0; i < getNumberArguments()-1; i++) {
			getArgument(inIndex, i, ioContext);
		}
		return getArgument(inIndex, getNumberArguments()-1, ioContext);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Sequential::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& Sequential::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Sequential::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& Sequential::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() == 0) {
			const static std::string lType("Void");
			return lType;
		}
		return getArgType(inIndex, getNumberArguments()-1, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Sequential::getReturnType(unsigned int, core::ExecutionContext&) const");
}
