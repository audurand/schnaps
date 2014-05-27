/*
 * String.cpp
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
 * \brief Construct a string with the value given.
 * \param inValue Value of the string instance.
 */
String::String(std::string inValue) :
	mValue(inValue.c_str())
{}

/*!
 * \brief Construct a string as a copy of the right string.
 * \param inOrignal Original string instance.
 */
String::String(const String& inOriginal) {
	mValue = inOriginal.getValue().c_str();
}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
String& String::operator=(const String& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue().c_str();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::String& SCHNAPS::Core::String::operator=(const SCHNAPS::Core::String&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void String::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief Test if an object is equal to another.
 * \param inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool String::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const String& lRightWrapper = castObjectT<const String&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::String::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Test if an object is less than another.
 * \param inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool String::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const String& lRightWrapper = castObjectT<const String&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::String::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */

void String::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	mValue = inStr.c_str();
	schnaps_StackTraceEndM("void SCHNAPS::Core::String::readStr(const std::string&)");
}

/*!
 * \brief Write the object to string.
 * \return The object as string.
 */
std::string String::writeStr() const {
	schnaps_StackTraceBeginM();
	return mValue;
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::String::writeStr() const");
}

/*!
 * \brief Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle String::clone() const {
	schnaps_StackTraceBeginM();
	return new String(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::String::clone() const");
}
