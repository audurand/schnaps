/*
 * ULong.cpp
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
 *  \brief Construct a unsigned long object with the value given.
 *  \param inValue Value of the unsigned long instance.
 */
ULong::ULong(unsigned long inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a unsigned long object as a copy of the right unsigned long.
 *  \param inOrignal Original unsigned long instance.
 */
ULong::ULong(const ULong& inOriginal) {
	mValue = inOriginal.getValue();
}

void ULong::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool ULong::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const ULong& lRightWrapper = castObjectT<const ULong&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::ULong::isEqual(const core::Object&) const");
}

bool ULong::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const ULong& lRightWrapper = castObjectT<const ULong&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::ULong::isLess(const core::Object&) const");
}

ULong& ULong::operator=(const ULong& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("core::ULong& core::ULong::operator=(const core::ULong&)");
}

void ULong::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void core::ULong::readStr(std::string&)");
}

std::string ULong::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string core::ULong::writeStr() const");
}

AnyType::Handle ULong::clone() const {
	schnaps_StackTraceBeginM();
		return new ULong(*this);
	schnaps_StackTraceEndM("core::AnyType::Handle core::ULong::clone() const");
}

Number& ULong::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::abs()");
}

Number& ULong::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue += lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::add(core::Number&)");
}

Number& ULong::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue /= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::div(core::Number&)");
}

Number& ULong::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue % lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::mod(core::Number&)");
}

Number& ULong::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue *= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::mult(core::Number&)");
}

Number& ULong::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue -= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::ULong::sub(core::Number&)");
}

ULong::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("core::ULong::operator Double() const");
}

ULong::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("core::ULong::operator Float() const");
}

ULong::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("core::ULong::operator Int() const");
}

ULong::operator Long() const {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("core::ULong::operator Long() const");
}

ULong::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("core::ULong::operator UInt() const");
}

ULong::operator ULong() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::ULong::operator ULong() const");
}
