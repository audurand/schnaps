/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2009 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
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
