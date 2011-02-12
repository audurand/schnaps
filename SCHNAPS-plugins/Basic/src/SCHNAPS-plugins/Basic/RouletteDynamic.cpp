/*
 * RouletteDynamic.cpp
 *
 *  Created on: 2010-01-11
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
 *  \brief Construct a new primitive that returns the index of the chosen child according to its probability in a roulette.
 */
RouletteDynamic::RouletteDynamic() :
		Primitive()	// Unknown number of children
{}

RouletteDynamic::RouletteDynamic(const RouletteDynamic& inOriginal) :
		Primitive(inOriginal.getNumberArguments())
{}

RouletteDynamic& RouletteDynamic::operator=(const RouletteDynamic& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("core::RouletteDynamic& core::RouletteDynamic::operator=(const core::RouletteDynamic&)");
}

core::AnyType::Handle RouletteDynamic::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		double lTotal = 0;
		core::Double::Handle lArg;
		core::RouletteT<unsigned int> lRoulette;

		// Insert branches values
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lArg = core::castHandleT<core::Double>(getArgument(inIndex, i, ioContext));
			lRoulette.insert(i+1, lArg->getValue());
			lTotal += lArg->getValue();
		}
		if (lTotal < 1) {
			// Insert default (last branch) value
			lRoulette.insert(lRoulette.size()+1, 1-lTotal);
		}

		return new core::UInt(lRoulette.select(ioContext.getRandomizer()));
	schnaps_StackTraceEndM("core::AnyType::Handle RouletteDynamic::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& RouletteDynamic::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("core::Double");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::RouletteDynamic::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& RouletteDynamic::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::UInt");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::RouletteDynamic::getReturnType(unsigned int, core::ExecutionContext&) const");
}
