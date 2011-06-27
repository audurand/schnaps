/*
 * ULong.cpp
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
 * \brief Construct a unsigned long object with the value given.
 * \param inValue Value of the unsigned long instance.
 */
ULong::ULong(unsigned long inValue) :
	mValue(inValue)
{}

/*!
 * \brief Construct a unsigned long object as a copy of the right unsigned long.
 * \param inOrignal Original unsigned long instance.
 */
ULong::ULong(const ULong& inOriginal) {
	mValue = inOriginal.getValue();
}

/*!
 * \brief  Copy operator.
 * \param  inOriginal Source of copy.
 * \return A reference to the current object.
 */
ULong& ULong::operator=(const ULong& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong& SCHNAPS::Core::ULong::operator=(const SCHNAPS::Core::ULong&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void ULong::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool ULong::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const ULong& lRightWrapper = castObjectT<const ULong&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ULong::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param  inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool ULong::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const ULong& lRightWrapper = castObjectT<const ULong&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ULong::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 */
void ULong::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::stringstream lSS(inStr);
	lSS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::ULong::readStr(std::string&)");
}

/*!
 * \brief  Write the object to string.
 * \return The object as string.
 */
std::string ULong::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::ULong::writeStr() const");
}

/*!
 * \brief  Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle ULong::clone() const {
	schnaps_StackTraceBeginM();
	return new ULong(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::ULong::clone() const");
}

/*!
 * \brief  Compute the absolute value.
 * \return A handle to the resulting number.
 */
Number::Handle ULong::abs() {
	schnaps_StackTraceBeginM();
	return new ULong(mValue);
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::abs()");
}

/*!
 * \brief  Compute the addition a number.
 * \return A handle to the resulting number.
 */
Number::Handle ULong::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	ULong lRightLong = inRightNumber;
	return new ULong(mValue + lRightLong.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::add(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the division by a number.
 * \return A handle to the resulting number.
 */
Number::Handle ULong::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	ULong lRightLong = inRightNumber;
	return new ULong(mValue / lRightLong.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::div(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the base-e exponential function, which is the e number raised to the power x.
 *  \return A handle to the resulting number.
 */
Number::Handle ULong::exp() {
	schnaps_StackTraceBeginM();
	return new Double(std::exp(mValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::exp()");
}

/*!
 * \brief  Compute the modulo by a number.
 * \return A handle to the resulting number.
 */
Number::Handle ULong::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	ULong lRightLong = inRightNumber;
	return new Int(mValue % lRightLong.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::mod(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the multiplication by a number.
 * \return A handle to the resulting number.
 */
Number::Handle ULong::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	ULong lRightLong = inRightNumber;
	return new ULong(mValue * lRightLong.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::mult(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the subtraction by a number.
 * \return A handle to the resulting number.
 */

Number::Handle ULong::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	ULong lRightLong = inRightNumber;
	return new ULong(mValue - lRightLong.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::ULong::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 */
ULong::operator Double() const {
	schnaps_StackTraceBeginM();
	Double lDouble(static_cast<double>(mValue));
	return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Double() const");
}

/*!
 * \brief Casting operator to integer.
 */
ULong::operator Int() const {
	schnaps_StackTraceBeginM();
	Int lInt(static_cast<int>(mValue));
	return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 */
ULong::operator Long() const {
	schnaps_StackTraceBeginM();
	Long lLong(static_cast<long>(mValue));
	return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 */
ULong::operator UInt() const {
	schnaps_StackTraceBeginM();
	UInt lUInt(static_cast<unsigned int>(mValue));
	return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator UInt() const");
}

/*!
 * \brief Casting operator to unsigned long.
 */
ULong::operator ULong() const {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator ULong() const");
}
