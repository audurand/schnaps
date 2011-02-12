/*
 * State.cpp
 *
 *  Created on: 2009-03-12
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
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

State::State(const State& inOriginal) {
	for (State::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
		(*this)[lIt->first] = lIt->second;
	}
}

State& State::operator=(const State& inOriginal) {
	schnaps_StackTraceBeginM();
		this->clear();
		for (State::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
			(*this)[lIt->first] = core::castHandleT<core::Atom>(lIt->second->clone());
		}

		return *this;
	schnaps_StackTraceEndM("simulator::State& simulator::State::operator=(const simulator::State&)");
}

void State::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		core::Atom::Alloc::Handle lAlloc;

		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" + getName() + "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "Variable") {
					std::ostringstream lOSS;
					lOSS << "tag <Variable> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				if (lChild->getAttribute("label").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "label attribute expected!");
				}
				if (lChild->getAttribute("type").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "type attribute expected!");
				}
				if (lChild->getAttribute("value").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "value attribute expected!");
				}

				lAlloc = core::castHandleT<core::Atom::Alloc>(ioSystem.getFactory().getAllocator(lChild->getAttribute("type")));
				if (lAlloc == NULL) {
					throw schnaps_InternalExceptionM("Could not find allocator '" + lChild->getAttribute("type") + "' in the factory!");
				}

				(*this)[lChild->getAttribute("label")] = core::castHandleT<core::Atom>(lAlloc->allocate());
				(*this)[lChild->getAttribute("label")]->readStr(lChild->getAttribute("value"));
			}
		}
	schnaps_StackTraceEndM("void simulator::State::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void State::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (State::const_iterator lIt = begin(); lIt != end(); lIt++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", lIt->first);
		ioStreamer.insertAttribute("type", lIt->second->getName());
		ioStreamer.insertAttribute("value", lIt->second->writeStr());
		ioStreamer.closeTag();
	}
}

void State::print(std::ostream& ioStream, const std::vector<std::string> inVariables) const {
	State::const_iterator lStateIt;

	for (unsigned int i = 0; i < inVariables.size(); i++) {
		lStateIt = this->find(inVariables[i]);

		if (lStateIt == this->end()) {
			throw schnaps_InternalExceptionM("There is no variable '" + inVariables[i] + "' in the state!");
		}

		if (lStateIt->second != NULL) {
			ioStream << "," << lStateIt->second->writeStr();
		} else {
			ioStream << ", NULL VARIABLE";
		}
	}
}
