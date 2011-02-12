/*
 * Parameter.cpp
 *
 *  Created on: 2010-08-03
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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

/*!
 *  \brief Construct a new token terminal primitive.
 */
Parameter::Parameter() :
		Primitive(0),
		mLabel(""),
		mParameterHandle(NULL)
{}

Parameter::Parameter(const Parameter& inOriginal) :
		Primitive(0),
		mLabel(inOriginal.mLabel.c_str()),
		mParameterHandle(inOriginal.mParameterHandle)
{}

void Parameter::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of parameter expected!");
		}

		mLabel = inIter->getAttribute("label");

		std::stringstream lSS;
		lSS << "ref." << mLabel;
		mParameterHandle = ioSystem.getParameters()[lSS.str().c_str()];
	schnaps_StackTraceEndM("void core::Parameter::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Parameter::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void core::Parameter::writeContent(PACC::XML::Streamer&, bool) const");
}

Parameter& Parameter::operator=(const Parameter& inOriginal) {
	schnaps_StackTraceBeginM();
		mLabel = inOriginal.mLabel.c_str();
		mParameterHandle = inOriginal.mParameterHandle;
		return *this;
	schnaps_StackTraceEndM("core::Parameter& core::Parameter::operator=(const core::Parameter&)");
}

core::AnyType::Handle Parameter::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mParameterHandle);
		return mParameterHandle->clone();
	schnaps_StackTraceEndM("core::AnyType::Handle core::Parameter::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Parameter::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mParameterHandle);
		return mParameterHandle->getType();
	schnaps_StackTraceEndM("const std::string& core::Parameter::getReturnType(unsigned int, core::ExecutionContext&) const");
}
