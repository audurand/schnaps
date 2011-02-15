/*
 * Long.cpp
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

void Long::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool Long::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Long& lRightWrapper = castObjectT<const Long&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Long::isEqual(const SCHNAPS::Core::Object&) const");
}

bool Long::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Long& lRightWrapper = castObjectT<const Long&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Long::isLess(const SCHNAPS::Core::Object&) const");
}

Long& Long::operator=(const Long& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long& SCHNAPS::Core::Long::operator=(const SCHNAPS::Core::Long&)");
}

void Long::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Long::readStr(std::string&)");
}

std::string Long::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Long::writeStr() const");
}

AnyType::Handle Long::clone() const {
	schnaps_StackTraceBeginM();
		return new Long(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Long::clone() const");
}

Number& Long::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::abs()");
}

Number& Long::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue += lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::add(SCHNAPS::Core::Number&)");
}

Number& Long::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue /= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::div(SCHNAPS::Core::Number&)");
}

Number& Long::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue % lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::mod(SCHNAPS::Core::Number&)");
}

Number& Long::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue *= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::mult(SCHNAPS::Core::Number&)");
}

Number& Long::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue -= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Long::sub(SCHNAPS::Core::Number&)");
}

Long::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Double() const");
}

Long::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Float() const");
}

Long::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Int() const");
}

Long::operator Long() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator Long() const");
}

Long::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator UInt() const");
}

Long::operator ULong() const {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Long::operator ULong() const");
}
