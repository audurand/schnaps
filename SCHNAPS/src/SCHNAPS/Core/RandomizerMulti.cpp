/*
 * RandomizerMulti.cpp
 *
 *  Created on: 2010-05-20
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

/*!
 *  \brief Construct a component formed of multiple randomizers.
 *  \param inRandomizers Bag containing the randomizers to use by the RandomizerMulti.
 */
RandomizerMulti::RandomizerMulti() :
	Component("RandomizerMulti"),
	mRandomizers(new Randomizer::Bag())
{}

void RandomizerMulti::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	// TODO: write current state?
	schnaps_StackTraceEndM("void SCHNAPS::Core::RandomizerMulti::writeContent(PACC::XML::Streamer&, bool) const");
}

void RandomizerMulti::init(System& ioSystem) {
	schnaps_StackTraceBeginM();
	unsigned int lThreads_Simulator = castHandleT<UInt>(ioSystem.getParameters()["threads.simulator"])->getValue();
	unsigned int lThreads_Generator = castHandleT<UInt>(ioSystem.getParameters()["threads.generator"])->getValue();
	unsigned int lThreads_Max;

	if (lThreads_Simulator >= lThreads_Generator) {
		lThreads_Max = lThreads_Simulator;
	} else {
		lThreads_Max = lThreads_Generator;
	}

	mRandomizers->clear();
	mRandomizers->reserve(lThreads_Max);
	for (unsigned int i = 0; i < lThreads_Max; ++i) {
		mRandomizers->push_back(new Randomizer());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::RandomizerMulti::init(SCHNAPS::Core::System&)");
}
