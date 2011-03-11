/*
 * Logger.cpp
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
 * \brief Construct a new logger.
 */
Logger::Logger() :
	mEnabled(false)
{}

/*!
 * \brief Open logger file and enable logger.
 * \param inFileName A const reference to the logger file name.
 */
void Logger::open(const std::string& inFileName) {
	schnaps_StackTraceBeginM();
	mOGZS = new ogzstream(inFileName.c_str());
	mEnabled = true;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Logger::init(const std::string&)");
}

/*!
 * \brief Log message to file.
 * \param inLog A const refernce to the message to log.
 */
void Logger::log(const std::string& inMessage) {
	schnaps_StackTraceBeginM();
	if (mEnabled) {
		(*mOGZS) << inMessage.c_str() << "\n";
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Logger::log(std::string)");
}

/*!
 * \brief Close logger file.
 * \note This MUST be call before ending.
 */
void Logger::close() {
	schnaps_StackTraceBeginM();
	if (mEnabled) {
		mOGZS->close();
		mEnabled = false;
		delete mOGZS;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Logger::close()");
}
