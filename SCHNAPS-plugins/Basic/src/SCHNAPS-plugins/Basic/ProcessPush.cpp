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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

ProcessPush::ProcessPush() :
	Primitive(0)
{}

ProcessPush::ProcessPush(const ProcessPush& inOriginal) :
	Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mTarget(inOriginal.mTarget),
	mDelay(inOriginal.mDelay)
{}

void ProcessPush::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			mTarget = simulator::Process::eCurrent;
		} else if (lTarget == "environment") {
			mTarget = simulator::Process::eEnvironment;
		} else if (lTarget == "individuals") {
			mTarget = simulator::Process::eIndividuals;
		} else {
			throw schnaps_IOExceptionNodeM(*inIter, "ProcessPush " + mLabel + " unknown target: " + lTarget);
		}

		if (inIter->getAttribute("delay").empty()) {
			mDelay = 0;
		} else {
			std::istringstream lISS(inIter->getAttribute("delay"));
			lISS >> mDelay;
		}
	schnaps_StackTraceEndM("void basic::ProcessPush::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void ProcessPush::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
		ioStreamer.insertAttribute("delay", mDelay);
		switch (mTarget) {
		case simulator::Process::eCurrent:
			ioStreamer.insertAttribute("target", "current");
			break;
		case simulator::Process::eEnvironment:
			ioStreamer.insertAttribute("target", "environment");
			break;
		case simulator::Process::eIndividuals:
			ioStreamer.insertAttribute("target", "individuals");
			break;
		default:
			break;
		}
	schnaps_StackTraceEndM("void basic::ProcessPush::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle ProcessPush::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
		lContext.getPushList().push_back(simulator::Push(mLabel, mTarget, lContext.getClock().getValue()+mDelay));
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle basic::ProcessPush::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& ProcessPush::getReturnType(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& basic::ProcessPush::getReturnType(core::ExecutionContext&) const");
}
