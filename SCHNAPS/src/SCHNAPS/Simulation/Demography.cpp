/*
 * Demography.cpp
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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

Demography::Demography(const Demography& inOriginal) {
	this->clear();
	this->insert(this->end(), inOriginal.begin(), inOriginal.end());
}

void Demography::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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

		PACC::XML::Document lDocument;
		std::string lFile, lLabel;

		lFile = inIter->getAttribute("file");
		if (lFile.empty()) {
#ifdef SCHNAPS_FULL_DEBUG
			printf("Reading %s\n", getName().c_str());
#endif
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

					lLabel = lChild->getAttribute("label");
#ifdef SCHNAPS_FULL_DEBUG
					printf("Reading variable: %s\n", lLabel.c_str());
#endif

					this->push_back(std::pair<std::string, SCHNAPS::Core::PrimitiveTree::Handle>(lLabel, new SCHNAPS::Core::PrimitiveTree()));
					this->back().second->readWithSystem(lChild->getFirstChild(), ioSystem);
				}
			}
		} else {
#ifdef SCHNAPS_FULL_DEBUG
		printf("Opening file: %s\n", lFile.c_str());
#endif
			lDocument.parse(lFile);
			this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Demography::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void Demography::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (unsigned int i = 0; i < this->size(); i++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", (*this)[i].first);
		(*this)[i].second->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
}
