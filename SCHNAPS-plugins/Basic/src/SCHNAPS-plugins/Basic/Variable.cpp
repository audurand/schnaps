/*
 * Variable.cpp
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
 *  \brief Construct a new token terminal primitive that refers to a variable.
 */
Variable::Variable() :
		Primitive(0)
{}

Variable::Variable(const Variable& inOriginal) :
		Primitive(0),
		mLabel(inOriginal.mLabel.c_str())
{}

void Variable::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			throw schnaps_IOExceptionNodeM(*inIter, "label of the referenced variable expected!");
		}
		mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void basic::Variable::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Variable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void basic::Variable::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle Variable::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);
#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in current individual state!");
		}
		if (lContext.getIndividual().getState().find(mLabel)->second == NULL) {
			throw schnaps_InternalExceptionM("Variable '" + mLabel + "' is empty for current individual!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
		schnaps_NonNullPointerAssertM(lContext.getIndividual().getState().find(mLabel)->second);
#endif
		return lContext.getIndividual().getState()[mLabel]->clone();
	schnaps_StackTraceEndM("core::AnyType::Handle basic::Variable::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Variable::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);
#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in current individual state!");
		}
		if (lContext.getIndividual().getState().find(mLabel)->second == NULL) {
			throw schnaps_InternalExceptionM("Variable '" + mLabel + "' is empty for current individual!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
		schnaps_NonNullPointerAssertM(lContext.getIndividual().getState().find(mLabel)->second);
#endif
		return lContext.getIndividual().getState()[mLabel]->getType();
		// TODO: this validation needs an individual loaded... validation should be possible without any individual in context.
	schnaps_StackTraceEndM("const std::string& basic::Variable::getReturnType(unsigned int, core::ExecutionContext&) const");
}
