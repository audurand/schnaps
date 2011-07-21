/*
 * Serialize.cpp
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
Serialize::Serialize() :
	Core::Primitive(0),
	mValue_Ref(""),
	mValue(NULL)
{}

/*!
 * \brief Construct an operator to serialize value to string as a copy of original.
 * \param inOriginal A const reference to the original operator to serialize value to string.
 */
Serialize::Serialize(const Serialize& inOriginal) :
	Core::Primitive(0),
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
			mValue = Core::castHandleT<Core::String>(inOriginal.mValue->clone());
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Serialize& Serialize::operator=(const Serialize& inOriginal) {
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
			mValue = Core::castHandleT<Core::String>(inOriginal.mValue->clone());
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::Serialize& SCHNAPS::Plugins::Operators::Serialize::operator=(const SCHNAPS::Plugins::Operators::Serialize&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inArg attribute is missing.
 */
void Serialize::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve argument
	if (inIter->getAttribute("inValue").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "argument expected!");
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
		default:
			// direct value
			mValue = new Core::String(mValue_Ref.substr(1));
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::Serialize::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Serialize::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::Serialize::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Serialize::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	std::string lValue;
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
			lValue = lContext.getIndividual().getState().getVariableHandle(mValue_Ref.substr(1))->writeStr();
			break;
		case '#':
			// environment variable value
			lValue = lContext.getEnvironment().getState().getVariableHandle(mValue_Ref.substr(1))->writeStr();
			break;
		case '%':
			// local variable value
			lValue = lContext.getLocalVariableHandle(mValue_Ref.substr(1))->writeStr();
			break;
		default:
			// parameter value or direct value
			lValue = mValue->writeStr();
			break;
	}
	
	return new Core::String(lValue);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::Serialize::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Serialize::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("String");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Serialize::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
