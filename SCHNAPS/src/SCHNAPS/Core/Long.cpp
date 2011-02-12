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

using namespace core;

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
	schnaps_StackTraceEndM("bool core::Long::isEqual(const core::Object&) const");
}

bool Long::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Long& lRightWrapper = castObjectT<const Long&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Long::isLess(const core::Object&) const");
}

Long& Long::operator=(const Long& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Long& core::Long::operator=(const core::Long&)");
}

void Long::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void core::Long::readStr(std::string&)");
}

std::string Long::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string core::Long::writeStr() const");
}

AnyType::Handle Long::clone() const {
	schnaps_StackTraceBeginM();
		return new Long(*this);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Long::clone() const");
}

Number& Long::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::abs()");
}

Number& Long::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue += lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::add(core::Number&)");
}

Number& Long::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue /= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::div(core::Number&)");
}

Number& Long::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue % lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::mod(core::Number&)");
}

Number& Long::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue *= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::mult(core::Number&)");
}

Number& Long::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		Long lRightLong = inRightNumber;
		mValue -= lRightLong.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Long::sub(core::Number&)");
}

Long::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("core::Long::operator Double() const");
}

Long::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("core::Long::operator Float() const");
}

Long::operator Int() const {
	schnaps_StackTraceBeginM();
		Int lInt(static_cast<int>(mValue));
		return lInt;
	schnaps_StackTraceEndM("core::Long::operator Int() const");
}

Long::operator Long() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Long::operator Long() const");
}

Long::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("core::Long::operator UInt() const");
}

Long::operator ULong() const {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("core::Long::operator ULong() const");
}
