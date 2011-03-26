/*
 * PreventionCampaign.cpp
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
PreventionCampaign::PreventionCampaign() :
	Primitive(0),
	mOutCost_Ref(""),
	mCost_Ref(""),
	mCost(NULL),
	mDiscountRate_Ref(""),
	mDiscountRate(NULL)
{}

/*!
 * \brief Construct a medical prevention campaign as a copy of an original.
 * \param inOriginal A const reference to the original medical prevention campaign.
 */
PreventionCampaign::PreventionCampaign(const PreventionCampaign& inOriginal) :
	Primitive(0),
	mOutCost_Ref(inOriginal.mOutCost_Ref.c_str()),
	mCost_Ref(inOriginal.mCost_Ref.c_str()),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str())
{
	switch (mCost_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
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
			// TODO: local variable value
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
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if outCost, inCost or inDiscountRate attributes are missing.
 */
void PreventionCampaign::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

		// retrieve cost output variable
		if (inIter->getAttribute("outCost").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "output cost destination expected!");
		}
		mOutCost_Ref.assign(inIter->getAttribute("outCost"));
		
		if (mOutCost_Ref[0] != '@') {
			// TODO: local variable value
			throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific cost destination source.");
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
			// TODO: local variable value
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
			// TODO: local variable value
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::PreventionCampaign::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void PreventionCampaign::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outCost", mOutCost_Ref);
	ioStreamer.insertAttribute("inCost", mCost_Ref);
	ioStreamer.insertAttribute("inDiscountRate", mDiscountRate_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::PreventionCampaign::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the primitive is not defined for the specific context.
 * \throw  SCHNAPS::Core::RunTimeException if the method is undefined for the specific cost source.
 * \throw  SCHNAPS::Core::RunTimeException if the method is undefined for the specific discount rate source.
 */
Core::AnyType::Handle PreventionCampaign::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (ioContext.getName() == "SimulationContext") {
			Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
			double lTime = lContext.getClock().getValue();
			double lCost, lCurrentCost, lDiscountRate;

			if (mCost == NULL) {
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
						// TODO: local variable value
						break;
					default:
						throw schnaps_RunTimeExceptionM("The method is undefined for the specific cost source.");
						break;
				}
			} else {
				lCost = mCost->getValue();
			}

			if (mDiscountRate == NULL) {
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
						// TODO: local variable value
						break;
					default:
						throw schnaps_RunTimeExceptionM("The method is undefined for the specific discount rate source.");
						break;
				}
			} else {
				lDiscountRate = mDiscountRate->getValue();
			}
			
			lCost = lCost/std::pow(lDiscountRate + 1, lTime);
			lCurrentCost = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mOutCost_Ref.substr(1))).getValue();
			lContext.getIndividual().getState().setVariable(mOutCost_Ref.substr(1), new Core::Double(lCurrentCost + lCost));
		} else {
			throw schnaps_RunTimeExceptionM("Prevention campaign primitive is not defined for context '" + ioContext.getName() + "'!");
		}
		return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Meds::PreventionCampaign::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& PreventionCampaign::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::PreventionCampaign::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
