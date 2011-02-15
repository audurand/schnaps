/*
 * UInt.cpp
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
 *  \brief Construct a unsigned int object with the value given.
 *  \param inValue Value of the unsigned int instance.
 */
UInt::UInt(unsigned int inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a unsigned int object as a copy of the right unsigned int.
 *  \param inOrignal Original unsigned int instance.
 */
UInt::UInt(const UInt& inOriginal) :
		mValue(inOriginal.getValue())
{}

void UInt::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool UInt::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const UInt& lRightWrapper = castObjectT<const UInt&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::UInt::isEqual(const SCHNAPS::Core::Object&) const");
}

bool UInt::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const UInt& lRightWrapper = castObjectT<const UInt&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::UInt::isLess(const SCHNAPS::Core::Object&) const");
}

UInt& UInt::operator=(const UInt& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt& SCHNAPS::Core::UInt::operator=(const SCHNAPS::Core::UInt&)");
}

void UInt::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::UInt::readStr(std::string&)");
}

std::string UInt::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::UInt::writeStr() const");
}

AnyType::Handle UInt::clone() const {
	schnaps_StackTraceBeginM();
		return new UInt(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::UInt::clone() const");
}

Number& UInt::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::abs()");
}

Number& UInt::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		UInt lRightUInt = inRightNumber;
		mValue += lRightUInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::add(SCHNAPS::Core::Number&)");
}

Number& UInt::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		UInt lRightUInt = inRightNumber;
		mValue /= lRightUInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::div(SCHNAPS::Core::Number&)");
}

Number& UInt::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		UInt lRightUInt = inRightNumber;
		mValue % lRightUInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::mod(SCHNAPS::Core::Number&)");
}

Number& UInt::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		UInt lRightUInt = inRightNumber;
		mValue *= lRightUInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::mult(SCHNAPS::Core::Number&)");
}

Number& UInt::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		UInt lRightUInt = inRightNumber;
		mValue -= lRightUInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::UInt::sub(SCHNAPS::Core::Number&)");
}

UInt::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Double() const");
}

UInt::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Float() const");
}

UInt::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Int() const");
}

UInt::operator Long() const {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator Long() const");
}

UInt::operator UInt() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator UInt() const");
}

UInt::operator ULong() const {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::UInt::operator ULong() const");
}
