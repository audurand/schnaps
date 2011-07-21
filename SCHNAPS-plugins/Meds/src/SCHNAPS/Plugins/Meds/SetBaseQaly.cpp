/*
 * SetBaseQaly.cpp
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

#include "SCHNAPS/Plugins/Meds/Meds.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Meds;

SetBaseQaly::SetBaseQaly() :
	Primitive(0),
	mBaseQaly_Ref(""),
	mQaly_Ref(""),
	mQaly(NULL)
{}

SetBaseQaly::SetBaseQaly(const SetBaseQaly& inOriginal) :
	mBaseQaly_Ref(inOriginal.mBaseQaly_Ref.c_str()),
	mQaly_Ref(inOriginal.mQaly_Ref.c_str())
{
	
	switch (mQaly_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mQaly = NULL;
			break;
		case '$':
			// parameter value
			mQaly = inOriginal.mQaly;
			break;
		default:
			// direct value
			mQaly = Core::castHandleT<Core::Double>(inOriginal.mQaly->clone());
			break;
	}

}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
SetBaseQaly& SetBaseQaly::operator=(const SetBaseQaly& inOriginal) {
	schnaps_StackTraceBeginM();
	mBaseQaly_Ref.assign(inOriginal.mBaseQaly_Ref.c_str());
	mQaly_Ref.assign(inOriginal.mQaly_Ref.c_str());
	
	switch (mQaly_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mQaly = NULL;
			break;
		case '$':
			// parameter value
			mQaly = inOriginal.mQaly;
			break;
		default:
			// direct value
			mQaly = Core::castHandleT<Core::Double>(inOriginal.mQaly->clone());
			break;
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Meds::SetBaseQaly& SCHNAPS::Plugins::Meds::SetBaseQaly::operator=(const SCHNAPS::Plugins::Meds::SetBaseQaly&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if outBaseQaly, inBaseQaly attributes are missing.
 */
void SetBaseQaly::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve output qaly destination
	if (inIter->getAttribute("outBaseQaly").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "output base QALY destination expected!");
	}
	mBaseQaly_Ref.assign(inIter->getAttribute("outBaseQaly"));
		
	if (mBaseQaly_Ref[0] != '@' && mBaseQaly_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific qaly destination source.");
	}
	
	// retrieve new base qaly
	if (inIter->getAttribute("inBaseQaly").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "base qaly expected!");
	}
	mQaly_Ref.assign(inIter->getAttribute("inBaseQaly"));
	
	switch (mQaly_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mQaly = NULL;
			break;
		case '$':
			// parameter value
			mQaly = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mQaly_Ref.substr(1)));
			break;
		default:
			// direct value
			mQaly = new Core::Double(SCHNAPS::str2dbl(mQaly_Ref));
			break;
	}

	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Qaly::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SetBaseQaly::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outBaseQaly", mBaseQaly_Ref);
	ioStreamer.insertAttribute("inBaseQaly", mQaly_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::SetBaseQaly::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle SetBaseQaly::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	double lBaseQaly, lQaly;
	
	switch (mQaly_Ref[0]) {
		case '@':
			// individual variable value
			lQaly = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mQaly_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lQaly = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mQaly_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lQaly = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mQaly_Ref.substr(1))).getValue();
			break;
		default:
			// parameter value or direct value
			lQaly = mQaly->getValue();
			break;
	}
	
	lBaseQaly = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mBaseQaly_Ref.substr(1))).getValue();

	if(lQaly < lBaseQaly) {
		
		lContext.getIndividual().getState().setVariable(mBaseQaly_Ref.substr(1), new Core::Double(lQaly));
	}
	
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::SetBaseQaly::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& SetBaseQaly::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::SetBaseQaly::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
