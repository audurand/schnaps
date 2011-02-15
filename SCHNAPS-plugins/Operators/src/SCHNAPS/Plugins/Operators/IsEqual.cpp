/*
 * Equal.cpp
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 *  \brief Construct a new IsEqual primitive.
 */
IsEqual::IsEqual() :
		Primitive(2)
{}

/*!
 *  \brief Construct a new IsEqual primitive.
 */
IsEqual::IsEqual(const IsEqual& inOriginal) :
		Primitive(2)
{}

IsEqual& IsEqual::operator=(const IsEqual& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("Core::IsEqual& SCHNAPS::Plugins::Operators::IsEqual::operator=(const Core::IsEqual&)");
}

Core::AnyType::Handle IsEqual::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Core::Atom::Handle lArg1 = Core::castHandleT<Core::Atom>(getArgument(inIndex, 0, ioContext));
		Core::Atom::Handle lArg2 = Core::castHandleT<Core::Atom>(getArgument(inIndex, 1, ioContext));
		return new Core::Bool(lArg1->isEqual(*lArg2));
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsEqual::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& IsEqual::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<2);
		if (inN == 0) {
			const static std::string lType("Core::Atom");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsEqual::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& IsEqual::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Core::Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsEqual::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
