/*
 * Double.cpp
 *
 *  Created on: 2009-01-23
 *  Author: Audrey Durand
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

using namespace core;

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

void Double::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool Double::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Double& lRightWrapper = castObjectT<const Double&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Double::isEqual(const core::Object&) const");
}

bool Double::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Double& lRightWrapper = castObjectT<const Double&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Double::isLess(const core::Object&) const");
}

Double& Double::operator=(const Double& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Double& core::Double::operator=(const core::Double&)");
}

void Double::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void core::Double::readStr(std::string&)");
}

std::string Double::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string core::Double::writeStr() const");
}

AnyType::Handle Double::clone() const {
	schnaps_StackTraceBeginM();
		return new Double(*this);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Double::clone() const");
}

Number& Double::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::abs()");
}

Number& Double::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Double lRightDouble = inRightNumber;
		mValue += lRightDouble.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::add(core::Number&)");
}

Number& Double::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Double lRightDouble = inRightNumber;
		mValue /= lRightDouble.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::div(core::Number&)");
}

Number& Double::exp() {
	schnaps_StackTraceBeginM();
		mValue = std::exp(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::exp()");
}

Number& Double::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Double lRightDouble = inRightNumber;
		mValue *= lRightDouble.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::mult(core::Number& )");
}

Number& Double::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Double lRightDouble = inRightNumber;
		mValue = std::pow(mValue, lRightDouble.getValue());
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::pow(core::Number&)");
}

Number& Double::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Double lRightDouble = inRightNumber;
		mValue -= lRightDouble.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Double::sub(core::Number&)");
}

Double::operator Double() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Double::operator Double() const");
}

Double::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("core::Double::operator Float() const");
}

Double::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("core::Double::operator Int() const");
}

Double::operator Long() const {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("core::Double::operator Long() const");
}

Double::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("core::Double::operator UInt() const");
}

Double::operator ULong() const {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("core::Double::operator ULong() const");
}
