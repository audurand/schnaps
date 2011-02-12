/*
 * ProcessPushed.cpp
 *
 *  Created on: 2009-02-26
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

ProcessPushed::ProcessPushed()
{}

ProcessPushed::ProcessPushed(const ProcessPushed& inOriginal)
{
	mLabel = inOriginal.mLabel.c_str();
}

ProcessPushed::ProcessPushed(const std::string inLabel)
{
	mLabel = inLabel.c_str();
}

void simulator::ProcessPushed::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("readWithSystem", "Process", getName());
	schnaps_StackTraceEndM("void simulator::ProcessPushed::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void simulator::ProcessPushed::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("writeContent", "Process", getName());
	schnaps_StackTraceEndM("void simulator::ProcessPushed::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle ProcessPushed::execute(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::AnyType::Handle lResult;
		core::PrimitiveTree::Handle lCurrentPrimitiveTree;
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);

#ifndef SCHNAPS_NDEBUG
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
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::ProcessPushed::execute(core::ExecutionContext&) const ");
}
