/*
 * RandomUniform.cpp
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
RandomUniform::RandomUniform() :
	Primitive(0)
{}

/*!
 * \brief Constructor a random number primitive as a copy of an original.
 * \param inOriginal A const reference to the orginial random number primitive.
 */
RandomUniform::RandomUniform(const RandomUniform& inOriginal) :
	Primitive(0)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
RandomUniform& RandomUniform::operator=(const RandomUniform& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Data::RandomUniform& SCHNAPS::Plugins::Data::RandomUniform::operator=(const SCHNAPS::Plugins::Data::RandomUniform&)");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle RandomUniform::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	return new Core::Double(ioContext.getRandomizer().rollUniform());
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Data::RandomUniform::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& RandomUniform::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Double");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RandomUniform::getReturnType(unsigned int, CSCHNAPS::ore::ExecutionContext&) const");
}
