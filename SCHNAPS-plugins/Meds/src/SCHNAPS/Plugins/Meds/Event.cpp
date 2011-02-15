/*
 * Event.cpp
 *
 *  Created on: 2010-11-02
 *  Updated on: 2010-11-02
 *      Author: Audrey Durand
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

Event::Event() :
		Primitive(2),
		mLabel(""),
		mProbabilityVariableLabel("")
{}

Event::Event(const Event& inOriginal) :
		Primitive(2),
		mLabel(inOriginal.mLabel.c_str()),
		mProbabilityVariableLabel(inOriginal.mProbabilityVariableLabel.c_str())
{}

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

		// Retrieve label
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of event expected!");
		}
		mLabel = inIter->getAttribute("label");

		// Retrieve label of individual variable associated to event probability
		if (inIter->getAttribute("probabilityVariableLabel").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of individual variable associated to event probability expected!");
		}
		mProbabilityVariableLabel = inIter->getAttribute("probabilityVariableLabel");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Event::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void Event::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
		ioStreamer.insertAttribute("probabilityVariableLabel", mProbabilityVariableLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Meds::Event::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle Event::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (ioContext.getName() == "SimulationContext") {
			Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);

			// get event probability for the individual
			Simulation::State::iterator lStateIt = lContext.getIndividual().getState().find(mProbabilityVariableLabel);
			if (lStateIt == lContext.getIndividual().getState().end()) {
				throw schnaps_InternalExceptionM("Event probability variable '" + mProbabilityVariableLabel + "' does not refer to a state variable.");
			}
			double lProbability = Core::castHandleT<Core::Double>(lStateIt->second)->getValue();

			if (ioContext.getRandomizer().rollUniform() <= lProbability) { // event
				getArgument(inIndex, 0, ioContext);
			} else { // no event
				getArgument(inIndex, 1, ioContext);
			}
		} else {
			throw schnaps_InternalExceptionM("Osteoporosis event primitive is not defined for context '" + ioContext.getName() + "'!");
		}
		return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Meds::Event::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& Event::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<3);
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Event::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& Event::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Event::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
