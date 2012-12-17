/*
 * ObjectException.cpp
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

#ifdef SCHNAPS_HAVE_RTTI
#include <typeinfo>
#endif // SCHNAPS_HAVE_RTTI

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Construct the exception using given values.
 * \param inObject     Reference to the object related to the exception.
 * \param inMessage    Message explaining the exception.
 * \param inFileName   Name of the file where the error state had been detected.
 * \param inLineNumber Line number in the filename.
 */
ObjectException::ObjectException(const Object&   inObject,
                                 std::string  inMessage,
                                 std::string  inFileName,
                                 unsigned int    inLineNumber) :
	TargetedException(inMessage,inFileName,inLineNumber)
{
	schnaps_StackTraceBeginM();
	setObjectState(inObject);
	schnaps_StackTraceEndM("ObjectException::ObjectException(const Object& inObject, string inMessage, string inFileName, unsigned int inLineNumber)");
}

/*!
 * \brief Explain the exception throwed in the stream given.
 * \param ioES Output C++ stream where the explanation about the exception is inserted.
 */
void ObjectException::explain(std::ostream& ioES) throw() {
	TargetedException::explain(ioES);
	ioES << "Object name: " << mObjectName << std::endl;
	ioES << "Object RTTI name: " << mObjectTypeName << std::endl;
	ioES << "Object reference counter value: " << mObjectRefCounter << std::endl;
}

/*!
 * \brief Return a const pointer the actual name (char) of the exception.
 * \return A const pointer to the actual name (char) of the exception.
 */
const char* ObjectException::getExceptionName() const throw() {
	return "Simulator::ObjectException";
}

/*!
 * \brief Set values of the ObjectException using given object reference.
 * \param inObject A const reference to object related to the exception.
 */
void ObjectException::setObjectState(const Object& inObject) {
	schnaps_StackTraceBeginM();
	mObjectRefCounter = inObject.getRefCounter();
	mObjectName = inObject.getName();
#ifdef SCHNAPS_HAVE_RTTI
	try {
		mObjectTypeName = typeid(inObject).name();
	} catch(...) {
		mObjectTypeName = "<Failed>";
	}
#else // SCHNAPS_HAVE_RTTI
	mObjectTypeName = "<RTTI disabled>";
#endif // SCHNAPS_HAVE_RTTI
	schnaps_StackTraceEndM("void ObjectException::setObjectState(const Object& inObject)");
}
