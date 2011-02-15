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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 *  \brief Construct a new token terminal primitive that refers to a variable from the environment.
 */
VariableEnvironment::VariableEnvironment() :
		Core::Primitive(0)
{}

VariableEnvironment::VariableEnvironment(const VariableEnvironment& inOriginal) :
		Core::Primitive(0),
		mLabel(inOriginal.getLabel().c_str())
{}

void VariableEnvironment::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::VariableEnvironment::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void VariableEnvironment::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::VariableEnvironment::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle VariableEnvironment::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel) != lContext.getEnvironment().getState().end());
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel)->second != NULL);
		return lContext.getEnvironment().getState().find(mLabel)->second->clone();
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::VariableEnvironment::execute(unsigned int, Core::ExecutionContext&)");
}

/*!
 *  \brief  Return the tag of the type of data return by primitive VariableEnvironment.
 *  \param  ioContext Execution context.
 *  \return Hierarchical typing tagging the data type returned.
 */
const std::string& VariableEnvironment::getReturnType(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel) != lContext.getEnvironment().getState().end());
	schnaps_AssertM(lContext.getEnvironment().getState().find(mLabel)->second != NULL);
		return lContext.getEnvironment().getState().find(mLabel)->second->getType();
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::VariableEnvironment::getReturnType(Core::ExecutionContext&) const");
}
