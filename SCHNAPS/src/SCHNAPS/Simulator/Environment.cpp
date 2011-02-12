/*
 * Environment.cpp
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

Environment::Environment() :
	Individual("E")
{}

Environment::Environment(const Environment& inOriginal) :
		Individual(inOriginal.mID)
{
	mState = inOriginal.mState;
	mInitState = inOriginal.mInitState;
	mPopulation = inOriginal.mPopulation;
}

void Environment::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

		std::string lFile = inIter->getAttribute("file");
		PACC::XML::ConstIterator lChild = inIter->getFirstChild();

		// Read state
		if (lFile.empty()) {
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading environment\n");
#endif
			// Local description of state
			mInitState.readWithSystem(lChild, ioSystem);
			mState = mInitState;
			lChild++;
		} else {
#ifdef SCHNAPS_FULL_DEBUG
			printf("Opening file: %s\n", lFile.c_str());
#endif
			PACC::XML::Document* lDocument = new PACC::XML::Document();
			lDocument->parse(lFile);
			readWithSystem(lDocument->getFirstDataTag(), ioSystem);
			delete lDocument;
		}
	schnaps_StackTraceEndM("void simulator::Environment::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Environment::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	mState.write(ioStreamer, inIndent);
}
