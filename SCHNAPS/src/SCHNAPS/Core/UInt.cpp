/*
 * UInt.cpp
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
 * \brief Construct a unsigned integer object with the value given.
 * \param inValue Value of the unsigned integer instance.
 */
UInt::UInt(unsigned int inValue) :
	mValue(inValue)
{}

/*!
 * \brief Construct a unsigned integer object as a copy of the right unsigned integer.
 * \param inOrignal Original unsigned integer instance.
 */
UInt::UInt(const UInt& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief  Copy operator.
 * \param  inOriginal Source of copy.
 * \return A reference to the current object.
 */
UInt& UInt::operator=(const UInt& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt& SCHNAPS::Core::UInt::operator=(const SCHNAPS::Core::UInt&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void UInt::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool UInt::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const UInt& lRightWrapper = castObjectT<const UInt&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::UInt::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param  inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool UInt::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const UInt& lRightWrapper = castObjectT<const UInt&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::UInt::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */
void UInt::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::istringstream lISS(inStr);
	lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::UInt::readStr(std::string&)");
}

/*!
 * \brief  Write the object to string.
 * \return The object as string.
 */
std::string UInt::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::UInt::writeStr() const");
}

/*!
 * \brief  Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle UInt::clone() const {
	schnaps_StackTraceBeginM();
	return new UInt(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::UInt::clone() const");
}

/*!
 *  \brief  Compute absolute value.
 *  \return A reference to the resulting number.
 */
Number& UInt::abs() {
	schnaps_StackTraceBeginM();
	mValue = std::fabs(mValue);
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::abs()");
}

/*!
 *  \brief  Add a number.
 *  \return A reference to the resulting number.
 */
Number& UInt::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	UInt lRightUInt = inRightNumber;
	mValue += lRightUInt.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::add(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Divide by a number.
 *  \return A reference to the resulting number.
 */
Number& UInt::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	UInt lRightUInt = inRightNumber;
	mValue /= lRightUInt.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::div(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the modulo by a number.
 *  \return A reference to the resulting number.
 */
Number& UInt::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	UInt lRightUInt = inRightNumber;
	mValue % lRightUInt.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::mod(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Multiply by a number.
 *  \return A reference to the resulting number.
 */
Number& UInt::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	UInt lRightUInt = inRightNumber;
	mValue *= lRightUInt.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::mult(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Subtract a number.
 *  \return A reference to the resulting number.
 */
Number& UInt::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	UInt lRightUInt = inRightNumber;
	mValue -= lRightUInt.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 */
UInt::operator Double() const {
	schnaps_StackTraceBeginM();
	Double lDouble(static_cast<double>(mValue));
	return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Double() const");
}

/*!
 * \brief Casting operator to float.
 */
UInt::operator Float() const {
	schnaps_StackTraceBeginM();
	Float lFloat(static_cast<float>(mValue));
	return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Float() const");
}

/*!
 * \brief Casting operator to integer.
 */
UInt::operator Int() const {
	schnaps_StackTraceBeginM();
	Int lInt(static_cast<int>(mValue));
	return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 */
UInt::operator Long() const {
	schnaps_StackTraceBeginM();
	Long lLong(static_cast<long>(mValue));
	return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 */
UInt::operator UInt() const {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator UInt() const");
}

/*!
 * \brief Casting operator to unsigned long.
 */
UInt::operator ULong() const {
	schnaps_StackTraceBeginM();
	ULong lULong(static_cast<unsigned long>(mValue));
	return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator ULong() const");
}
