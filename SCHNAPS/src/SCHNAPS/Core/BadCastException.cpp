/*
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

/*!
 *  \file   SCHNAPS/Core/BadCastException.cpp
 *  \brief  Implementation of class BadCastException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 */

#include "SCHNAPS/Core.hpp"

using namespace core;

/*!
 *  \brief Construct a new BadCastException using data given.
 *  \param inMessage    Message related to the exception.
 *  \param inFileName   File name where the exception is detected (throwed).
 *  \param inLineNumber Line number where the exception is detected (throwed).
 */
BadCastException::BadCastException(std::string inMessage,
                                   std::string inFileName,
                                   unsigned int   inLineNumber) :
		TargetedException(inMessage,inFileName,inLineNumber)
{ }

const char* BadCastException::getExceptionName() const throw() {
	return "SIMULATOR_Simulator::BadCastException";
}
