/*
 * PreventionCampain.cpp
 *
 *  Created on: 2010-11-02
 *  Author: Audrey Durand
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

#include "SCHNAPS-plugins/Osteoporosis/Osteoporosis.hpp"

using namespace osteoporosis;

PreventionCampain::PreventionCampain() :
		Primitive(0),
		mCost(NULL),
		mDiscountRate(NULL),
		mCost_Ref(""),
		mDiscountRate_Ref(""),
		mCostVariableLabel("")
{}

PreventionCampain::PreventionCampain(const PreventionCampain& inOriginal) :
		Primitive(0),
		mCost_Ref(inOriginal.mCost_Ref.c_str()),
		mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str()),
		mCostVariableLabel(inOriginal.mCostVariableLabel.c_str())
{
	mCost = core::castHandleT<core::Double>(inOriginal.mCost->clone());
	mDiscountRate = core::castHandleT<core::Double>(inOriginal.mDiscountRate->clone());
}

void PreventionCampain::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

		// Retrieve cost
		if (inIter->getAttribute("cost").empty()) {
			if (inIter->getAttribute("cost.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "cost expected!");
			} else {
				mCost_Ref = inIter->getAttribute("cost.ref");
				std::stringstream lSS;
				lSS << "ref." << mCost_Ref;
				mCost = core::castHandleT<core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			mCost = new core::Double(core::str2dbl(inIter->getAttribute("cost")));
		}

		// Retrieve discount rate
		if (inIter->getAttribute("discountRate").empty()) {
			if (inIter->getAttribute("discountRate.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
			} else {
				mDiscountRate_Ref = inIter->getAttribute("discountRate.ref");
				std::stringstream lSS;
				lSS << "ref." << mDiscountRate_Ref;
				mDiscountRate = core::castHandleT<core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			mDiscountRate = new core::Double(core::str2dbl(inIter->getAttribute("discountRate")));
		}

		// Retrieve label of cost variable
		if (inIter->getAttribute("costVariableLabel").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of cost variable to refer expected!");
		}
		mCostVariableLabel = inIter->getAttribute("costVariableLabel");
	schnaps_StackTraceEndM("void osteoporosis::PreventionCampain::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void PreventionCampain::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
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
	schnaps_StackTraceEndM("void osteoporosis::PreventionCampain::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle PreventionCampain::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (ioContext.getName() == "SimulationContext") {
			simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
			double lTime = lContext.getClock().getValue();

			// Compute cost
			core::Double lDenum(std::pow(mDiscountRate->getValue() + 1, lTime));
			core::Double::Handle lCost = core::castHandleT<core::Double>(mCost->clone());
			lCost->div(lDenum);

			// Set cost
			simulator::State::iterator lStateIt = lContext.getIndividual().getState().find(mCostVariableLabel);
			if (lStateIt == lContext.getIndividual().getState().end()) {
				throw schnaps_InternalExceptionM("PreventionCampain cost variable '" + mCostVariableLabel + "' does not refer to a state variable.");
			}
			core::castHandleT<core::Double>(lStateIt->second)->add(*lCost);
		} else {
			throw schnaps_InternalExceptionM("Prevention campain primitive is not defined for context '" + ioContext.getName() + "'!");
		}
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle osteoporosis::PreventionCampain::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& PreventionCampain::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& osteoporosis::PreventionCampain::getReturnType(unsigned int, core::ExecutionContext&) const");
}
