/*
 * IndividualModel.cpp
 *
 *  Created on: 2010-04-14
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

IndividualModel::IndividualModel(const IndividualModel& inOriginal) {
	for (IndividualModel::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
		this->insert((*lIt).c_str());
	}
}

/*!
 *  \brief Reading the state.
 */
void IndividualModel::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" + getName() + "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		this->clear();
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

				this->insert(lChild->getAttribute("label"));
			}
		}
	schnaps_StackTraceEndM("void simulator::IndividualModel::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

/*!
 *  \brief Write the state.
 *  \param ioStreamer XML streamer to write the state into.
 *  \param inIndent Whether XML output should be indented.
 */
void IndividualModel::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (IndividualModel::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", (*lIt));
		ioStreamer.closeTag();
	}
}
