/*
 * TargetedException.cpp
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
 * \brief Construct a new exception using data given.
 * \param inMessage     Message related to the exception.
 * \param inFileName    File name where the exception is detected (throwed).
 * \param inLineNumber  Line number where the exception is detected (throwed).
 */
TargetedException::TargetedException(std::string inMessage,
                                     std::string inFileName,
                                     unsigned int inLineNumber) :
	Exception(inMessage),
	mFileName(inFileName),
	mLineNumber(inLineNumber)
{}

/*!
 * \brief Explain the exception throwed in the stream given.
 * \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void TargetedException::explain(std::ostream& ioES) throw() {
	ioES << "From file '" << mFileName << "', line " << mLineNumber << " :";
	ioES << std::endl << std::flush;
	Exception::explain(ioES);
}
