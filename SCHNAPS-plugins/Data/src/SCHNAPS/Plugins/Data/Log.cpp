/*
 * Log.cpp
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
Log::Log() :
	Primitive(0),
	mType_Ref(""),
	mType(NULL),
	mMessage_Ref(""),
	mMessage(NULL)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inType or inMessage attributes are missing.
 */
void Log::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	
	// retrieve log type
	if (inIter->getAttribute("inType").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "log type expected!");
	}
	mType_Ref.assign(inIter->getAttribute("inType"));
	
	switch (mType_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mType = NULL;
			break;
		case '$':
			// parameter value
			mType = ioSystem.getParameters().getParameterHandle(mType_Ref.substr(1));
			break;
		default:
			// direct value
			mType = new Core::String(mType_Ref);
			break;
	}

	// retrieve log message
	if (inIter->getAttribute("inMessage").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "log message expected!");
	}
	mMessage_Ref.assign(inIter->getAttribute("inMessage"));
	
	switch (mMessage_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mMessage = NULL;
			break;
		case '$':
			// parameter value
			mMessage = ioSystem.getParameters().getParameterHandle(mMessage_Ref.substr(1));
			break;
		default:
			// direct value
			mMessage = new Core::String(mMessage_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Log::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Log::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inType", mType_Ref);
	ioStreamer.insertAttribute("inMessage", mMessage_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Log::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific type source.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific message source.
 */
Core::AnyType::Handle Log::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	std::string lType, lMessage;
	
	if (mType == NULL) {
		switch (mType_Ref[0]) {
			case '@':
				// individual variable value
				lType = lContext.getIndividual().getState().getVariableHandle(mType_Ref.substr(1))->writeStr();
				break;
			case '#':
				// environment variable value
				lType = lContext.getEnvironment().getState().getVariableHandle(mType_Ref.substr(1))->writeStr();
				break;
			case '%':
				// local variable value
				lType = lContext.getLocalVariableHandle(mType_Ref.substr(1))->writeStr();
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specific type source.");
				break;
		}
	} else {
		// parameter value or direct value
		lType = mType->writeStr();
	}
	
	if (mMessage == NULL) {
		switch (mMessage_Ref[0]) {
			case '@':
				// individual variable value
				lMessage = lContext.getIndividual().getState().getVariableHandle(mMessage_Ref.substr(1))->writeStr();
				break;
			case '#':
				// environment variable value
				lMessage = lContext.getEnvironment().getState().getVariableHandle(mMessage_Ref.substr(1))->writeStr();
				break;
			case '%':
				// local variable value
				lMessage = lContext.getLocalVariableHandle(mMessage_Ref.substr(1))->writeStr();
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specific message source.");
				break;
		}
	} else {
		// parameter value or direct value
		lMessage = mMessage->writeStr();
	}
	
	std::stringstream lLog;
	lLog << lContext.getIndividual().getID() << "," << lContext.getClock().getValue() << "," << lType << "," << "\"" << lMessage << "\"";
	lContext.getLogger().log(lLog.str());
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::Log::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Log::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& Log::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext& ioContext) const");
}
