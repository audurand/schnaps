/*
 * Parameter.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 * \brief Default constructor.
 */
Parameter::Parameter() :
	Primitive(0),
	mLabel(""),
	mParameterHandle(NULL)
{}

/*!
 * \brief Construct a primitive for accessing a parameter as a copy of an original.
 * \param inOriginal The original primitive for accessing a parameter.
 */
Parameter::Parameter(const Parameter& inOriginal) :
	Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mParameterHandle(inOriginal.mParameterHandle)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Parameter& Parameter::operator=(const Parameter& inOriginal) {
	schnaps_StackTraceBeginM();
	mLabel = inOriginal.mLabel.c_str();
	mParameterHandle = inOriginal.mParameterHandle;
	return *this;
	schnaps_StackTraceEndM("Core::Parameter& SCHNAPS::Plugins::Data::Parameter::operator=(const SCHNAPS::Core::Parameter&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Parameter::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	mParameterHandle = ioSystem.getParameters().getParameterHandle(lSS.str().c_str());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Parameter::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Parameter::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Parameter::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::AssertException if the parameter handle is NULL.
 */
Core::AnyType::Handle Parameter::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mParameterHandle);
	return mParameterHandle->clone();
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::Parameter::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw  SCHNAPS::Core::AssertException if the parameter handle is NULL.
 */
const std::string& Parameter::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mParameterHandle);
	return mParameterHandle->getType();
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Parameter::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
