/*
 * ProcessPush.cpp
 *
 *  Created on: 2010-01-11
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

ProcessPush::ProcessPush() :
	Primitive(0)
{}

ProcessPush::ProcessPush(const ProcessPush& inOriginal) :
	Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mTarget(inOriginal.mTarget),
	mDelay(inOriginal.mDelay)
{}

void ProcessPush::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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

		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of the referenced process expected!");
		}
		mLabel = inIter->getAttribute("label");

		std::string lTarget = inIter->getAttribute("target");
		if (lTarget.empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "target of the referenced process " + mLabel + " expected!");
		}

		if (lTarget == "current") {
			mTarget = Simulation::Process::eCurrent;
		} else if (lTarget == "environment") {
			mTarget = Simulation::Process::eEnvironment;
		} else if (lTarget == "individuals") {
			mTarget = Simulation::Process::eIndividuals;
		} else {
			throw schnaps_IOExceptionNodeM(*inIter, "ProcessPush " + mLabel + " unknown target: " + lTarget);
		}

		if (inIter->getAttribute("delay").empty()) {
			mDelay = 0;
		} else {
			std::istringstream lISS(inIter->getAttribute("delay"));
			lISS >> mDelay;
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessPush::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void ProcessPush::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
		ioStreamer.insertAttribute("delay", mDelay);
		switch (mTarget) {
		case Simulation::Process::eCurrent:
			ioStreamer.insertAttribute("target", "current");
			break;
		case Simulation::Process::eEnvironment:
			ioStreamer.insertAttribute("target", "environment");
			break;
		case Simulation::Process::eIndividuals:
			ioStreamer.insertAttribute("target", "individuals");
			break;
		default:
			break;
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessPush::writeContent(PACC::XML::Streamer&, bool) const");
}

SCHNAPS::Core::AnyType::Handle ProcessPush::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::SimulationContext& lContext = SCHNAPS::Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		lContext.getPushList().push_back(Simulation::Push(mLabel, mTarget, lContext.getClock().getValue()+mDelay));
		return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::ProcessPush::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

const std::string& ProcessPush::getReturnType(SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessPush::getReturnType(SCHNAPS::Core::ExecutionContext&) const");
}
