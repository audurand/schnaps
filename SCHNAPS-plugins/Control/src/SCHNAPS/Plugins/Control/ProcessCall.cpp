/*
 * ProcessCall.cpp
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 * \brief Default constructor.
 */
ProcessCall::ProcessCall() :
	Primitive(0)
{}

/*!
 * \brief Construct a primitive for calling process as a copy of an original.
 * \param inOriginal A const reference to the original primitive for calling process.
 */
ProcessCall::ProcessCall(const ProcessCall& inOriginal) :
	Primitive(0)
{
	mLabel = inOriginal.mLabel.c_str();
}

/*!
 * \brief Construct a primitive for calling specific process.
 * \param inLabel A const reference to the label of process to call.
 */
ProcessCall::ProcessCall(const std::string& inLabel) :
	Primitive(0),
	mLabel(inLabel.c_str())
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel attribute is missing.
 */
void ProcessCall::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	
	if (inIter->getAttribute("inLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of called process expected!");
	}
	mLabel.assign(inIter->getAttribute("inLabel"));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessCall::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ProcessCall::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inLabel", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessCall::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle ProcessCall::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::AnyType::Handle lResult;
	Core::PrimitiveTree::Handle lCurrentPrimitiveTree;
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);

	// save current primitive tree and local variables
	lCurrentPrimitiveTree = lContext.getPrimitiveTreeHandle();
	Simulation::SimulationContext::LocalVariablesMap lCurrentLocalVariables = lContext.getLocalVariables();

	// execute process called
	lResult = lContext.getProcessHandle(mLabel)->execute(ioContext);

	// restore current primitive tree and local variables
	lContext.setPrimitiveTree(lCurrentPrimitiveTree);
	for (Simulation::SimulationContext::LocalVariablesMap::const_iterator lIt = lCurrentLocalVariables.begin(); lIt != lCurrentLocalVariables.end(); lIt++) {
		lContext.insertLocalVariable(lIt->first, lIt->second);
	}

	return lResult;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::ProcessCall::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ProcessCall::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	return lContext.getProcessHandle(mLabel)->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessCall::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
