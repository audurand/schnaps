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
 *  \file   SCHNAPS/Core/IOException.cpp
 *  \brief  Implementation of class IOException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15 $
 *  $Date: 2007/08/10 20:32:22 $
 */

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;


/*!
 *  \brief Construct the IOException using given values.
 *  \param inMessage     Message explaining the exception.
 *  \param inFileName    Name of the file where the error state had been detected.
 *  \param inLineNumber  Line number in the filename.
 */
IOException::IOException(std::string  inMessage,
                         std::string  inFileName,
                         unsigned int inLineNumber) :
		TargetedException(inMessage,inFileName,inLineNumber)
{ }


/*!
 *  \brief Construct the IOException using given values.
 *  \param inIter        XML node in which the exception occured.
 *  \param inMessage     Message explaining the exception.
 *  \param inFileName    Name of the file where the error state had been detected.
 *  \param inLineNumber  Line number in the filename.
 */
IOException::IOException(const PACC::XML::Node& inIter,
                         std::string inMessage,
                         std::string inFileName,
                         unsigned int inLineNumber) :
		TargetedException("",inFileName,inLineNumber)
{
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << "Error with XML node '" << inIter.getValue();
	lOSS << "': " << inMessage;
	setMessage(lOSS.str());
	schnaps_StackTraceEndM("IOException::IOException(const PACC::XML::Node& inIter, string inMessage, string inFileName, unsigned int inLineNumber)");
}


/*!
 *  \brief  Get the name of the exception.
 *  \return Name of the exception.
 */
const char* IOException::getExceptionName() const throw()
{
	return "Simulator::IOException";
}

