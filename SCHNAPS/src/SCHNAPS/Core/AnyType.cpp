/*
 * AnyType.cpp
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
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
AnyType& AnyType::operator=(const AnyType& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType& SCHNAPS::Core::AnyType::operator=(const SCHNAPS::Core::AnyType&)");
}

/*!
 *  \brief Read data from string.
 *  \param inStr A const reference to the string to read from.
 *  \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
void AnyType::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("readStr", "AnyType", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::AnyType::readStr(std::string&)");
}

/*!
 *  \brief Return data as string.
 *  \return Data as string.
 *  \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
std::string AnyType::writeStr() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("writeStr", "AnyType", getName());
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::AnyType::writeStr() const");
}

/*!
 * \brief Return a handle to a clone (deep copy).
 * \return A handle to a clone (deep copy).
 * \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
AnyType::Handle AnyType::clone() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("clone", "AnyType", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::AnyType::clone() const");
}
