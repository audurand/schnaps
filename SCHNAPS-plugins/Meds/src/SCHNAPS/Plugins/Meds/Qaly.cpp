/*
 * Qaly.cpp
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

Qaly::Qaly() :
	Primitive(0),
	mOutQaly_Ref(""),
	mOldQaly_Ref(""),
	mQaly_Ref(""),
	mQaly(NULL),
	mDiscountRate_Ref(""),
	mDiscountRate(NULL)
{}

Qaly::Qaly(const Qaly& inOriginal) :
	mOutQaly_Ref(inOriginal.mOutQaly_Ref.c_str()),
	mOldQaly_Ref(inOriginal.mOldQaly_Ref.c_str()),
	mQaly_Ref(inOriginal.mQaly_Ref.c_str()),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str())
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
	
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = inOriginal.mDiscountRate;
			break;
		default:
			// direct value
			mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
			break;
	}

}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Qaly& Qaly::operator=(const Qaly& inOriginal) {
	schnaps_StackTraceBeginM();
	mOutQaly_Ref.assign(inOriginal.mOutQaly_Ref.c_str());
	mOldQaly_Ref.assign(inOriginal.mOldQaly_Ref.c_str());
	mQaly_Ref.assign(inOriginal.mQaly_Ref.c_str());
	mDiscountRate_Ref.assign(inOriginal.mDiscountRate_Ref.c_str());
	
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
	
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = inOriginal.mDiscountRate;
			break;
		default:
			// direct value
			mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
			break;
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Meds::Qaly& SCHNAPS::Plugins::Meds::Qaly::operator=(const SCHNAPS::Plugins::Meds::Qaly&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel, outQaly, inOldQaly, inQaly or inDiscountRate attributes are missing.
 */
void Qaly::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	if (inIter->getAttribute("outQaly").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "output QALY destination expected!");
	}
	mOutQaly_Ref.assign(inIter->getAttribute("outQaly"));
		
	if (mOutQaly_Ref[0] != '@' && mOutQaly_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific qaly destination source.");
	}

	// retrieve old qaly input source
	if (inIter->getAttribute("inOldQaly").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "old QALY input expected!");
	}
	mOldQaly_Ref.assign(inIter->getAttribute("inOldQaly"));
		
	if (mOldQaly_Ref[0] != '@' && mOldQaly_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific old qaly input source.");
	}
	
	// retrieve qaly
	if (inIter->getAttribute("inQaly").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "qaly expected!");
	}
	mQaly_Ref.assign(inIter->getAttribute("inQaly"));
	
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
		
	// retrieve discount rate
	if (inIter->getAttribute("inDiscountRate").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
	}
	mDiscountRate_Ref.assign(inIter->getAttribute("inDiscountRate"));
	
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mDiscountRate_Ref.substr(1)));
			break;
		default:
			// direct value
			mDiscountRate = new Core::Double(SCHNAPS::str2dbl(mDiscountRate_Ref));
			break;
		}

	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Qaly::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Qaly::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outQaly", mOutQaly_Ref);
	ioStreamer.insertAttribute("inOldQaly", mOldQaly_Ref);
	ioStreamer.insertAttribute("inQaly", mQaly_Ref);
	ioStreamer.insertAttribute("inDiscountRate", mDiscountRate_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Qaly::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Qaly::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	double lTime = lContext.getClock().getValue(SCHNAPS::Simulation::Clock::eYear);
	double lQaly, lDiscountRate, lOldQaly, lTotalQaly;
	
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

	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		default:
			// parameter value or direct value
			lDiscountRate = mDiscountRate->getValue();
			break;
	}
	
	lTotalQaly = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mOutQaly_Ref.substr(1))).getValue();
	lOldQaly = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mOldQaly_Ref.substr(1))).getValue();

	if(lOldQaly+lQaly/std::pow(lDiscountRate + 1, lTime) < lTotalQaly) {
		
		lContext.getIndividual().getState().setVariable(mOutQaly_Ref.substr(1), new Core::Double(lOldQaly+lQaly/std::pow(lDiscountRate + 1, lTime)));
	}
	
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::Qaly::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Qaly::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Qaly::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
