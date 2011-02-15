/*
 * String.cpp
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

using namespace SCHNAPS;
using namespace Core;

/*!
 *  \brief Construct a string object with the value given.
 *  \param inValue Value of the string instance.
 */
String::String(std::string inValue) :
		mValue(inValue.c_str())
{}

/*!
 *  \brief Construct a string object as a copy of the right string.
 *  \param inOrignal Original string instance.
 */
String::String(const String& inOriginal) {
	mValue = inOriginal.getValue().c_str();
}

void String::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool String::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const String& lRightWrapper = castObjectT<const String&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::String::isEqual(const SCHNAPS::Core::Object&) const");
}

bool String::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const String& lRightWrapper = castObjectT<const String&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::String::isLess(const SCHNAPS::Core::Object&) const");
}

String& String::operator=(const String& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue().c_str();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::String& SCHNAPS::Core::String::operator=(const SCHNAPS::Core::String&)");
}

void String::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		mValue = inStr.c_str();
	schnaps_StackTraceEndM("void SCHNAPS::Core::String::readStr(const std::string&)");
}

std::string String::writeStr() const {
	schnaps_StackTraceBeginM();
		return mValue;
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::String::writeStr() const");
}

AnyType::Handle String::clone() const {
	schnaps_StackTraceBeginM();
		return new String(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::String::clone() const");
}
