/*
 * Char.cpp
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
 *  \brief Construct a char object with the value given.
 *  \param inValue Value of the char instance.
 */
Char::Char(char inValue) :
	mValue(inValue)
{}

/*!
 *  \brief Construct a char object as a copy of the right char.
 *  \param inOrignal Original char instance.
 */
Char::Char(const Char& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
Char& Char::operator=(const Char& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Char& SCHNAPS::Core::Char::operator=(const SCHNAPS::Core::Char&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void Char::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief Test if an object is equal to another.
 * \param inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Char::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Char& lRightWrapper = castObjectT<const Char&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Char::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Test if an object is less than another.
 * \param inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Char::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Char& lRightWrapper = castObjectT<const Char&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Char::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */
void Char::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::istringstream lISS(inStr);
	lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Char::readStr(std::string&)");
}

/*!
 * \brief Write the object to string.
 * \return The object as string.
 */
std::string Char::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Char::writeStr() const");
}

/*!
 * \brief Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle Char::clone() const {
	schnaps_StackTraceBeginM();
	return new Char(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::Atom::Handle SCHNAPS::Core::Char::clone() const");
}
