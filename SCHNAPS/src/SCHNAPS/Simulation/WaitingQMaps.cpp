/*
 * WaitingQMaps.cpp
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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Construct a class of waiting FIFOs for environment and individuals as a copy of an original.
 * \param inOriginal The original class of waiting FIFOs for environment and individuals.
 */
WaitingQMaps::WaitingQMaps(const WaitingQMaps& inOriginal) {
	mEnvironment.clear();
	for (std::map<unsigned int, std::queue<Process::Handle> >::const_iterator lIt = inOriginal.mEnvironment.begin(); lIt != inOriginal.mEnvironment.end(); lIt++) {
		mEnvironment[lIt->first] = lIt->second;
	}

	mIndividuals.clear();
	mIndividuals.resize(inOriginal.mIndividuals.size());
	for (unsigned int i = 0; i < mIndividuals.size(); i++) {
		for (std::map<unsigned int, std::queue<Process::Handle> >::const_iterator lIt = inOriginal.mIndividuals[i].begin(); lIt != inOriginal.mIndividuals[i].end(); lIt++) {
			mIndividuals[i][lIt->first] = lIt->second;
		}
	}
}
