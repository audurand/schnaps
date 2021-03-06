/*
 * LoggerMulti.cpp
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
 * \brief Construct a component formed of multiple loggers.
 */
LoggerMulti::LoggerMulti() :
	Component("LoggerMulti"),
	mLoggers(new Logger::Bag())
{}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void LoggerMulti::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	// TODO: write current state?
	schnaps_StackTraceEndM("void SCHNAPS::Core::LoggerMulti::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Initialize this component.
 * \param ioSystem A reference to the system.
 */
void LoggerMulti::init(System& ioSystem) {
	schnaps_StackTraceBeginM();
	unsigned int lThreads_Simulator = castObjectT<const UInt&>(ioSystem.getParameters().getParameter("threads.simulator")).getValue();
	unsigned int lThreads_Generator = castObjectT<const UInt&>(ioSystem.getParameters().getParameter("threads.generator")).getValue();
	unsigned int lThreads_Max;

	if (lThreads_Simulator >= lThreads_Generator) {
		lThreads_Max = lThreads_Simulator;
	} else {
		lThreads_Max = lThreads_Generator;
	}

	mLoggers->clear();
	mLoggers->reserve(lThreads_Max);
	for (unsigned int i = 0; i < lThreads_Max; ++i) {
		mLoggers->push_back(new Logger());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::LoggerMulti::init(SCHNAPS::Core::System&)");
}
