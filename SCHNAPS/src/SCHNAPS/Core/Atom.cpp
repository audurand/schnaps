/*
 * Atom.cpp
 *
 *  Created on: 2009-02-17
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

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;

void Atom::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		if (inIter) {
			if (inIter->getType() != PACC::XML::eData) {
				throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
			}
			if (inIter->getValue() != getName()) {
				std::ostringstream lOSS;
				lOSS << "tag <" << getName() << "> expected, but ";
				lOSS << "got tag <" << inIter->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}
			if (!inIter->getAttribute("value").empty()) {
				readStr(inIter->getAttribute("value"));
			}
		}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Atom::read(PACC::XML::ConstIterator inIter)");
}

void Atom::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
		this->read(inIter);
	schnaps_StackTraceEndM("void SCHNAPS::Core::Atom::read(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System&)");
}
