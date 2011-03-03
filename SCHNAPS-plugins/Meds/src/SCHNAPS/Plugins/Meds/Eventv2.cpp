/*
 * Eventv2.cpp
 *
 *  Created on: 2010-12-02
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

Eventv2::Eventv2() :
	Primitive(2),
	mLabel(""),
	mProbabilityVariableLabel("")
{}

Eventv2::Eventv2(const Eventv2& inOriginal) :
	Primitive(2),
	mLabel(inOriginal.mLabel.c_str()),
	mProbabilityVariableLabel(inOriginal.mProbabilityVariableLabel.c_str())
{}

void Eventv2::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
		throw schnaps_IOExceptionNodeM(*inIter, "label of Eventv2 expected!");
	}
	mLabel = inIter->getAttribute("label");

	// retrieve label of individual variable associated to Eventv2 probability
	if (inIter->getAttribute("probabilityVariableLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of individual variable associated to Eventv2 probability expected!");
	}
	mProbabilityVariableLabel = inIter->getAttribute("probabilityVariableLabel");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Eventv2::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void Eventv2::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	ioStreamer.insertAttribute("probabilityVariableLabel", mProbabilityVariableLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Eventv2::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle Eventv2::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (ioContext.getName() == "SimulationContext") {
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);

		// get event probability for the individual
		Simulation::State::iterator lStateIt = lContext.getIndividual().getState().find(mProbabilityVariableLabel);
		if (lStateIt == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Probability variable '" + mProbabilityVariableLabel + "' does not refer to a state variable.");
		}
		double lProbability = Core::castHandleT<Core::Double>(lStateIt->second)->getValue();

		if (ioContext.getRandomizer().rollUniform() <= lProbability) {
			// event
			getArgument(inIndex, 0, ioContext);
		} else {
			// no event
			getArgument(inIndex, 1, ioContext);
		}
	} else {
		throw schnaps_InternalExceptionM("Primitive is not defined for context '" + ioContext.getName() + "'!");
	}
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Meds::Eventv2::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& Eventv2::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<2);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Eventv2::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& Eventv2::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Eventv2::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
