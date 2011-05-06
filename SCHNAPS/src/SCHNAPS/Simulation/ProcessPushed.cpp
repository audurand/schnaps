/*
 * ProcessPushed.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

/*!
 * \brief Default constructor.
 */
ProcessPushed::ProcessPushed()
{
	mLabel = "";
}

/*!
 * \brief Construct a pushed process as a copy of an original.
 * \param inOriginal The original push process.
 */
ProcessPushed::ProcessPushed(const ProcessPushed& inOriginal)	
{
	mLabel = inOriginal.mLabel.c_str();
}

/*!
 * \brief Construct a pushed process with a specific label.
 * \param inLabel The specific pushed process label.
 */
ProcessPushed::ProcessPushed(const std::string inLabel)
{
	mLabel = inLabel.c_str();
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::InternalException if the method is undefined for the class.
 * 
 */
void ProcessPushed::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("readWithSystem", "PushedProcess", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ProcessPushed::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 * \throw SCHNAPS::Core::InternalException if the method is undefined for the class.
 */
void ProcessPushed::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("writeContent", "PushedProcess", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::ProcessPushed::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Return a handle to the result of process execution.
 * \param  ioContext A reference to the execution context required for primitive tree function execution.
 * \return A handle to the result of process execution.
 */
Core::AnyType::Handle ProcessPushed::execute(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	SimulationContext& lContext = Core::castObjectT<SimulationContext&>(ioContext);
	return lContext.getProcesses().find(mLabel)->second->execute(ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Simulation::ProcessPushed::execute(SCHNAPS::Core::ExecutionContext&) const ");
}
