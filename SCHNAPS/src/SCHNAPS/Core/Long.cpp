/*
 * Long.cpp
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

#include <math.h>

using namespace SCHNAPS;
using namespace Core;

/*!
 *  \brief Construct a long object with the value given.
 *  \param inValue Value of the long instance.
 */
Long::Long(long inValue) :
	mValue(inValue)
{}

/*!
 *  \brief Construct a long object as a copy of the right long.
 *  \param inOrignal Original long instance.
 */
Long::Long(const Long& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief  Copy operator.
 * \param  inOriginal Source of copy.
 * \return A reference to the current object.
 */
Long& Long::operator=(const Long& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long& SCHNAPS::Core::Long::operator=(const SCHNAPS::Core::Long&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void Long::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Long::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Long& lRightWrapper = castObjectT<const Long&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Long::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param  inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Long::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Long& lRightWrapper = castObjectT<const Long&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Long::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */
void Long::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::istringstream lISS(inStr);
	lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Long::readStr(std::string&)");
}

/*!
 * \brief  Write the object to string.
 * \return The object as string.
 */
std::string Long::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Long::writeStr() const");
}

/*!
 * \brief  Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle Long::clone() const {
	schnaps_StackTraceBeginM();
	return new Long(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Long::clone() const");
}

/*!
 *  \brief  Compute absolute value.
 *  \return A reference to the resulting number.
 */
Number& Long::abs() {
	schnaps_StackTraceBeginM();
	mValue = std::fabs(mValue);
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::abs()");
}

/*!
 *  \brief  Add a number.
 *  \return A reference to the resulting number.
 */
Number& Long::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Long lRightLong = inRightNumber;
	mValue += lRightLong.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::add(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Divide by a number.
 *  \return A reference to the resulting number.
 */
Number& Long::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Long lRightLong = inRightNumber;
	mValue /= lRightLong.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::div(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the modulo by a number.
 *  \return A reference to the resulting number.
 */
Number& Long::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Long lRightLong = inRightNumber;
	mValue % lRightLong.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::mod(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Multiply by a number.
 *  \return A reference to the resulting number.
 */
Number& Long::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Long lRightLong = inRightNumber;
	mValue *= lRightLong.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::mult(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Subtract a number.
 *  \return A reference to the resulting number.
 */
Number& Long::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Long lRightLong = inRightNumber;
	mValue -= lRightLong.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 */
Long::operator Double() const {
	schnaps_StackTraceBeginM();
	Double lDouble(static_cast<double>(mValue));
	return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Double() const");
}

/*!
 * \brief Casting operator to float.
 */
Long::operator Float() const {
	schnaps_StackTraceBeginM();
	Float lFloat(static_cast<float>(mValue));
	return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Float() const");
}

/*!
 * \brief Casting operator to integer.
 */
Long::operator Int() const {
	schnaps_StackTraceBeginM();
	Int lInt(static_cast<int>(mValue));
	return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 */
Long::operator Long() const {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 */
Long::operator UInt() const {
	schnaps_StackTraceBeginM();
	UInt lUInt(static_cast<unsigned int>(mValue));
	return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator UInt() const");
}

/*!
 * \brief Casting operator to unsigned long.
 */
Long::operator ULong() const {
	schnaps_StackTraceBeginM();
	ULong lULong(static_cast<unsigned long>(mValue));
	return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator ULong() const");
}