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
 *  \file   SCHNAPS/Core/InternalException.cpp
 *  \brief  Implementation of class InternalException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 */

#include "SCHNAPS/Core.hpp"

using namespace core;


/*!
 *  \brief Construct a new InternalException using data given.
 *  \param inMessage    Message related to the exception.
 *  \param inFileName   File name where the exception is detected (throwed).
 *  \param inLineNumber Line number where the exception is detected (throwed).
 */
InternalException::InternalException(std::string inMessage,
                                     std::string inFileName,
                                     unsigned int inLineNumber) :
		TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* InternalException::getExceptionName() const throw()
{
	return "SIMULATOR_Simulator::InternalException";
}

