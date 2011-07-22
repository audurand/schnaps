/*
 * RandomizerMulti.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

/*!
 *  \brief Construct a component formed of multiple randomizers.
 *  \param inRandomizers Bag containing the randomizers to use by the RandomizerMulti.
 */
RandomizerMulti::RandomizerMulti() :
	Component("RandomizerMulti")
{
	mRandomizers.push_back(new Randomizer());
}

void RandomizerMulti::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	// TODO: write current state?
	schnaps_StackTraceEndM("void SCHNAPS::Core::RandomizerMulti::writeContent(PACC::XML::Streamer&, bool) const");
}

void RandomizerMulti::init(System& ioSystem) {
	schnaps_StackTraceBeginM();
	unsigned int lThreads_Simulator = castObjectT<const UInt&>(ioSystem.getParameters().getParameter("threads.simulator")).getValue();
	unsigned int lThreads_Generator = castObjectT<const UInt&>(ioSystem.getParameters().getParameter("threads.generator")).getValue();
	unsigned int lThreads_New;

	if (lThreads_Simulator >= lThreads_Generator) {
		lThreads_New = lThreads_Simulator;
	} else {
		lThreads_New = lThreads_Generator;
	}
	
	unsigned int lThreads_Old = mRandomizers.size();
	
	if (lThreads_New > lThreads_Old) {
		for (unsigned int i = lThreads_Old; i < lThreads_New; i++) {
			mRandomizers.push_back(new Randomizer());
		}
	} else {
		for (unsigned int i = lThreads_New; i > lThreads_Old; i--) {
			mRandomizers.pop_back();
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::RandomizerMulti::init(SCHNAPS::Core::System&)");
}
