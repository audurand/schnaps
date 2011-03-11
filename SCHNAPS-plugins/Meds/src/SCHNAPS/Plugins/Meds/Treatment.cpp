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
	mCompliance(NULL),
	mCost(NULL),
	mDiscountRate(NULL),
	mCompliance_Ref(""),
	mCost_Ref(""),
	mDiscountRate_Ref(""),
	mCostVariableLabel("")
{}

Treatment::Treatment(const Treatment& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mCompliance_Ref(inOriginal.mCompliance_Ref.c_str()),
	mCost_Ref(inOriginal.mCost_Ref.c_str()),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str()),
	mCostVariableLabel(inOriginal.mCostVariableLabel.c_str())
{
	mCompliance = Core::castHandleT<Core::Double>(inOriginal.mCompliance->clone());
	mCost = Core::castHandleT<Core::Double>(inOriginal.mCost->clone());
	mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 * \throw SCHNAPS::Core::IOException if compliance attribute and compliance.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if cost attribute and cost.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if discountRate attribute and discountRate.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if costLabelVariable attribute is missing.
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
	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label expected!");
	} else {
		mLabel = inIter->getAttribute("label");
	}

	// retrieve compliance
	if (inIter->getAttribute("compliance").empty()) {
		if (inIter->getAttribute("compliance.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "compliance expected!");
		} else {
			mCompliance_Ref = inIter->getAttribute("ref.compliance");
			std::stringstream lSS;
			lSS << "ref." << mCompliance_Ref;
			mCompliance = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mCompliance = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("compliance")));
	}

	// retrieve cost
	if (inIter->getAttribute("cost").empty()) {
		if (inIter->getAttribute("cost.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "cost expected!");
		} else {
			mCost_Ref = inIter->getAttribute("cost.ref");
			std::stringstream lSS;
			lSS << "ref." << mCost_Ref;
			mCost = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mCost = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("cost")));
	}

	// retrieve discount rate
	if (inIter->getAttribute("discountRate").empty()) {
		if (inIter->getAttribute("discountRate.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
		} else {
			mDiscountRate_Ref = inIter->getAttribute("discountRate.ref");
			std::stringstream lSS;
			lSS << "ref." << mDiscountRate_Ref;
			mDiscountRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mDiscountRate = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("discountRate")));
	}

	// retrieve label of cost variable
	if (inIter->getAttribute("costVariableLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of cost variable to refer expected!");
	}
	mCostVariableLabel = inIter->getAttribute("costVariableLabel");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Treatment::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Treatment::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	if (mCompliance_Ref.empty()) {
		ioStreamer.insertAttribute("compliance.ref", "");
		ioStreamer.insertAttribute("compliance", mCompliance->writeStr());
	} else {
		ioStreamer.insertAttribute("compliance.ref", mCompliance_Ref);
	}
	if (mCost_Ref.empty()) {
		ioStreamer.insertAttribute("cost.ref", "");
		ioStreamer.insertAttribute("cost", mCost->writeStr());
	} else {
		ioStreamer.insertAttribute("cost.ref", mCost_Ref);
	}
	if (mDiscountRate_Ref.empty()) {
		ioStreamer.insertAttribute("discountRate.ref", "");
		ioStreamer.insertAttribute("discountRate", mDiscountRate->writeStr());
	} else {
		ioStreamer.insertAttribute("discountRate.ref", mDiscountRate_Ref);
	}
	ioStreamer.insertAttribute("costVariableLabel", mCostVariableLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Treatment::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the primitive is not defined for the specific context.
 */
Core::AnyType::Handle Treatment::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (ioContext.getName() == "SimulationContext") {
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		double lTime = lContext.getClock().getValue();

		// compute cost
		Core::Double lDenum(std::pow(mDiscountRate->getValue() + 1, lTime));
		Core::Double::Handle lCost = Core::castHandleT<Core::Double>(mCost->clone());
		lCost->div(lDenum);

		// set cost
		Core::Double::Handle lNewValue = Core::castHandleT<Core::Double>(lContext.getIndividual().getState().getVariableHandle(mCostVariableLabel)->clone());
		lNewValue->add(*lCost);
		lContext.getIndividual().getState().setVariable(mCostVariableLabel, lNewValue);

		if (ioContext.getRandomizer().rollUniform() < mCompliance->getValue()) {
			// apply treatment effects
			getArgument(inIndex, 0, ioContext);
		} else { // not compliant
			getArgument(inIndex, 1, ioContext);
		}

	} else {
		throw schnaps_RunTimeExceptionM("Primitive is not defined for context '" + ioContext.getName() + "'!");
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
