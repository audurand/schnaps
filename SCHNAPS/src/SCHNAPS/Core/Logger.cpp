/*
 * Logger.cpp
 *
 *  Created on: 2010-05-19
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

Logger::Logger() :
		mEnabled(false)
{}

void Logger::open(const std::string& inLogFile) {
	schnaps_StackTraceBeginM();
		mOGZS = new ogzstream(inLogFile.c_str());
		mEnabled = true;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Logger::init(const std::string&)");
}

/*!
 * \brief Output log in log file.
 * \param inLog Log to output.
 */
void Logger::log(const std::string& inLog) {
	schnaps_StackTraceBeginM();
		if (mEnabled) {
			(*mOGZS) << inLog.c_str() << "\n";
		}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Logger::log(std::string)");
}

/*!
 * \brief Close logger file.
 *
 * This MUST be call before ending.
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
