/*
 * Individual.cpp
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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

Individual::Individual(std::string inID) :
		mID(inID)
{}

Individual::Individual(const Individual& inOriginal) :
		mID(inOriginal.mID.c_str()),
		mState(inOriginal.mState)
{}

void Individual::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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
		if (inIter->getAttribute("id").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "individual ID expected!");
		}

		mID = inIter->getAttribute("id");
		mState.readWithSystem(inIter->getFirstChild(), ioSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Individual::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void Individual::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("id", mID);
	mState.write(ioStreamer, inIndent);
}

void Individual::print(std::ostream& ioStream, const std::vector<std::string> inVariables) const {
	ioStream << mID;
	mState.print(ioStream, inVariables);
	ioStream << std::endl;
}
