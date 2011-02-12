/*
 * Bool.cpp
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

using namespace core;

/*!
 *  \brief Construct a bool object with the value given.
 *  \param inValue Value of the bool instance.
 */
Bool::Bool(bool inValue) :
		mValue(inValue)
{}

/*!
 *  \brief Construct a bool object as a copy of the right bool.
 *  \param inOrignal Original bool instance.
 */
Bool::Bool(const Bool& inOriginal) :
		mValue(inOriginal.getValue())
{}

void Bool::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("value", this->writeStr());
}

bool Bool::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Bool& lRightWrapper = castObjectT<const Bool&>(inRightObj);
		return mValue == lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Bool::isEqual(const core::Object&) const");
}

bool Bool::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Bool& lRightWrapper = castObjectT<const Bool&>(inRightObj);
		return mValue < lRightWrapper.mValue;
	schnaps_StackTraceEndM("bool core::Bool::isLess(const core::Object&) const");
}

Bool& Bool::operator=(const Bool& inOriginal) {
	schnaps_StackTraceBeginM();
		mValue = inOriginal.getValue();
		return *this;
	schnaps_StackTraceEndM("core::Bool& core::Bool::operator=(const core::Bool&)");
}

void Bool::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		if ((inStr == "true") || (inStr == "1")) {
			mValue = true;
		} else if ((inStr == "false") || (inStr == "0")) {
			mValue = false;
		} else {
			throw schnaps_InternalExceptionM("Unknown bool value '" + inStr + "'!");
		}
	schnaps_StackTraceEndM("void core::Bool::readStr(std::string&)");
}

std::string Bool::writeStr() const {
	schnaps_StackTraceBeginM();
		std::string lStr;
		if (mValue) {
			lStr = "true";
		} else {
			lStr = "false";
		}
		return lStr;
	schnaps_StackTraceEndM("std::string core::Bool::writeStr() const");
}

AnyType::Handle Bool::clone() const {
	schnaps_StackTraceBeginM();
		return new Bool(*this);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Bool::clone() const");
}
