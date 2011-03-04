/*
 * IsEqualVariable.cpp
 *
 *  Created on: 2010-11-22
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 *  \brief Construct a new primitive to check if variable equals value.
 */
IsEqualVariable::IsEqualVariable() :
	Core::Primitive(0),
	mLabel(""),
	mValue_Ref(""),
	mValue(NULL)
{}

IsEqualVariable::IsEqualVariable(const IsEqualVariable& inOriginal) :
	Core::Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mValue_Ref(inOriginal.mValue_Ref.c_str())
{
	if (mValue_Ref.empty()) {
		mValue = Core::castHandleT<Core::Atom>(inOriginal.mValue->clone());
	} else {
		mValue = Core::castHandleT<Core::Atom>(inOriginal.mValue);
	}
}

void IsEqualVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
		throw schnaps_IOExceptionNodeM(*inIter, "label of variable to compare expected!");
	}
	mLabel = inIter->getAttribute("label");

	// retrieve value
	if (inIter->getAttribute("value").empty()) {
		if (inIter->getAttribute("value.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "comparison value expected!");
		} else { // from parameter
			mValue_Ref = inIter->getAttribute("value.ref");

			std::stringstream lSS;
			lSS << "ref." << mValue_Ref;
			mValue = Core::castHandleT<Core::Atom>(ioSystem.getParameters()[lSS.str().c_str()]);
		}
	} else { // explicitly given
		if (inIter->getAttribute("valueType").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "type of comparison value expected!");
		}

		Core::Atom::Alloc::Handle lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
		mValue =  Core::castHandleT<Core::Atom>(lAlloc->allocate());
		if (mValue == NULL) {
			std::ostringstream lOSS;
			lOSS << "no number named '" <<  inIter->getAttribute("valueType");
			lOSS << "' found in the factory";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
		mValue->readStr(inIter->getAttribute("value"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsEqualVariable::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void IsEqualVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	if (mValue_Ref.empty()) {
		ioStreamer.insertAttribute("valueType", mValue->getName());
		ioStreamer.insertAttribute("value", mValue->writeStr());
	} else {
		ioStreamer.insertAttribute("value.ref", mValue_Ref);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsEqualVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

Core::AnyType::Handle IsEqualVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);

#ifndef SIMULATOR_NDEBUG
	if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
		throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in the current individual state!");
	}
#else
	schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
#endif

	Core::Atom::Handle lVariable = Core::castHandleT<Core::Atom>(lContext.getIndividual().getState()[mLabel]);
	return new Core::Bool(lVariable->isEqual(*mValue));
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsEqualVariable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

const std::string& IsEqualVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsEqualVariable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
