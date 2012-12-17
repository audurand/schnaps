/*
 * Population.cpp
 *
 *  Created on: 2009-02-26
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

/*!
 *  \brief Construct a new population.
 */
Population::Population(const Population& inOriginal) {
	insert(begin(), inOriginal.begin(), inOriginal.end());
}

/*!
 *  \brief Reading the environment.
 */
void Population::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
		std::string lSource = inIter->getAttribute("source");
		std::string lFile = inIter->getAttribute("file");

		PACC::XML::Document lDocument;
		igzstream lIGZS;

		// Read population
		if (lSource == "index") {
			if (lFile.empty()) {
				// Local index
				readFromIndexWithSystem(inIter->getFirstChild(), ioSystem);
			} else {
				lIGZS.open(lFile.c_str(), std::ios::in);
				lDocument.parse(lIGZS);
				this->readFromIndexWithSystem(lDocument.getFirstDataTag(), ioSystem);
				lIGZS.close();
			}
		} else if ((lSource.empty()) || (lSource == "file")) {
			Individual::Handle lIndividual;

			if (lFile.empty()) {
				// Local description of population
				if (inIter->getAttribute("size").empty() == false) {
					unsigned int lSize;
					std::istringstream lISS(inIter->getAttribute("size"));
					lISS >> lSize;
					this->reserve(lSize);
				}
				for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
					this->push_back(new Individual(""));
					this->back()->readWithSystem(lChild, ioSystem);
				}
			} else {
				// Remote description of population
				lIGZS.open(lFile.c_str(), std::ios::in);
				lDocument.parse(lIGZS);
				this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
				lIGZS.close();
			}
		} else {
			throw schnaps_IOExceptionNodeM(*inIter, "unknown source of population!");
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Population::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void Population::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	PACC::XML::Streamer *lStreamer;
	ogzstream lOGZS;
	std::stringstream lFile;

	// Always with index
	ioStreamer.openTag("Index");
	for (unsigned int i = 0; i < this->size()/SUB_POPULATION_SIZE; i++) {
		lFile.str(""); // Clear string
		lFile << "./Population_" << i << ".gz";

		// Print sub population
		lOGZS.open(lFile.str().c_str(), std::ios::out);
		lStreamer = new PACC::XML::Streamer(lOGZS);
		this->writeSubPopulation(*lStreamer, i*SUB_POPULATION_SIZE, (i+1)*SUB_POPULATION_SIZE, inIndent);
		delete lStreamer;
		lOGZS.close();

		// Add sub population entry to index
		ioStreamer.openTag("File");
		ioStreamer.insertAttribute("file", lFile.str());
		ioStreamer.closeTag();
	}
	if (this->size() % SUB_POPULATION_SIZE > 0) {
		lFile.str(""); // Clear string
		lFile << "./Population_" << this->size()/SUB_POPULATION_SIZE << ".gz";

		// Print sub population
		lOGZS.open(lFile.str().c_str(), std::ios::out);
		lStreamer = new PACC::XML::Streamer(lOGZS);
		this->writeSubPopulation(*lStreamer, (this->size()/SUB_POPULATION_SIZE)*SUB_POPULATION_SIZE, this->size(), inIndent);
		delete lStreamer;
		lOGZS.close();

		// Add sub population entry to index
		ioStreamer.openTag("File");
		ioStreamer.insertAttribute("file", lFile.str());
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
}

/*!
 *  \brief Reading a population from an index file.
 */
void Population::readFromIndexWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		inIter = inIter->getFirstChild();

		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Index") {
			std::ostringstream lOSS;
			lOSS << "tag <Index> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		int count = 0;
		PACC::XML::Document lDocument;
		igzstream lIGZS;

		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "File") {
					std::ostringstream lOSS;
					lOSS << "tag <File> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
			}
			// Read population from file
			std::string lFile = lChild->getAttribute("file");
			if (lFile.empty() == true) {
				throw schnaps_IOExceptionNodeM(*lChild, "no file attribute specified for actual population load from index!");
			}

#ifndef SCHNAPS_NDEBUG
			printf("Reading %d\n", count);
#endif
			count++;

			lIGZS.clear();
			lIGZS.open(lFile.c_str(), std::ios::in);
			lDocument.parse(lIGZS);
			readWithSystem(lDocument.getFirstDataTag(), ioSystem);
			lIGZS.close();
		}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Population::readFromIndexWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void Population::writeSubPopulation(PACC::XML::Streamer& ioStreamer, unsigned int inFrom, unsigned int inTo, bool inIndent) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inTo <= size());
		ioStreamer.openTag(getName());
		ioStreamer.insertAttribute("size", inTo-inFrom);
		for (unsigned int i = inFrom; i < inTo; i++) {
			(*this)[i]->write(ioStreamer, inIndent);
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Population::writeSubPopulation(PACC::XML::Streamer&, unsigned int, unsigned int, bool) const");
}

bool Population::addIndividuals(Individual::Bag::Handle lNewIndividuals) {
	schnaps_StackTraceBeginM();
		if (lNewIndividuals != NULL) {
			this->insert(this->end(), lNewIndividuals->begin(), lNewIndividuals->end());
			return true;
		}
		return false;
	schnaps_StackTraceEndM("bool SCHNAPS::Simulation::Population::addIndividuals(SCHNAPS::Simulation::Individual::Bag::Handle lNewIndividuals)");
}
