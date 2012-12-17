/*
 * Clock.cpp
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
 * \brief Default constructor.
 */
Clock::Clock() :
	mValue(0),
	mStop(NULL),
	mUnits(eOther)
{}

/*!
 * \brief Construct a clock as a copy of an original.
 * \param inOriginal A const reference to the original clock.
 */
Clock::Clock(const Clock& inOriginal) :
	mValue(inOriginal.mValue),
	mStop(inOriginal.mStop),
	mUnits(inOriginal.mUnits)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Clock::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// read units
	if (inIter->getAttribute("units").empty()) {
		mUnits = eOther;
	} else {
		if (inIter->getAttribute("units") == "year") {
			mUnits = eYear;
		} else if (inIter->getAttribute("units") == "month") {
			mUnits = eMonth;
		} else if (inIter->getAttribute("units") == "week") {
                        mUnits = eWeek;
		} else if (inIter->getAttribute("units") == "day") {
			mUnits = eDay;
		} else {
			mUnits = eOther;
		}
	}

	// read stop condition tree
	mStop = new Core::PrimitiveTree();
	mStop->readWithSystem(inIter->getFirstChild(), ioSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Clock::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Clock::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	// write units
	switch (mUnits) {
		case eYear:
			ioStreamer.insertAttribute("units", "year");
			break;
		case eMonth:
			ioStreamer.insertAttribute("units", "month");
			break;
		case eWeek:
                        ioStreamer.insertAttribute("units", "week");
                        break;
		case eDay:
			ioStreamer.insertAttribute("units", "day");
			break;
		default:
			ioStreamer.insertAttribute("units", "other");
			break;
			
	}
	
	// write stop condition tree
	mStop->write(ioStreamer, inIndent);
}
