/*
 * AnyType.cpp
 *
 *  Created on: 2009-11-19
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

AnyType& AnyType::operator=(const AnyType& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType& SCHNAPS::Core::AnyType::operator=(const SCHNAPS::Core::AnyType&)");
}

/*!
 *  \brief Read data from string.
 *  \param inStr Reference to the string to read.
 *  \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
void AnyType::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("readStr", "AnyType", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::AnyType::readStr(std::string&)");
}

/*!
 *  \brief Write data to string.
 *  \return The string.
 *  \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
std::string AnyType::writeStr() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("writeStr", "AnyType", getName());
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::AnyType::writeStr() const");
}

/*!
 * \brief Return a clone (deep copy).
 * \return Handle to a deep copy.
 * \throw InternalException if method undefined in subclasses.
 */
AnyType::Handle AnyType::clone() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("clone", "AnyType", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::AnyType::clone() const");
}
