/*
 * Double.cpp
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
 *  \brief Construct a double object with the value given.
 *  \param inValue Value of the double instance.
 */
Double::Double(double inValue) :
	mValue(inValue)
{}

/*!
 *  \brief Construct a double object as a copy of the right double.
 *  \param inOrignal Original double instance.
 */
Double::Double(const Double& inOriginal) :
	mValue(inOriginal.getValue())
{}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
Double& Double::operator=(const Double& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue = inOriginal.getValue();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double& SCHNAPS::Core::Double::operator=(const SCHNAPS::Core::Double&)");
}

/*!
 * \brief Write the content of the object to XML.
 * \param ioStreamer The XML streamer for output.
 * \param inIndent True for indentation.
 */
void Double::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

/*!
 * \brief Test if an object is equal to another.
 * \param inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Double::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Double& lRightWrapper = castObjectT<const Double&>(inRightObj);
	return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Double::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Test if an object is less than another.
 * \param inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Double::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Double& lRightWrapper = castObjectT<const Double&>(inRightObj);
	return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Double::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException if the string contains an unknown value for object.
 */
void Double::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::istringstream lISS(inStr);
	lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Double::readStr(std::string&)");
}

/*!
 * \brief Write the object to string.
 * \return The object as string.
 */
std::string Double::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	lOSS << mValue;
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Double::writeStr() const");
}

/*!
 * \brief Return a handle to a clone of the current object.
 * \return A handle to a clone of the current object.
 */
AnyType::Handle Double::clone() const {
	schnaps_StackTraceBeginM();
	return new Double(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Double::clone() const");
}

/*!
 * \brief  Compute the absolute value.
 * \return A handle to the resulting number.
 */
Number::Handle Double::abs() {
	schnaps_StackTraceBeginM();
	return new Double(std::fabs(mValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::abs()");
}

/*!
 * \brief  Compute the addition a number.
 * \return A handle to the resulting number.
 */
Number::Handle Double::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Double lRightDouble = inRightNumber;
	return new Double(mValue + lRightDouble.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::add(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the division by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Double::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Double lRightDouble = inRightNumber;
	return new Double(mValue / lRightDouble.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::div(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the base-e exponential function, which is the e number raised to the power x.
 *  \return A handle to the resulting number.
 */
Number::Handle Double::exp() {
	schnaps_StackTraceBeginM();
	return new Double(std::exp(mValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::exp()");
}

/*!
 * \brief  Compute the multiplication by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Double::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Double lRightDouble = inRightNumber;
	return new Double(mValue * lRightDouble.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::mult(SCHNAPS::Core::Number&)");
}

/*!
 *  \brief  Compute the raise to the power exponent.
 *  \return A handle to the resulting number.
 */
Number::Handle Double::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Double lRightDouble = inRightNumber;
	return new Double(std::pow(mValue, lRightDouble.getValue()));
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::pow(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the subtraction by a number.
 * \return A handle to the resulting number.
 */
Number::Handle Double::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Double lRightDouble = inRightNumber;
	return new Double(mValue - lRightDouble.getValue());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Double::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 */
Double::operator Double() const {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator Double() const");
}

/*!
 * \brief Casting operator to float.
 */
Double::operator Float() const {
	schnaps_StackTraceBeginM();
	Float lFloat(static_cast<float>(mValue));
	return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator Float() const");
}

/*!
 * \brief Casting operator to integer.
 */
Double::operator Int() const {
	schnaps_StackTraceBeginM();
	Int lInt(static_cast<int>(mValue));
	return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 */
Double::operator Long() const {
	schnaps_StackTraceBeginM();
	Long lLong(static_cast<long>(mValue));
	return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 */
Double::operator UInt() const {
	schnaps_StackTraceBeginM();
	UInt lUInt(static_cast<unsigned int>(mValue));
	return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator UInt() const");
}

/*!
 * \brief Casting operator to unsigned long.
 */
Double::operator ULong() const {
	schnaps_StackTraceBeginM();
	ULong lULong(static_cast<unsigned long>(mValue));
	return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Double::operator ULong() const");
}
