/*
 * IsBetween.cpp
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 * \brief Default constructor.
 */
IsBetween::IsBetween() :
	Core::Primitive(0),
	mValue_Ref(""),
	mValue(NULL),
	mArgLeft_Ref(""),
	mArgLeft(NULL),
	mArgRight_Ref(""),
	mArgRight(NULL)
{}

/*!
 * \brief Construct a comparison operator "is value between arguments" as a copy of an original.
 * \param inOriginal A const reference to the original comparison operator "is value between arguments".
 */
IsBetween::IsBetween(const IsBetween& inOriginal) :
	Core::Primitive(0),
	mValue_Ref(inOriginal.mValue_Ref.c_str()),
	mArgLeft_Ref(inOriginal.mArgLeft_Ref.c_str()),
	mArgRight_Ref(inOriginal.mArgRight_Ref.c_str())
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
			mValue = Core::castHandleT<Core::Number>(inOriginal.mValue->clone());
	}
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = inOriginal.mArgLeft;
			break;
		default:
			// direct value
			mArgLeft = Core::castHandleT<Core::Number>(inOriginal.mArgLeft->clone());
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = inOriginal.mArgRight;
			break;
		default:
			// direct value
			mArgRight = Core::castHandleT<Core::Number>(inOriginal.mArgRight->clone());
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
IsBetween& IsBetween::operator=(const IsBetween& inOriginal) {
	schnaps_StackTraceBeginM();
	mValue_Ref.assign(inOriginal.mValue_Ref.c_str());
	mArgLeft_Ref.assign(inOriginal.mArgLeft_Ref.c_str());
	mArgRight_Ref.assign(inOriginal.mArgRight_Ref.c_str());
	
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
			mValue = Core::castHandleT<Core::Number>(inOriginal.mValue->clone());
	}
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = inOriginal.mArgLeft;
			break;
		default:
			// direct value
			mArgLeft = Core::castHandleT<Core::Number>(inOriginal.mArgLeft->clone());
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = inOriginal.mArgRight;
			break;
		default:
			// direct value
			mArgRight = Core::castHandleT<Core::Number>(inOriginal.mArgRight->clone());
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::IsBetween& SCHNAPS::Plugins::Operators::IsBetween::operator=(const SCHNAPS::Plugins::Operators::IsBetween&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inValue, inArgLeft, or inArgRight attributes are missing.
 * \throw SCHNAPS::Core::IOException if inValue attribute is a direct value and inValue_Type attribute is missing.
 * \throw SCHNAPS::Core::IOException if inArgLeft attribute is a direct value and inArgLeft_Type attribute is missing.
 * \throw SCHNAPS::Core::IOException if inArgRight attribute is a direct value and inArgRight_Type attribute is missing.
 */
void IsBetween::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve value to compare
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
			mValue = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(mArgLeft_Ref.substr(1)));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inValue_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of value expected!");
			}
			Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inValue_Type")));
			mValue = Core::castHandleT<Core::Number>(lAlloc->allocate());
			mValue->readStr(mValue_Ref);
			break; }
	}

	// retrieve left argument
	if (inIter->getAttribute("inArgLeft").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "left argument expected!");
	}
	mArgLeft_Ref.assign(inIter->getAttribute("inArgLeft"));
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgLeft = NULL;
			break;
		case '$':
			// parameter value
			mArgLeft = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(mArgLeft_Ref.substr(1)));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inArgLeft_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of left argument expected!");
			}
			Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inArgLeft_Type")));
			mArgLeft = Core::castHandleT<Core::Number>(lAlloc->allocate());
			mArgLeft->readStr(mArgLeft_Ref);
			break; }
	}
	
	// retrieve right argument
	if (inIter->getAttribute("inArgRight").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "right argument expected!");
	}
	mArgRight_Ref.assign(inIter->getAttribute("inArgRight"));
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArgRight = NULL;
			break;
		case '$':
			// parameter value
			mArgRight = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(mArgRight_Ref.substr(1)));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inArgRight_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of right argument expected!");
			}
			Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inArgLeft_Type")));
			mArgRight = Core::castHandleT<Core::Number>(lAlloc->allocate());
			mArgRight->readStr(mArgRight_Ref);
			break; }
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsBetween::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void IsBetween::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	ioStreamer.insertAttribute("inArgLeft", mArgLeft_Ref);
	ioStreamer.insertAttribute("inArgRight", mArgRight_Ref);
	
	if (mValue != NULL & mValue_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inValue_Type", mValue->getType());
	}
	if (mArgLeft != NULL & mArgLeft_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inArgLeft_Type", mArgLeft->getType());
	}
	if (mArgRight != NULL & mArgRight_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inArgRight_Type", mArgRight->getType());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsBetween::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle IsBetween::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArgLeft, lArgRight, lValue;
	
	switch (mArgLeft_Ref[0]) {
		case '@':
			// individual variable value
			lArgLeft = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getIndividual().getState().getVariableHandle(mArgLeft_Ref.substr(1)));
			break;
		case '#':
			// environment variable value
			lArgLeft = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getEnvironment().getState().getVariableHandle(mArgLeft_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lArgLeft = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::SimulationContext&>(ioContext).getLocalVariableHandle(mArgLeft_Ref.substr(1)));
			break;
		case '$':
			// parameter value or
			lArgLeft = Core::castHandleT<Core::Number>(mArgLeft->clone());
			break;
		default:
			// direct value
			lArgLeft = mArgLeft;
			break;
	}
	
	switch (mArgRight_Ref[0]) {
		case '@':
			// individual variable value
			lArgRight = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getIndividual().getState().getVariableHandle(mArgRight_Ref.substr(1)));
			break;
		case '#':
			// environment variable value
			lArgRight = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getEnvironment().getState().getVariableHandle(mArgRight_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lArgRight = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::SimulationContext&>(ioContext).getLocalVariableHandle(mArgRight_Ref.substr(1)));
			break;
		case '$':
			// parameter value
			lArgRight = Core::castHandleT<Core::Number>(mArgRight->clone());
			break;
		default:
			// direct value
			lArgRight = mArgRight;
			break;
	}
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
			lValue = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getIndividual().getState().getVariableHandle(mValue_Ref.substr(1)));
			break;
		case '#':
			// environment variable value
			lValue = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getEnvironment().getState().getVariableHandle(mValue_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lValue = Core::castHandleT<Core::Number>(Core::castObjectT<Simulation::SimulationContext&>(ioContext).getLocalVariableHandle(mValue_Ref.substr(1)));
			break;
		case '$':
			// parameter value
			lValue = Core::castHandleT<Core::Number>(mValue->clone());
			break;
		default:
			// direct value
			lValue = mValue;
			break;
	}
	
	return new Core::Bool(!lValue->isLess(*lArgLeft) && !lArgRight->isLess(*lValue));
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsBetween::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IsBetween::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetween::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
