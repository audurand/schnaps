/*
 * AssertException.cpp
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
 * \brief Construct a new ValidationException object.
 * \param inMessage Message describing the exception.
 */
ValidationException::ValidationException(std::string inMessage) :
	Exception(inMessage)
{}

/*!
 * \brief Return a const pointer the actual name (char) of the exception.
 * \return A const pointer to the actual name (char) of the exception.
 */
const char* ValidationException::getExceptionName() const throw() {
	return "SCHNAPS::Core::ValidationException";
}
