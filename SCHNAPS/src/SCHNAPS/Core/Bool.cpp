/*
 * Bool.cpp
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
 * \brief Construct a boolean with the value given.
 * \param inValue Value of the boolean instance.
 */
Bool::Bool(bool inValue) :
	mValue(inValue)
{}

/*!
 * \brief Construct a boolean as a copy of the right boolean.
 * \param inOrignal Original boolean instance.
 */
Bool::Bool(const Bool& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
Bool& Bool::operator=(const Bool& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Bool& SCHNAPS::Core::Bool::operator=(const SCHNAPS::Core::Bool&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void Bool::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", this->writeStr());
}

/*!
 * \brief Test if an object is equal to another.
 * \param inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Bool::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Bool& lRightWrapper = castObjectT<const Bool&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Bool::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Test if an object is less than another.
 * \param inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Bool::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Bool& lRightWrapper = castObjectT<const Bool&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Bool::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */
void Bool::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	if ((inStr == "true") || (inStr == "1")) {
		mValue = true;
	} else if ((inStr == "false") || (inStr == "0")) {
		mValue = false;
	} else {
		throw schnaps_InternalExceptionM("Unknown bool value '" + inStr + "'!");
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Bool::readStr(std::string&)");
}

/*!
 * \brief Write the object to string.
 * \return The object as string.
 */
std::string Bool::writeStr() const {
	schnaps_StackTraceBeginM();
	std::string lStr;
	if (mValue) {
		lStr = "true";
	} else {
		lStr = "false";
	}
	return lStr;
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Bool::writeStr() const");
}

/*!
 * \brief Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle Bool::clone() const {
	schnaps_StackTraceBeginM();
	return new Bool(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Bool::clone() const");
}
