/*
 * SetVariable.cpp
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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

/*!
 *  \brief Construct a new primitive to set the value of a variable.
 */
SetVariable::SetVariable() :
		core::Primitive(1)
{}

SetVariable::SetVariable(const SetVariable& inOriginal) :
		core::Primitive(1),
		mLabel(inOriginal.mLabel.c_str())
{}

void SetVariable::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
		// Retrieve label
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of variable to set expected!");
		}
		mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void simulator::SetVariable::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void SetVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void simulator::SetVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle SetVariable::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);

#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in the current individual state!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
#endif
		core::Atom::Handle lArg = core::castHandleT<core::Atom>(getArgument(inIndex, 0, ioContext));

		std::string lTypeVariable = lContext.getIndividual().getState()[mLabel]->getType();
		std::string lTypeArg = lArg->getType();
#ifdef SCHNAPS_FULL_DEBUG
		if (lTypeVariable != lTypeArg) {
			throw schnaps_InternalExceptionM("The type of variable '" + mLabel + "' (" + lTypeVariable + ") does not match the type of argument (" + lTypeArg + ")!");
		}
#else
		schnaps_AssertM(lTypeVariable == lTypeArg);
#endif

		lContext.getIndividual().getState()[mLabel] = lArg;
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::SetVariable::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& SetVariable::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		schnaps_AssertM(inN<1);
		const static std::string lType("Atom");
		return lType;
	schnaps_StackTraceEndM("const std::string& simulator::SetVariable::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& SetVariable::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& simulator::SetVariable::getReturnType(unsigned int, core::ExecutionContext&) const");
}
