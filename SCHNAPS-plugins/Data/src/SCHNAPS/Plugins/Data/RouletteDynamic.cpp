/*
 * RouletteDynamic.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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
 * \brief Default constructor.
 */
RouletteDynamic::RouletteDynamic() :
	Primitive()	// unknown number of children
{}

/*!
 * \brief Construct a primitive that returns an unsigned int from a roulette with dynamic weights as a copy of an original.
 * \param inOriginal A const reference to the original primitive that returns an unsigned int from a roulette with dynamic weights.
 */
RouletteDynamic::RouletteDynamic(const RouletteDynamic& inOriginal) :
	Primitive(inOriginal.getNumberArguments())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
RouletteDynamic& RouletteDynamic::operator=(const RouletteDynamic& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Data::RouletteDynamic& SCHNAPS::Plugins::Data::RouletteDynamic::operator=(const SCHNAPS::Plugins::Data::RouletteDynamic&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle RouletteDynamic::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	double lTotal = 0;
	Core::Double::Handle lArg;
	Core::RouletteT<unsigned int> lRoulette;

	// insert branches values
	for (unsigned int i = 0; i < getNumberArguments(); i++) {
		lArg = Core::castHandleT<Core::Double>(getArgument(inIndex, i, ioContext));
		lRoulette.insert(i+1, lArg->getValue());
		lTotal += lArg->getValue();
	}
	if (lTotal < 1) {
		// insert default (last branch) value
		lRoulette.insert(lRoulette.size()+1, 1-lTotal);
	}

	return new Core::UInt(lRoulette.select(ioContext.getRandomizer()));
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle RouletteDynamic::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& RouletteDynamic::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Core::Double");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RouletteDynamic::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& RouletteDynamic::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::UInt");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RouletteDynamic::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
