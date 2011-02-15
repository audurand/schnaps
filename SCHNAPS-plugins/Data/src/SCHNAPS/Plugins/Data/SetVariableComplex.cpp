/*
 * SetVariableComplex.cpp
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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 *  \brief Construct a new primitive to set the value of a variable.
 */
SetVariableComplex::SetVariableComplex() :
		Core::Primitive(1)
{}

SetVariableComplex::SetVariableComplex(const SetVariableComplex& inOriginal) :
		Core::Primitive(1),
		mLabel(inOriginal.mLabel.c_str())
{}

void SetVariableComplex::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
		// retrieve label
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label of variable to set expected!");
		}
		mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariableComplex::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void SetVariableComplex::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariableComplex::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle SetVariableComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);

#ifdef SCHNAPS_FULL_DEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in the current individual state!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
#endif
		Core::Atom::Handle lArg = Core::castHandleT<Core::Atom>(getArgument(inIndex, 0, ioContext));

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
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::SetVariableComplex::execute(unsigned int, Core::ExecutionContext&)");
}

const std::string& SetVariableComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		schnaps_AssertM(inN<1);
		const static std::string lType("Atom");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetVariableComplex::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& SetVariableComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetVariableComplex::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
