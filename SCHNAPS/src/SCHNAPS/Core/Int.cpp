/*
 * Int.cpp
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
 *  \brief Construct a int object with the value given.
 *  \param inValue Value of the int instance.
 */
Int::Int(int inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a int object as a copy of the right int.
 *  \param inOrignal Original int instance.
 */
Int::Int(const Int& inOriginal) :
		mValue(inOriginal.getValue())
{}

void Int::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool Int::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Int& lRightWrapper = castObjectT<const Int&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Int::isEqual(const SCHNAPS::Core::Object&) const");
}

bool Int::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Int& lRightWrapper = castObjectT<const Int&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Int::isLess(const SCHNAPS::Core::Object&) const");
}

Int& Int::operator=(const Int& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int& SCHNAPS::Core::Int::operator=(const SCHNAPS::Core::Int&)");
}

void Int::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Int::readStr(std::string&)");
}

std::string Int::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Int::writeStr() const");
}

AnyType::Handle Int::clone() const {
	schnaps_StackTraceBeginM();
		return new Int(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Int::clone() const");
}

Number& Int::abs() {
	schnaps_StackTraceBeginM();
		mValue = std::fabs(mValue);
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::abs()");
}

Number& Int::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Int lRightInt = inRightNumber;
		mValue += lRightInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::add(SCHNAPS::Core::Number&)");
}

Number& Int::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Int lRightInt = inRightNumber;
		mValue /= lRightInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::div(SCHNAPS::Core::Number&)");
}

Number& Int::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Int lRightInt = inRightNumber;
		mValue % lRightInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::mod(SCHNAPS::Core::Number&)");
}

Number& Int::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Int lRightInt = inRightNumber;
		mValue *= lRightInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::mult(SCHNAPS::Core::Number&)");
}

Number& Int::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	Int lRightInt = inRightNumber;
		mValue -= lRightInt.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Int::sub(SCHNAPS::Core::Number&)");
}

Int::operator Double() const {
	schnaps_StackTraceBeginM();
		Double lDouble(static_cast<double>(mValue));
		return lDouble;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator Double() const");
}

Int::operator Float() const {
	schnaps_StackTraceBeginM();
		Float lFloat(static_cast<float>(mValue));
		return lFloat;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator Float() const");
}

Int::operator Int() const {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator Int() const");
}

Int::operator Long() const {
	schnaps_StackTraceBeginM();
		Long lLong(static_cast<long>(mValue));
		return lLong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator Long() const");
}

Int::operator UInt() const {
	schnaps_StackTraceBeginM();
		UInt lUInt(static_cast<unsigned int>(mValue));
		return lUInt;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator UInt() const");
}

Int::operator ULong() const {
	schnaps_StackTraceBeginM();
		ULong lULong(static_cast<unsigned long>(mValue));
		return lULong;
	schnaps_StackTraceEndM("SCHNAPS::Core::Int::operator ULong() const");
}
