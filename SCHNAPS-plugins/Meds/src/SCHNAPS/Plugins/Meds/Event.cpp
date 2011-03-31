/*
 * Event.cpp
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

/*!
 * \brief Default constructor.
 */
Event::Event() :
	Primitive(2),
	mLabel(""),
	mProbability_Ref(""),
	mProbability(NULL)
{}

/*!
 * \brief Construct a medical event as a copy of an original.
 * \param inOriginal A const reference to the original medical event.
 */
Event::Event(const Event& inOriginal) :
	Primitive(2),
	mLabel(inOriginal.mLabel.c_str()),
	mProbability_Ref(inOriginal.mProbability_Ref.c_str())
{
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbability = NULL;
			break;
		case '$':
			// parameter value
			mProbability = inOriginal.mProbability;
			break;
		default:
			// direct value
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
			break;
	}
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel or inProbability attributes are missing.
 */
void Event::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	if (inIter->getAttribute("inLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of event expected!");
	}
	mLabel.assign(inIter->getAttribute("inLabel"));

	// retrieve event probability
	if (inIter->getAttribute("inProbability").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "event probability expected!");
	}
	mProbability_Ref.assign(inIter->getAttribute("inProbability"));
	
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mProbability = NULL;
			break;
		case '$':
			// parameter value
			mProbability = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mProbability_Ref.substr(1)));
			break;
		default:
			// direct value
			mProbability = new Core::Double();
			mProbability->readStr(mProbability_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Event::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Event::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inLabel", mLabel);
	ioStreamer.insertAttribute("inProbability", mProbability_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Event::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Event::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	double lProbability;
	
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mProbability_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mProbability_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mProbability_Ref.substr(1))).getValue();
			break;
		default:
			// parameter value or direct value
			lProbability = mProbability->getValue();
			break;
	}
	
	if (ioContext.getRandomizer().rollUniform() <= lProbability) { // event
		getArgument(inIndex, 0, ioContext);
	} else { // no event
		getArgument(inIndex, 1, ioContext);
	}
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::Event::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Event::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Event::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Event::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Event::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
