/*
 * RandomUniform.cpp
 *
 *  Created on: 2009-02-12
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
 *  \brief Construct a new primitive that returns a random number from uniform distribution [0,1].
 */
RandomUniform::RandomUniform() :
		Primitive(0)
{}

RandomUniform::RandomUniform(const RandomUniform& inOriginal) :
		Primitive(0)
{}

RandomUniform& RandomUniform::operator=(const RandomUniform& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("Core::RandomUniform& SCHNAPS::Plugins::Data::RandomUniform::operator=(const Core::RandomUniform&)");
}

Core::AnyType::Handle RandomUniform::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return new Core::Double(ioContext.getRandomizer().rollUniform());
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::RandomUniform::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& RandomUniform::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::Double");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RandomUniform::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
