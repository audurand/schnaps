/*
 * ConcatVariable.cpp
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
ConcatVariable::ConcatVariable() :
	Core::Primitive(0),
	mLabel(""),
	mValue_Ref(""),
	mValue(NULL)
{}

/*!
 * \brief Construct a primitive for string concatenation to a current individual variable as a copy of an original.
 * \param inOriginal A const reference to the original primitive for string concatenation to a current individual variable.
 */
ConcatVariable::ConcatVariable(const ConcatVariable& inOriginal) :
	Core::Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mValue_Ref(inOriginal.mValue_Ref.c_str())
{
	if (mValue_Ref.empty()) {
		mValue = Core::castHandleT<Core::String>(inOriginal.mValue->clone());
	} else {
		mValue = Core::castHandleT<Core::String>(inOriginal.mValue);
	}
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 * \throw SCHNAPS::Core::IOException if value attribute and value.ref attriute are missing.
 */
void ConcatVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve label
	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of variable to set expected!");
	}
	mLabel = inIter->getAttribute("label");

	// retrieve value
	if (inIter->getAttribute("value").empty()) {
		if (inIter->getAttribute("value.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "comparison value expected!");
		} else { // from parameter
			mValue_Ref = inIter->getAttribute("value.ref");

			std::stringstream lSS;
			lSS << "ref." << mValue_Ref;
			mValue = Core::castHandleT<Core::String>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else { // explicitly given
		mValue =  new Core::String();
		mValue->readStr(inIter->getAttribute("value"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::ConcatVariable::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ConcatVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	if (mValue_Ref.empty()) {
		ioStreamer.insertAttribute("valueType", mValue->getName());
		ioStreamer.insertAttribute("value", mValue->writeStr());
	} else {
		ioStreamer.insertAttribute("value.ref", mValue_Ref);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::ConcatVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific execution context.
 */
Core::AnyType::Handle ConcatVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (ioContext.getName() == "GenerationContext") {
		throw schnaps_RunTimeExceptionM("The method is not defined for context 'GenerationContext'.");
	}
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);

	std::stringstream lSS;
	lSS << Core::castHandleT<Core::String>(lContext.getIndividual().getState().getVariableHandle(mLabel))->getValue() << mValue->getValue();
	lContext.getIndividual().getState().setVariable(mLabel, new Core::String(lSS.str()));
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::ConcatVariable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ConcatVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::ConcatVariable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
