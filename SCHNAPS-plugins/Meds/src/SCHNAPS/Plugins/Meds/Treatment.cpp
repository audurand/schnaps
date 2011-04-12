/*
 * Treatment.cpp
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

Treatment::Treatment() :
	Primitive(2),
	mLabel(""),
	mOutCost_Ref(""),
	mCompliance_Ref(""),
	mCompliance(NULL),
	mCost_Ref(""),
	mCost(NULL),
	mDiscountRate_Ref(""),
	mDiscountRate(NULL),
	mChargeNonCompliant(true)
{}

Treatment::Treatment(const Treatment& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mOutCost_Ref(inOriginal.mOutCost_Ref.c_str()),
	mCompliance_Ref(inOriginal.mCompliance_Ref.c_str()),
	mCost_Ref(inOriginal.mCost_Ref.c_str()),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str()),
	mChargeNonCompliant(inOriginal.mChargeNonCompliant)
{
	switch (mCompliance_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCompliance = NULL;
			break;
		case '$':
			// parameter value
			mCompliance = inOriginal.mCompliance;
			break;
		default:
			// direct value
			mCompliance = Core::castHandleT<Core::Double>(inOriginal.mCompliance->clone());
			break;
	}
	
	switch (mCost_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCost = NULL;
			break;
		case '$':
			// parameter value
			mCost = inOriginal.mCost;
			break;
		default:
			// direct value
			mCost = Core::castHandleT<Core::Double>(inOriginal.mCost->clone());
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
Treatment& Treatment::operator=(const Treatment& inOriginal) {
	schnaps_StackTraceBeginM();
	mLabel.assign(inOriginal.mLabel.c_str());
	mOutCost_Ref.assign(inOriginal.mOutCost_Ref.c_str());
	mCompliance_Ref.assign(inOriginal.mCompliance_Ref.c_str());
	mCost_Ref.assign(inOriginal.mCost_Ref.c_str());
	mDiscountRate_Ref.assign(inOriginal.mDiscountRate_Ref.c_str());
	mChargeNonCompliant = inOriginal.mChargeNonCompliant;
	
	switch (mCompliance_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCompliance = NULL;
			break;
		case '$':
			// parameter value
			mCompliance = inOriginal.mCompliance;
			break;
		default:
			// direct value
			mCompliance = Core::castHandleT<Core::Double>(inOriginal.mCompliance->clone());
			break;
	}
	
	switch (mCost_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCost = NULL;
			break;
		case '$':
			// parameter value
			mCost = inOriginal.mCost;
			break;
		default:
			// direct value
			mCost = Core::castHandleT<Core::Double>(inOriginal.mCost->clone());
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
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Meds::Treatment& SCHNAPS::Plugins::Meds::Treatment::operator=(const SCHNAPS::Plugins::Meds::Treatment&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel, outCost, inCompliance, inCost, inDiscountRate or inChargeNonCompliant attributes are missing.
 * \throw SCHNAPS::Core::IOException if inChargeNonCompliant attribute value is unknown.
 */
void Treatment::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve output cost destination
	if (inIter->getAttribute("outCost").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "output cost destination expected!");
	}
	mOutCost_Ref.assign(inIter->getAttribute("outCost"));
		
	if (mOutCost_Ref[0] != '@' && mOutCost_Ref[0] != '%') {
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific cost destination source.");
	}
	
	// retrieve compliance
	if (inIter->getAttribute("inCompliance").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "compliance expected!");
	}
	mCompliance_Ref.assign(inIter->getAttribute("inCompliance"));
	
	switch (mCompliance_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCompliance = NULL;
			break;
		case '$':
			// parameter value
			mCompliance = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mCompliance_Ref.substr(1)));
			break;
		default:
			// direct value
			mCompliance = new Core::Double();
			mCompliance->readStr(mCompliance_Ref);
			break;
	}
	
	// retrieve cost
	if (inIter->getAttribute("inCost").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "cost expected!");
	}
	mCost_Ref.assign(inIter->getAttribute("inCost"));
	
	switch (mCost_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mCost = NULL;
			break;
		case '$':
			// parameter value
			mCost = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mCost_Ref.substr(1)));
			break;
		default:
			// direct value
			mCost = new Core::Double();
			mCost->readStr(mCost_Ref);
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
		mDiscountRate = new Core::Double();
		mDiscountRate->readStr(mDiscountRate_Ref);
		break;
	}
	
	// retrieve charge non-compliant flag
	if (inIter->getAttribute("inChargeNonCompliant").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "charge non-compliant flag expected!");
	}
	std::string lChargeNonCompliant = inIter->getAttribute("inChargeNonCompliant");
	
	if (lChargeNonCompliant == "true" || lChargeNonCompliant == "TRUE" || lChargeNonCompliant == "True") {
		mChargeNonCompliant = true;
	} else if (lChargeNonCompliant == "false" || lChargeNonCompliant == "FALSE" || lChargeNonCompliant == "False") {
		mChargeNonCompliant = false;
	} else {
		throw schnaps_IOExceptionNodeM(*inIter, "unknown value for charge non-compliant flag!");
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Treatment::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Treatment::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inLabel", mLabel);
	ioStreamer.insertAttribute("outCost", mOutCost_Ref);
	ioStreamer.insertAttribute("inCompliance", mCompliance_Ref);
	ioStreamer.insertAttribute("inCost", mCost_Ref);
	ioStreamer.insertAttribute("inDiscountRate", mDiscountRate_Ref);
	if (mChargeNonCompliant) {
		ioStreamer.insertAttribute("inChargeNonCompliante", "true");
	} else {
		ioStreamer.insertAttribute("inChargeNonCompliante", "false");
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Treatment::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle Treatment::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	double lCompliance;
	
	switch (mCompliance_Ref[0]) {
		case '@':
			// individual variable value
			lCompliance = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mCompliance_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lCompliance = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mCompliance_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lCompliance = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mCompliance_Ref.substr(1))).getValue();
			break;
		default:
			// parameter value or direct value
			lCompliance = mCompliance->getValue();
			break;
	}
	
	if (ioContext.getRandomizer().rollUniform() <= lCompliance) {
		// individual is compliant
		double lTime = lContext.getClock().getValue();
		double lCost, lCurrentCost, lDiscountRate;

		switch (mCost_Ref[0]) {
			case '@':
				// individual variable value
				lCost = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mCost_Ref.substr(1))).getValue();
				break;
			case '#':
				// environment variable value
				lCost = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mCost_Ref.substr(1))).getValue();
				break;
			case '%':
				// local variable value
				lCost = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mCost_Ref.substr(1))).getValue();
				break;
			default:
				// parameter value or direct value
				lCost = mCost->getValue();
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
		
		// add test cost
		lCost = lCost/std::pow(lDiscountRate + 1, lTime);
		lCurrentCost = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mOutCost_Ref.substr(1))).getValue();
		lContext.getIndividual().getState().setVariable(mOutCost_Ref.substr(1), new Core::Double(lCurrentCost + lCost));
		
		getArgument(inIndex, 0, ioContext);
	} else {
		// individual is not compliant
		if (mChargeNonCompliant) {
			double lTime = lContext.getClock().getValue();
			double lCost, lCurrentCost, lDiscountRate;

			switch (mCost_Ref[0]) {
				case '@':
					// individual variable value
					lCost = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mCost_Ref.substr(1))).getValue();
					break;
				case '#':
					// environment variable value
					lCost = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mCost_Ref.substr(1))).getValue();
					break;
				case '%':
					// local variable value
					lCost = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mCost_Ref.substr(1))).getValue();
					break;
				default:
					// parameter value or direct value
					lCost = mCost->getValue();
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
					lDiscountRate = mDiscountRate->getValue();
					break;
			}
			
			// add test cost
			lCost = lCost/std::pow(lDiscountRate + 1, lTime);
			lCurrentCost = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mOutCost_Ref.substr(1))).getValue();
			lContext.getIndividual().getState().setVariable(mOutCost_Ref.substr(1), new Core::Double(lCurrentCost + lCost));
		}
		
		getArgument(inIndex, 1, ioContext);
	}
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::Treatment::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& Treatment::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Treatment::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Treatment::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Treatment::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
