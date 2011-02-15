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
 *  \file   SCHNAPS/Core/Exception.cpp
 *  \brief  Implementation of class Exception.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 */

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;


/*!
 *  \brief Construct a LSD::Exception with the message given.
 *  \param inMessage Message describing the exception.
 */
Exception::Exception(std::string inMessage) :
		mMessage(inMessage)
{ }


/*!
 *  \brief Explain the exception throwed in the stream given.
 *  \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void Exception::explain(std::ostream& ioES) throw()
{
	std::string lMessage = std::string("'")+mMessage+std::string("'");

#ifndef SCHNAPS_NDEBUG
	if(!mStackTrace.empty()) {
		lMessage += "\nIn:";
		for (unsigned int i=0; i<mStackTrace.size(); i++) {
			lMessage += std::string("\n")+uint2str(i)+std::string(": ")+mStackTrace[i].mName+
			            std::string("\t(")+mStackTrace[i].mFilename+std::string(", line ")+
			            int2str(mStackTrace[i].mLineNumber)+std::string(")");
		}
	}
#endif // SCHNAPS_NDEBUG

	wrapString(lMessage);
	ioES << lMessage << std::endl << std::flush;
}


/*!
 *  \brief Terminate execution of actual program by displaying exception message et aborting.
 *  \param ioES C++ output stream to display the error.
 */
void Exception::terminate(std::ostream& ioES) throw()
{
	ioES << getExceptionName() << ":" << std::endl << std::flush;
	explain(ioES);
	ioES << std::endl << std::flush;
	abort();
}

/*!
 *  \brief  Return a copy of the error message.
 *  \return Error message.
 */
const char* Exception::what() const throw()
{
	return mMessage.c_str();
}


