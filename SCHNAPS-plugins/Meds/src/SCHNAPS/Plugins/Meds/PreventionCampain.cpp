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

#include "SCHNAPS/Plugins/Meds/Meds.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Meds;

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
	mCost = Core::castHandleT<Core::Double>(inOriginal.mCost->clone());
	mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
}

void PreventionCampain::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

		// retrieve cost
		if (inIter->getAttribute("cost").empty()) {
			if (inIter->getAttribute("cost.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "cost expected!");
			} else {
				mCost_Ref = inIter->getAttribute("cost.ref");
				std::stringstream lSS;
				lSS << "ref." << mCost_Ref;
				mCost = Core::castHandleT<Core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
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
				mDiscountRate = Core::castHandleT<Core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			mDiscountRate = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("discountRate")));
		}

		// retrieve label of cost variable
		if (inIter->getAttribute("costVariableLabel").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of cost variable to refer expected!");
		}
		mCostVariableLabel = inIter->getAttribute("costVariableLabel");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::PreventionCampain::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::PreventionCampain::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle PreventionCampain::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (ioContext.getName() == "SimulationContext") {
			Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
			double lTime = lContext.getClock().getValue();

			// Compute cost
			Core::Double lDenum(std::pow(mDiscountRate->getValue() + 1, lTime));
			Core::Double::Handle lCost = Core::castHandleT<Core::Double>(mCost->clone());
			lCost->div(lDenum);

			// Set cost
			Simulation::State::iterator lStateIt = lContext.getIndividual().getState().find(mCostVariableLabel);
			if (lStateIt == lContext.getIndividual().getState().end()) {
				throw schnaps_InternalExceptionM("PreventionCampain cost variable '" + mCostVariableLabel + "' does not refer to a state variable.");
			}
			Core::castHandleT<Core::Double>(lStateIt->second)->add(*lCost);
		} else {
			throw schnaps_InternalExceptionM("Prevention campain primitive is not defined for context '" + ioContext.getName() + "'!");
		}
		return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Meds::PreventionCampain::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& PreventionCampain::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::PreventionCampain::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
