/*
 * SetVariable.cpp
 *
 *  Created on: 2010-11-20
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

#include "SCHNAPS-plugins/Easier/Easier.hpp"

using namespace easier;

/*!
 *  \brief Construct a new primitive to set the value of a variable.
 */
SetVariable::SetVariable() :
		core::Primitive(0),
		mValue_Ref(""),
		mValue(NULL)
{}

SetVariable::SetVariable(const SetVariable& inOriginal) :
		core::Primitive(0),
		mLabel(inOriginal.mLabel.c_str()),
		mValue_Ref(inOriginal.mValue_Ref.c_str())
{
	if (mValue_Ref.empty()) {
		mValue = core::castHandleT<core::Atom>(inOriginal.mValue->clone());
	} else {
		mValue = core::castHandleT<core::Atom>(inOriginal.mValue);
	}
}

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

		// Retrieve value
		if (inIter->getAttribute("value").empty()) {
			if (inIter->getAttribute("value.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "comparison value expected!");
			} else { // From parameter
				mValue_Ref = inIter->getAttribute("value.ref");

				std::stringstream lSS;
				lSS << "ref." << mValue_Ref;
				mValue = core::castHandleT<core::Atom>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // Explicitly given
			if (inIter->getAttribute("valueType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of comparison value expected!");
			}

			core::Atom::Alloc::Handle lAlloc = core::castHandleT<core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
			mValue =  core::castHandleT<core::Atom>(lAlloc->allocate());
			if (mValue == NULL) {
				std::ostringstream lOSS;
				lOSS << "no type named '" <<  inIter->getAttribute("valueType");
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}
			mValue->readStr(inIter->getAttribute("value"));
		}
	schnaps_StackTraceEndM("void simulator::SetVariable::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void SetVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
		if (mValue_Ref.empty()) {
			ioStreamer.insertAttribute("valueType", mValue->getName());
			ioStreamer.insertAttribute("value", mValue->writeStr());
		} else {
			ioStreamer.insertAttribute("value.ref", mValue_Ref);
		}
	schnaps_StackTraceEndM("void simulator::SetVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle SetVariable::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);

#ifndef SIMULATOR_NDEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in the current individual state!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
#endif

		lContext.getIndividual().getState()[mLabel] = core::castHandleT<core::Atom>(mValue->clone());
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::SetVariable::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& SetVariable::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& simulator::SetVariable::getReturnType(unsigned int, core::ExecutionContext&) const");
}
