/*
 * SetVariable.cpp
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
SetVariable::SetVariable() :
		Core::Primitive(0),
		mVariable_Ref(""),
		mValue_Ref(""),
		mValue(NULL)
{}

/*!
 * \brief Construct a primitive for setting variable value as a copy of an original.
 * \param inOriginal The original primitive for setting variable value.
 */
SetVariable::SetVariable(const SetVariable& inOriginal) :
		Core::Primitive(0),
		mVariable_Ref(inOriginal.mVariable_Ref.c_str()),
		mValue_Ref(inOriginal.mValue_Ref.c_str())
{
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = inOriginal.mValue->clone();
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
SetVariable& SetVariable::operator=(const SetVariable& inOriginal) {
	schnaps_StackTraceBeginM();
	mVariable_Ref.assign(inOriginal.mVariable_Ref.c_str());
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
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Data::SetVariable& SCHNAPS::Plugins::Data::SetVariable::operator=(const SCHNAPS::Plugins::Data::SetVariable&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if outVariable or inValue attributes are missing.
 * \throw SCHNAPS::Core::IOException if inValue attribute is a direct value and inValue_Type attribute is missing.
 * \throw SCHNAPS::Core::RunTimeException if outVariable attribute is not a reference to a variable.
 */
void SetVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve output variable
	if (inIter->getAttribute("outVariable").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "variable where to store result expected!");
	}
	mVariable_Ref.assign(inIter->getAttribute("outVariable"));
	
	if (mVariable_Ref[0] != '@' && mVariable_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific variable source!");
	}

	// retrieve new value
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariable::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SetVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outVariable", mVariable_Ref);
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	
	if (mValue != NULL && mValue_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inValue_Type", mValue->getType());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the type of new value does not match the type of variable.
 */
Core::AnyType::Handle SetVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	Core::AnyType::Handle lValue;
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
			lValue = lContext.getIndividual().getState().getVariableHandle(mValue_Ref.substr(1))->clone();
			break;
		case '#':
			// environment variable value
			lValue = lContext.getEnvironment().getState().getVariableHandle(mValue_Ref.substr(1))->clone();
			break;
		case '%':
			// local variable value
			lValue = lContext.getLocalVariableHandle(mValue_Ref.substr(1))->clone();
			break;
		default:
			// parameter value or direct value
			lValue = mValue->clone();
			break;
	}
	
	std::string lTypeNewValue = lValue->getType();

	if (mVariable_Ref[0] == '@') {
		// individual variable
		Simulation::SimulationContext& lSimulationContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		std::string lTypeVariable = lSimulationContext.getIndividual().getState().getVariable(mVariable_Ref.substr(1)).getType();
		if (lTypeVariable != lTypeNewValue) {
			std::stringstream lOSS;
			lOSS << "The type of variable '" << mVariable_Ref.substr(1) << "' (" << lTypeVariable << ") ";
			lOSS << "does not match the type of argument (" << lTypeNewValue << "); ";
			lOSS << "could not set the variable.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		lContext.getIndividual().getState().setVariable(mVariable_Ref.substr(1), lValue);
	} else { // mVariable_Ref[0] == '%'
		// local variable
		std::string lTypeVariable = lContext.getLocalVariable(mVariable_Ref.substr(1)).getType();
		if (lTypeVariable != lTypeNewValue) {
			std::stringstream lOSS;
			lOSS << "The type of variable '" << mVariable_Ref.substr(1) << "' (" << lTypeVariable << ") ";
			lOSS << "does not match the type of argument (" << lTypeNewValue << "); ";
			lOSS << "could not set the variable.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		lContext.setLocalVariable(mVariable_Ref.substr(1), lValue);
	}
	
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::SetVariable::execute(unsigned int, Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& SetVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetVariable::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
