/*
 * Value.cpp
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
Value::Value() :
	Primitive(0),
	mValue_Ref(""),
	mValue(NULL)
{}

/*!
 * \brief Construct a value primitive as a copy of an original.
 * \param inOriginal A const reference to the original value.
 */
Value::Value(const Value& inOriginal) :
	Primitive(0),
	mValue_Ref(inOriginal.mValue_Ref.c_str())
{
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = inOriginal.mValue->clone();
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Value& Value::operator=(const Value& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue_Ref.assign(inOriginal.mValue_Ref.c_str());
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = inOriginal.mValue->clone();
			break;
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Data::Value& SCHNAPS::Plugins::Data::Value::operator=(const SCHNAPS::Plugins::Data::Value&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inValue attribute is missing.
 * \throw SCHNAPS::Core::IOException if inValue attribute is a direct value and inValue_Type attribute is missing.
 */
void Value::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve value
	if (inIter->getAttribute("inValue").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "value expected!");
	}
	mValue_Ref.assign(inIter->getAttribute("inValue"));
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = ioSystem.getParameters().getParameterHandle(mValue_Ref.substr(1));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inValue_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of value expected!");
			}
			Core::AnyType::Alloc::Handle lAlloc = Core::castHandleT<Core::AnyType::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inValue_Type")));
			mValue = Core::castHandleT<Core::AnyType>(lAlloc->allocate());
			mValue->readStr(mValue_Ref);
			break; }
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Value::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 * \throw SCHNAPS::Core::AssertException if the handle to token value is NULL. 
 */
void Value::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	
	if (mValue != NULL && mValue_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("invalue_Type", mValue->getType());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Value::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Value::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	Core::AnyType::Handle lValue;
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
			lValue = Core::castHandleT<Core::AnyType>(lContext.getIndividual().getState().getVariableHandle(mValue_Ref.substr(1))->clone());
			break;
		case '#':
			// environment variable value
			lValue = Core::castHandleT<Core::AnyType>(lContext.getEnvironment().getState().getVariableHandle(mValue_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lValue = Core::castHandleT<Core::AnyType>(lContext.getLocalVariableHandle(mValue_Ref.substr(1))->clone());
			break;
		default:
			// parameter value or direct value
			lValue = Core::castHandleT<Core::AnyType>(mValue->clone());
			break;
	}
	
	return lValue;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Data::Value::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific value source.
 */
const std::string& Value::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (mValue == NULL) {
		switch (mValue_Ref[0]) {
			case '@':
				// individual variable value
			case '#':
				// environment variable value
			case '%':
				// TODO: local variable value
				const static std::string lType("Core::AnyType");
				return lType;
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specific value source.");
				break;
		}
	} else {
		// parameter value or direct value
		return mValue->getType();
	}
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Value::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
