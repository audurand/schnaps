/*
 * ProcessCall.cpp
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

ProcessCall::ProcessCall() :
		Primitive(0)
{}


ProcessCall::ProcessCall(const ProcessCall& inOriginal) :
		Primitive(0)
{
	mLabel = inOriginal.getLabel().c_str();
}

ProcessCall::ProcessCall(std::string inLabel) :
		Primitive(0),
		mLabel(inLabel.c_str())
{}

void ProcessCall::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			throw schnaps_IOExceptionNodeM(*inIter, "label of called process expected!");
		}

		mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void basic::ProcessCall::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void ProcessCall::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void basic::ProcessCall::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle ProcessCall::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::AnyType::Handle lResult;
		core::PrimitiveTree::Handle lCurrentPrimitiveTree;
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);

#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getProcesses().find(mLabel) == lContext.getProcesses().end()) {
			throw schnaps_InternalExceptionM("Could not find process '" + mLabel + "' in the process database!");
		}
#else
		schnaps_AssertM(lContext.getProcesses().find(mLabel) != lContext.getProcesses().end());
#endif

		// Save current primitive tree
		lCurrentPrimitiveTree = lContext.getPrimitiveTreeHandle();

		// Process called primitive tree
		lResult = lContext.getProcesses().find(mLabel)->second->execute(ioContext);

		// Restore current primitive tree
		lContext.setPrimitiveTree(lCurrentPrimitiveTree);

		return lResult;
	schnaps_StackTraceEndM("core::AnyType::Handle basic::ProcessCall::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& ProcessCall::getReturnType(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getProcesses().find(mLabel) == lContext.getProcesses().end()) {
			throw schnaps_InternalExceptionM("Could not find process '" + mLabel + "' in the process database!");
		}
#else
		schnaps_AssertM(lContext.getProcesses().find(mLabel) != lContext.getProcesses().end());
#endif
		return lContext.getProcesses().find(mLabel)->second->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& basic::ProcessCall::getReturnType(core::ExecutionContext&) const");
}
