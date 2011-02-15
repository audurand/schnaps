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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

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

void ProcessCall::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessCall::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void ProcessCall::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessCall::writeContent(PACC::XML::Streamer&, bool) const");
}

SCHNAPS::Core::AnyType::Handle ProcessCall::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::AnyType::Handle lResult;
		SCHNAPS::Core::PrimitiveTree::Handle lCurrentPrimitiveTree;
		Simulation::SimulationContext& lContext = SCHNAPS::Core::castObjectT<Simulation::SimulationContext&>(ioContext);

#ifdef SCHNAPS_Plugins_FULL_DEBUG
		if (lContext.getProcesses().find(mLabel) == lContext.getProcesses().end()) {
			throw SCHNAPS_Plugins_InternalExceptionM("Could not find process '" + mLabel + "' in the process database!");
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
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::ProcessCall::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

const std::string& ProcessCall::getReturnType(SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = SCHNAPS::Core::castObjectT<Simulation::SimulationContext&>(ioContext);
#ifdef SCHNAPS_Plugins_FULL_DEBUG
		if (lContext.getProcesses().find(mLabel) == lContext.getProcesses().end()) {
			throw SCHNAPS_Plugins_InternalExceptionM("Could not find process '" + mLabel + "' in the process database!");
		}
#else
		schnaps_AssertM(lContext.getProcesses().find(mLabel) != lContext.getProcesses().end());
#endif
		return lContext.getProcesses().find(mLabel)->second->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessCall::getReturnType(SCHNAPS::Core::ExecutionContext&) const");
}
