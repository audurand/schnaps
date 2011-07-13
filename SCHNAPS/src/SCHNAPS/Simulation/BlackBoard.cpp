/*
 * BlackBoard.cpp
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
 * \brief Construct a blackboard as a copy of an original.
 * \param inOriginal A const reference to the original blackboard.
 */
BlackBoard::BlackBoard(const BlackBoard& inOriginal) {
	this->clear();
	for (BlackBoard::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
		(*this)[lIt->first] = lIt->second;
	}
}
