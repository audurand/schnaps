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

using namespace SCHNAPS;
using namespace Core;

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
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ULong::isEqual(const SCHNAPS::Core::Object&) const");
}

bool ULong::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const ULong& lRightWrapper = castObjectT<const ULong&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ULong::isLess(const SCHNAPS::Core::Object&) const");
}

ULong& ULong::operator=(const ULong& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong& SCHNAPS::Core::ULong::operator=(const SCHNAPS::Core::ULong&)");
}

void ULong::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::ULong::readStr(std::string&)");
}

std::string ULong::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::ULong::writeStr() const");
}

AnyType::Handle ULong::clone() const {
	schnaps_StackTraceBeginM();
		return new ULong(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::ULong::clone() const");
}

Number& ULong::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::abs()");
}

Number& ULong::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue += lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::add(SCHNAPS::Core::Number&)");
}

Number& ULong::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue /= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::div(SCHNAPS::Core::Number&)");
}

Number& ULong::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue % lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::mod(SCHNAPS::Core::Number&)");
}

Number& ULong::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue *= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::mult(SCHNAPS::Core::Number&)");
}

Number& ULong::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		ULong lRightLong = inRightNumber;
		mValue -= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::ULong::sub(SCHNAPS::Core::Number&)");
}

ULong::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Double() const");
}

ULong::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Float() const");
}

ULong::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Int() const");
}

ULong::operator Long() const {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator Long() const");
}

ULong::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator UInt() const");
}

ULong::operator ULong() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::ULong::operator ULong() const");
}
