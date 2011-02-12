/*
 * Clock.cpp
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
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

Clock::Clock() :
	mValue(0),
	mStop(NULL)
{}

Clock::Clock(const Clock& inOriginal) :
	mValue(inOriginal.getValue()),
	mStop(inOriginal.getStop())
{}

void Clock::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading clock\n");
#endif

		// Read stop condition tree
		mStop = new core::PrimitiveTree();
		mStop->readWithSystem(inIter->getFirstChild(), ioSystem);
	schnaps_StackTraceEndM("void simulator::Clock::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Clock::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	// Write stop condition tree
	mStop->write(ioStreamer, inIndent);
}
