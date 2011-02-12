/*
 * Float.cpp
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

void Float::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool Float::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Float& lRightWrapper = castObjectT<const Float&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Float::isEqual(const core::Object&) const");
}

bool Float::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Float& lRightWrapper = castObjectT<const Float&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Float::isLess(const core::Object&) const");
}

Float& Float::operator=(const Float& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Float& core::Float::operator=(const core::Float&)");
}

void Float::readStr(std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void core::Float::readStr(std::string&)");
}

std::string Float::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string core::Float::writeStr() const");
}

AnyType::Handle Float::clone() const {
	schnaps_StackTraceBeginM();
		return new Float(*this);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Float::clone() const");
}

Number& Float::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::abs()");
}

Number& Float::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Float lRightFloat = inRightNumber;
		mValue += lRightFloat.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::add(core::Number&)");
}

Number& Float::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Float lRightFloat = inRightNumber;
		mValue /= lRightFloat.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::div(core::Number&)");
}

Number& Float::exp() {
	schnaps_StackTraceBeginM();
		mValue = std::exp(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::exp()");
}

Number& Float::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Float lRightFloat = inRightNumber;
		mValue *= lRightFloat.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::mult(core::Number&)");
}

Number& Float::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Float lRightFloat = inRightNumber;
		mValue = std::pow(mValue, lRightFloat.getValue());
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::pow(core::Number&)");
}

Number& Float::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Float lRightFloat = inRightNumber;
		mValue -= lRightFloat.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Float::sub(core::Number&)");
}

Float::operator Double() {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("core::Float::operator Double()");
}

Float::operator Float() {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Float::operator Float()");
}

Float::operator Int() {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("core::Float::operator Int()");
}

Float::operator Long() {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("core::Float::operator Long()");
}

Float::operator UInt() {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("core::Float::operator UInt()");
}

Float::operator ULong() {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("core::Float::operator ULong()");
}
