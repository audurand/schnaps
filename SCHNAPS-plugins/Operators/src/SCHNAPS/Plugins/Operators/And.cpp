/*
 * And.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2014 by Audrey Durand
 *
 * This program is free software: you can redistribute it And/or modify
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
And::And() :
	Core::Primitive(0),
	mArgLeft_Ref(""),
	mArgLeft(NULL),
	mArgRight_Ref(""),
	mArgRight(NULL)
{}

/*!
 * \brief Construct logical AND primitive as a copy of an original.
 * \param inOriginal A const reference to the original logical AND primitive.
 */
And::And(const And& inOriginal) :
	Core::Primitive(0),
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
			mArgLeft = Core::castHandleT<Core::Bool>(inOriginal.mArgLeft->clone());
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
			mArgRight = Core::castHandleT<Core::Bool>(inOriginal.mArgRight->clone());
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
And& And::operator=(const And& inOriginal) {
	schnaps_StackTraceBeginM();
	mArgLeft_Ref.assign(inOriginal.mArgLeft_Ref.c_str());
	mArgRight_Ref.assign(inOriginal.mArgRight_Ref.c_str());
	
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
			mArgLeft = Core::castHandleT<Core::Bool>(inOriginal.mArgLeft->clone());
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
			mArgRight = Core::castHandleT<Core::Bool>(inOriginal.mArgRight->clone());
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::And& SCHNAPS::Plugins::Operators::And::operator=(const SCHNAPS::Plugins::Operators::And&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inArgLeft or inArgRight attributes are missing.
 */
void And::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
			mArgLeft = Core::castHandleT<Core::Bool>(ioSystem.getParameters().getParameterHandle(mArgLeft_Ref.substr(1)));
			break;
		default:
			// direct value
			mArgLeft = new Core::Bool();
			mArgLeft->readStr(mArgLeft_Ref);
			break;
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
			mArgRight = Core::castHandleT<Core::Bool>(ioSystem.getParameters().getParameterHandle(mArgRight_Ref.substr(1)));
			break;
		default:
			// direct value
			mArgRight = new Core::Bool();
			mArgRight->readStr(mArgRight_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::And::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void And::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inArgLeft", mArgLeft_Ref);
	ioStreamer.insertAttribute("inArgRight", mArgRight_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::And::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A Handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the primitive is not defined for the specific left argument source.
 * \throw  SCHNAPS::Core::RunTimeException if the primitive is not defined for the specific right argument source.
 */
Core::AnyType::Handle And::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	bool lArgLeft, lArgRight;
	
	if (mArgLeft == NULL) {
		switch (mArgLeft_Ref[0]) {
			case '@':
				// individual variable value
				lArgLeft = Core::castObjectT<const Core::Bool&>(lContext.getIndividual().getState().getVariable(mArgLeft_Ref.substr(1))).getValue();
				break;
			case '#':
				// environment variable value
				lArgLeft = Core::castObjectT<const Core::Bool&>(lContext.getEnvironment().getState().getVariable(mArgLeft_Ref.substr(1))).getValue();
				break;
			case '%':
				// local variable value
				lArgLeft = Core::castObjectT<const Core::Bool&>(lContext.getLocalVariable(mArgLeft_Ref.substr(1))).getValue();
				break;
			default:
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific left argument source.");
				break;
		}
	} else {
		// parameter value or direct value
		lArgLeft = mArgLeft->getValue();
	}
	
	if (mArgRight == NULL) {
		switch (mArgRight_Ref[0]) {
			case '@':
				// individual variable value
				lArgRight = Core::castObjectT<const Core::Bool&>(lContext.getIndividual().getState().getVariable(mArgRight_Ref.substr(1))).getValue();
				break;
			case '#':
				// environment variable value
				lArgRight = Core::castObjectT<const Core::Bool&>(lContext.getEnvironment().getState().getVariable(mArgRight_Ref.substr(1))).getValue();
				break;
			case '%':
				// local variable value
				lArgRight = Core::castObjectT<const Core::Bool&>(lContext.getLocalVariable(mArgRight_Ref.substr(1))).getValue();
				break;
			default:
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific right argument source.");
				break;
		}
	} else {
		// parameter value or direct value
		lArgRight = mArgRight->getValue();
	}
	
	return new Core::Bool(lArgLeft && lArgRight);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::And::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& And::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Core::Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::And::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
