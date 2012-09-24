/*
 * Transmission.cpp
 *
 * SCHNAPS
 * Copyright (C) 2011 by Xavier Douville
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

#include "SCHNAPS/Plugins/Contacts/Contacts.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Contacts;

Transmission::Transmission() :
	Primitive(0),
	mLabel(""),
	mContacts_Ref(""),
	mContacts(NULL),
	mProbability_Ref(""),
	mProbability(NULL)
{}

Transmission::Transmission(const Transmission& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mProbability_Ref(inOriginal.mProbability_Ref.c_str()),
	mContacts_Ref(inOriginal.mContacts_Ref.c_str())
{
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
			mProbability = inOriginal.mProbability;
			break;
		default:
			// direct value
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
			break;
	}
	
	switch (mContacts_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mContacts = NULL;
			break;
		case '$':
			// parameter value
			mContacts = inOriginal.mContacts;
			break;
		default:
			// direct value
			mContacts = Core::castHandleT<Core::Vector>(inOriginal.mContacts->clone());
			break;
	}
	
	setNumberArguments(inOriginal.getNumberArguments());
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Transmission& Transmission::operator=(const Transmission& inOriginal) {
	schnaps_StackTraceBeginM();
	mLabel.assign(inOriginal.mLabel.c_str());
	mProbability_Ref.assign(inOriginal.mProbability_Ref.c_str());
	mContacts_Ref.assign(inOriginal.mContacts_Ref.c_str());
	
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
			mProbability = inOriginal.mProbability;
			break;
		default:
			// direct value
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
			break;
	}
	
	switch (mContacts_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mContacts = NULL;
			break;
		case '$':
			// parameter value
			mContacts = inOriginal.mContacts;
			break;
		default:
			// direct value
			mContacts = Core::castHandleT<Core::Vector>(inOriginal.mContacts->clone());
			break;
	}
	
	setNumberArguments(inOriginal.getNumberArguments());

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Contacts::Transmission& SCHNAPS::Plugins::Contacts::Transmission::operator=(const SCHNAPS::Plugins::Contacts::Transmission&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel, outCost, inCompliance, inCost, inDiscountRate or inChargeNonCompliant attributes are missing.
 * \throw SCHNAPS::Core::IOException if inChargeNonCompliant attribute value is unknown.
 */
void Transmission::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	//std::cout << mLabel << std::endl;
	
	// retrieve probability of infection
	if (inIter->getAttribute("inProbability").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "probability of executing first branch expected!");
	}
	mProbability_Ref = inIter->getAttribute("inProbability");
	
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
	
	// retrieve contacts
	if (inIter->getAttribute("inContacts").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "contacts of individual expected!");
	}
	mContacts_Ref.assign(inIter->getAttribute("inContacts"));
	
	switch (mContacts_Ref[0]) {
		case '@':
			// individual variable value
			mContacts = NULL;
			break;
		case '#':
			// environment variable value
		case '%':
			// local variable value
			throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific contact source!");
			break;
		case '$':
			// parameter value
			mContacts = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(mContacts_Ref.substr(1)));
			break;
		default: {
			// direct value
			mContacts = NULL;
			break; }
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Contacts::Transmission::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Transmission::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inLabel", mLabel);
	ioStreamer.insertAttribute("inProbability", mProbability_Ref);
	ioStreamer.insertAttribute("inContacts", mContacts_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Contacts::Transmission::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Transmission::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);

	
	
	double lProbability;
	unsigned int lIndividual;
	
	Core::Vector::Handle lContacts;

	
	switch (mContacts_Ref[0]) {
		case '@':
			// individual variable value
			lContacts = Core::castHandleT<Core::Vector>(lContext.getIndividual().getState().getVariableHandle(mContacts_Ref.substr(1)));
			break;
		case '#':
			// environment variable value
			lContacts = Core::castHandleT<Core::Vector>(lContext.getIndividual().getState().getVariableHandle(mContacts_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lContacts = Core::castHandleT<Core::Vector>(lContext.getLocalVariableHandle(mContacts_Ref.substr(1)));
			break;
		case '$':
			// parameter value
			lContacts = Core::castHandleT<Core::Vector>(mContacts->clone());
			break;
		default:
			// direct value
			//do not contain a direct contact list, but a string refering to an individual variable to be created by the generator.
			lContacts = Core::castHandleT<Core::Vector>(lContext.getIndividual().getState().getVariableHandle(mContacts_Ref));
			break;
	}
	
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mProbability_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mProbability_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lProbability = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mProbability_Ref.substr(1))).getValue();
			break;
		default:
			lProbability = mProbability->getValue();
			break;
	}
	unsigned long lDelay=1;
	unsigned long lDelta=1;
	unsigned long lStartValue = lContext.getClock().getValue(Simulation::Clock::eOther) + lDelay;
	for (unsigned int i = 0; i < lContacts->size(); i++) {
		double lRandom = ioContext.getRandomizer().rollUniform();
		if (lRandom < lProbability) {
			lIndividual = Core::castHandleT<Core::UInt>((*lContacts)[i])->getValue();
			lContext.getPushList().push_back(Simulation::Push(mLabel, Simulation::Process::eIndividualByID, lContext.getClock().getTick(lStartValue, Simulation::Clock::eOther),lIndividual));		
		}
		
	}	

	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Contacts::Transmission::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Transmission::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Contacts::Transmission::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
