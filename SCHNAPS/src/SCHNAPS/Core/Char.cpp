/*
 * Char.cpp
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
 *  \brief Construct a char object with the value given.
 *  \param inValue Value of the char instance.
 */
Char::Char(char inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a char object as a copy of the right char.
 *  \param inOrignal Original char instance.
 */
Char::Char(const Char& inOriginal) :
		mValue(inOriginal.getValue())
{}

void Char::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", mValue);
}

bool Char::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Char& lRightWrapper = castObjectT<const Char&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Char::isEqual(const SCHNAPS::Core::Object&) const");
}

bool Char::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Char& lRightWrapper = castObjectT<const Char&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Char::isLess(const SCHNAPS::Core::Object&) const");
}

Char& Char::operator=(const Char& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Char& SCHNAPS::Core::Char::operator=(const SCHNAPS::Core::Char&)");
}

void Char::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		lISS >> mValue;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Char::readStr(std::string&)");
}

std::string Char::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		lOSS << mValue;
		return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Char::writeStr() const");
}

AnyType::Handle Char::clone() const {
	schnaps_StackTraceBeginM();
		return new Char(*this);
	schnaps_StackTraceEndM("SCHNAPS::Core::Atom::Handle SCHNAPS::Core::Char::clone() const");
}
