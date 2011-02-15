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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

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
	schnaps_StackTraceEndM("Core::RouletteDynamic& SCHNAPS::Plugins::Data::RouletteDynamic::operator=(const Core::RouletteDynamic&)");
}

Core::AnyType::Handle RouletteDynamic::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		double lTotal = 0;
		Core::Double::Handle lArg;
		Core::RouletteT<unsigned int> lRoulette;

		// Insert branches values
		for (unsigned int i = 0; i < getNumberArguments(); i++) {
			lArg = Core::castHandleT<Core::Double>(getArgument(inIndex, i, ioContext));
			lRoulette.insert(i+1, lArg->getValue());
			lTotal += lArg->getValue();
		}
		if (lTotal < 1) {
			// Insert default (last branch) value
			lRoulette.insert(lRoulette.size()+1, 1-lTotal);
		}

		return new Core::UInt(lRoulette.select(ioContext.getRandomizer()));
	schnaps_StackTraceEndM("Core::AnyType::Handle RouletteDynamic::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& RouletteDynamic::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < getNumberArguments());
		const static std::string lType("Core::Double");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RouletteDynamic::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& RouletteDynamic::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::UInt");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RouletteDynamic::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
