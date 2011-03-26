/*
 * Float.cpp
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
 *  \brief Construct a float object with the value given.
 *  \param inValue Value of the float instance.
 */
Float::Float(float inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a float object as a copy of the right float.
 *  \param inOrignal Original float instance.
 */
Float::Float(const Float& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief  Copy operator.
 * \param  inOriginal Source of copy.
 * \return A reference to the current object.
 */
Float& Float::operator=(const Float& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float& SCHNAPS::Core::Float::operator=(const SCHNAPS::Core::Float&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void Float::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Float::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Float& lRightWrapper = castObjectT<const Float&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Float::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param  inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Float::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Float& lRightWrapper = castObjectT<const Float&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Float::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 */
void Float::readStr(std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::istringstream lISS(inStr);
	lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Float::readStr(std::string&)");
}

/*!
 * \brief  Write the object to string.
 * \return The object as string.
 */
std::string Float::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Float::writeStr() const");
}

/*!
 * \brief  Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle Float::clone() const {
	schnaps_StackTraceBeginM();
	return new Float(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Float::clone() const");
}

/*!
 * \brief  Compute the absolute value.
 * \return A handle to the resulting number.
 */
Number::Handle Float::abs() {
	schnaps_StackTraceBeginM();
	return new Float(std::fabs(mValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::abs()");
}

/*!
 * \brief  Compute the addition a number.
 * \return A handle to the resulting number.
 */
Number::Handle Float::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Float lRightFloat = inRightNumber;
	return new Float(mValue + lRightFloat.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::add(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the division by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Float::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Float lRightFloat = inRightNumber;
	return new Float(mValue / lRightFloat.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::div(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the base-e exponential function, which is the e number raised to the power x.
 *  \return A handle to the resulting number.
 */
Number::Handle Float::exp() {
	schnaps_StackTraceBeginM();
	return new Float(std::exp(mValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::exp()");
}

/*!
 * \brief  Compute the multiplication by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Float::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Float lRightFloat = inRightNumber;
	return new Float(mValue * lRightFloat.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::mult(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the raise to the power exponent.
 *  \return A handle to the resulting number.
 */
Number::Handle Float::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Float lRightFloat = inRightNumber;
	return new Float(std::pow(mValue, lRightFloat.getValue()));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::pow(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the subtraction by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Float::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Float lRightFloat = inRightNumber;
	return new Float(mValue - lRightFloat.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Float::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 */
Float::operator Double() const {
	schnaps_StackTraceBeginM();
	Double lDouble(static_cast<double>(mValue));
	return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator Double() const");
}

/*!
 * \brief Casting operator to float.
 */
Float::operator Float() const {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator Float() const");
}

/*!
 * \brief Casting operator to integer.
 */
Float::operator Int() const {
	schnaps_StackTraceBeginM();
	Int lInt(static_cast<int>(mValue));
	return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 */
Float::operator Long() const {
	schnaps_StackTraceBeginM();
	Long lLong(static_cast<long>(mValue));
	return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 */
Float::operator UInt() const {
	schnaps_StackTraceBeginM();
	UInt lUInt(static_cast<unsigned int>(mValue));
	return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator UInt() const");
}

/*!
 * \brief Casting operator to unsigned long.
 */
Float::operator ULong() const {
	schnaps_StackTraceBeginM();
	ULong lULong(static_cast<unsigned long>(mValue));
	return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Float::operator ULong() const");
}
