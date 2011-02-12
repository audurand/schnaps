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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

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
	schnaps_StackTraceEndM("core::RandomUniform& core::RandomUniform::operator=(const core::RandomUniform&)");
}

core::AnyType::Handle RandomUniform::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return new core::Double(ioContext.getRandomizer().rollUniform());
	schnaps_StackTraceEndM("core::AnyType::Handle core::RandomUniform::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& RandomUniform::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("core::Double");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::RandomUniform::getReturnType(unsigned int, core::ExecutionContext&) const");
}
