/*
 * SubVariable.cpp
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
SubVariable::SubVariable() :
	Core::Primitive(0),
	mResult_Ref(""),
	mArgLeft_Ref(""),
	mArgLeft(NULL),
	mArgRight_Ref(""),
	mArgRight(NULL)
{}

/*!
 * \brief Construct an operator to subtract right argument from left argument and store result in a variable as a copy of an original.
 * \param inOriginal A const reference to the original operator to subtract right argument from left argument and store result in a variable.
 */
SubVariable::SubVariable(const SubVariable& inOriginal) :
	Core::Primitive(0),
	mResult_Ref(inOriginal.mResult_Ref.c_str()),
	mArgLeft_Ref(inOriginal.mArgLeft_Ref.c_str()),
	mArgRight_Ref(inOriginal.mArgRight_Ref.c_str())
{
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
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if outResult or inArgLeft or inArgRight attributes are missing.
 * \throw SCHNAPS::Core::IOException if inArgLeft attribute is a direct value and inArgLeft_Type attribute is missing.
 * \throw SCHNAPS::Core::IOException if inArgRight attribute is a direct value and inArgRight_Type attribute is missing.
 * \throw SCHNAPS::Core::RunTimeException if outResult attribute is not a reference to a variable.
 */
void SubVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	if (inIter->getAttribute("outResult").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of variable where to store result expected!");
	}
	mResult_Ref.assign(inIter->getAttribute("outResult"));
	
	if (mResult_Ref[0] != '@' && mResult_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific output result source!");
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
			Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inArgRight_Type")));
			mArgRight = Core::castHandleT<Core::Number>(lAlloc->allocate());
			mArgRight->readStr(mArgRight_Ref);
			break; }
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::SubVariable::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SubVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outResult", mResult_Ref);
	ioStreamer.insertAttribute("inArgLeft", mArgLeft_Ref);
	ioStreamer.insertAttribute("inArgRight", mArgRight_Ref);
	
	if (mArgLeft != NULL & mArgLeft_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inArgLeft_Type", mArgLeft->getType());
	}
	if (mArgRight != NULL & mArgRight_Ref[0] != '$') {
		// direct value
		ioStreamer.insertAttribute("inArgRight_Type", mArgRight->getType());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::SubVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific left argument source.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific right argument source.
 */
Core::AnyType::Handle SubVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	Core::Number::Handle lArgLeft, lArgRight;
	
	if (mArgLeft == NULL) {
		switch (mArgLeft_Ref[0]) {
			case '@':
				// individual variable value
				lArgLeft = Core::castHandleT<Core::Number>(lContext.getIndividual().getState().getVariableHandle(mArgLeft_Ref.substr(1)));
				break;
			case '#':
				// environment variable value
				lArgLeft = Core::castHandleT<Core::Number>(lContext.getEnvironment().getState().getVariableHandle(mArgLeft_Ref.substr(1))->clone());
				break;
			case '%':
				// local variable value
				lArgLeft = Core::castHandleT<Core::Number>(lContext.getLocalVariableHandle(mArgLeft_Ref.substr(1)));
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specific left argument source.");
				break;
		}
	} else {
		// parameter value or direct value
		lArgLeft = Core::castHandleT<Core::Number>(mArgLeft->clone());
	}
	
	if (mArgRight == NULL) {
		switch (mArgRight_Ref[0]) {
			case '@':
				// individual variable value
				lArgRight = Core::castHandleT<Core::Number>(lContext.getIndividual().getState().getVariableHandle(mArgRight_Ref.substr(1)));
				break;
			case '#':
				// environment variable value
				lArgRight = Core::castHandleT<Core::Number>(lContext.getEnvironment().getState().getVariableHandle(mArgRight_Ref.substr(1))->clone());
				break;
			case '%':
				// local variable value
				lArgRight = Core::castHandleT<Core::Number>(lContext.getLocalVariableHandle(mArgRight_Ref.substr(1)));
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specific right argument source.");
				break;
		}
	} else {
		// parameter value or direct value
		lArgRight = Core::castHandleT<Core::Number>(mArgRight->clone());
	}
	
	if (mResult_Ref[0] == '@') {
		// individual variable
		Simulation::SimulationContext& lSimulationContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		lSimulationContext.getIndividual().getState().setVariable(mResult_Ref.substr(1), lArgLeft->sub(*lArgRight));
	} else { // mResult_Ref[0] == '%'
		// local variable
		lContext.setLocalVariable(mResult_Ref.substr(1), lArgLeft->sub(*lArgRight));
	}
	
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::SubVariable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& SubVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::SubVariable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
