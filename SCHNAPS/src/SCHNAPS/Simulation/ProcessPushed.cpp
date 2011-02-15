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
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

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

void SCHNAPS::Simulation::ProcessPushed::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("readWithSystem", "Process", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ProcessPushed::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void SCHNAPS::Simulation::ProcessPushed::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("writeContent", "Process", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ProcessPushed::writeContent(PACC::XML::Streamer&, bool) const");
}

SCHNAPS::Core::AnyType::Handle ProcessPushed::execute(SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::AnyType::Handle lResult;
		SCHNAPS::Core::PrimitiveTree::Handle lCurrentPrimitiveTree;
		SCHNAPS::Simulation::SimulationContext& lContext = SCHNAPS::Core::castObjectT<SCHNAPS::Simulation::SimulationContext&>(ioContext);

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
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Simulation::ProcessPushed::execute(SCHNAPS::Core::ExecutionContext&) const ");
}
