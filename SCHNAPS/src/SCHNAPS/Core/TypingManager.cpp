/*
 * TypingManager.cpp
 *
 *  Created on: 2009-10-08
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

using namespace core;

/*!
 *  \brief Construct typing manager component.
 */
TypingManager::TypingManager() :
		Component("TypingManager")
{
	// Root type
	mTypeMap["Any"];

	// Basic types
	mTypeMap["Atom"].push_back("Any");
	mTypeMap["Vector"].push_back("Any");
	mTypeMap["Void"].push_back("Any");

	// Atom types
	mTypeMap["Bool"].push_back("Atom");
	mTypeMap["Char"].push_back("Atom");
	mTypeMap["String"].push_back("Atom");
	mTypeMap["Number"].push_back("Atom");

	// Number types
	mTypeMap["Double"].push_back("Number");
	mTypeMap["Float"].push_back("Number");
	mTypeMap["Int"].push_back("Number");
	mTypeMap["Long"].push_back("Number");
	mTypeMap["UInt"].push_back("Number");
	mTypeMap["ULong"].push_back("Number");
}

TypingManager::TypingManager(const TypingManager& inOriginal) :
		Component("TypingManager")
{
	mTypeMap.clear();
	mTypeMap.insert(inOriginal.mTypeMap.begin(), inOriginal.mTypeMap.end());
}

void TypingManager::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		// TODO: write current state?
	schnaps_StackTraceEndM("void core::TypingManager::writeContent(PACC::XML::Streamer&, bool) const");
}
