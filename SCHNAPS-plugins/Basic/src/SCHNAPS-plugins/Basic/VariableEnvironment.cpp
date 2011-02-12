/*
 * VariableEnvironment.cpp
 *
 *  Created on: 2009-03-17
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

/*!
 *  \brief Construct a new token terminal primitive that refers to a variable from the environment.
 */
VariableEnvironment::VariableEnvironment() :
		core::Primitive(0)
{}

VariableEnvironment::VariableEnvironment(const VariableEnvironment& inOriginal) :
		core::Primitive(0),
		mLabel(inOriginal.getLabel().c_str())
{}

void VariableEnvironment::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			throw schnaps_IOExceptionNodeM(*inIter, "label of the referenced environment variable expected!");
		}
		mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void simulator::VariableEnvironment::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void VariableEnvironment::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void simulator::VariableEnvironment::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle VariableEnvironment::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel) != lContext.getEnvironment().getState().end());
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel)->second != NULL);
		return lContext.getEnvironment().getState().find(mLabel)->second->clone();
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::VariableEnvironment::execute(unsigned int, core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive VariableEnvironment.
 *  \param  ioContext Execution context.
 *  \return Hierarchical typing tagging the data type returned.
 */
const std::string& VariableEnvironment::getReturnType(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel) != lContext.getEnvironment().getState().end());
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel)->second != NULL);
		return lContext.getEnvironment().getState().find(mLabel)->second->getType();
	schnaps_StackTraceEndM("const std::string& simulator::VariableEnvironment::getReturnType(core::ExecutionContext&) const");
}
