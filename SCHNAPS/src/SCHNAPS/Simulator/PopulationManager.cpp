/*
 * PopulationManager.cpp
 *
 *  Created on: 2010-04-15
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

PopulationManager::PopulationManager() :
		mGenerator(new Generator())
{}

PopulationManager::PopulationManager(core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
		mGenerator(new Generator(inSystem, inClock, inEnvironment))
{}

void PopulationManager::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading population manager\n");
#endif

		inIter = inIter->getFirstChild();
		// Read generator
		mGenerator->read(inIter++);

		// Read population source
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "Population") {
			std::ostringstream lOSS;
			lOSS << "tag <Population> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				if (lChild->getValue() != "SubPopulation") {
					std::ostringstream lOSS;
					lOSS << "tag <SubPopulation> expected, but ";
					lOSS << "got tag <" << lChild->getValue() << "> instead!";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				if (lChild->getAttribute("time").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "time attribute expected!");
				}
				if (lChild->getAttribute("profile").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "profile attribute expected!");
				}
				if (lChild->getAttribute("size").empty()) {
					throw schnaps_IOExceptionNodeM(*lChild, "size attribute expected!");
				}

				this->insert(std::pair<unsigned long, Source>(core::str2uint(lChild->getAttribute("time")),
																Source(lChild->getAttribute("profile"), core::str2uint(lChild->getAttribute("size")))));
			}
		}
	schnaps_StackTraceEndM("void simulator::PopulationManager::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void PopulationManager::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		// Write generator
		mGenerator->write(ioStreamer, inIndent);

		// Write population source
		ioStreamer.openTag("Population");
		for (PopulationManager::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
			ioStreamer.openTag("SubPopulation");
			ioStreamer.insertAttribute("time", core::uint2str(lIt->first));
			ioStreamer.insertAttribute("profile", lIt->second.mProfile);
			ioStreamer.insertAttribute("size", core::uint2str(lIt->second.mSize));
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void simulator::PopulationManager::writeContent(PACC::XML::Streamer&, bool) const");
}

Individual::Bag::Handle simulator::PopulationManager::getIndividuals() {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mGenerator);
		unsigned long lCurrentTime = mGenerator->getClock().getValue();
		std::pair<PopulationManager::const_iterator, PopulationManager::const_iterator> lSources = this->equal_range(lCurrentTime);

		// If nothing to generate at current time
		if (lSources.first == lSources.second) {
			return NULL;
		}

		unsigned int lStartingIndex = mGenerator->getEnvironment().getPopulation().size();

		std::stringstream lPrefix;
		int lSubPopNumber = 0;

		Individual::Bag::Handle lIndividuals = new Individual::Bag();
		Individual::Bag::Handle lSubPop;
		for (PopulationManager::const_iterator lIt = lSources.first; lIt != lSources.second; lIt++) {
			// Assign new prefix to sub-population
			lPrefix.str("");
			lPrefix << "I_" << lCurrentTime << "_" << lSubPopNumber++;
			mPrefixes[lPrefix.str()] = lIt->second;

			// Generate sub-population
			lSubPop = mGenerator->generate(lIt->second.mProfile, lIt->second.mSize, lPrefix.str(), lStartingIndex);
			lStartingIndex += lSubPop->size();

			// Add new individuals to time step population
			lIndividuals->insert(lIndividuals->end(), lSubPop->begin(), lSubPop->end());
		}

		return lIndividuals;
	schnaps_StackTraceEndM("simulator::Individual::Bag::Handle simulator::PopulationManager::getIndividuals()");
}
