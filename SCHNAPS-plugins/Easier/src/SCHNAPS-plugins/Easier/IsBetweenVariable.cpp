/*
 * IsBetweenVariable.cpp
 *
 *  Created on: 2010-12-01
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
 *  \brief Construct a new primitive to check if variable is between theses values (lower value <= variable <= upper value).
 */
IsBetweenVariable::IsBetweenVariable() :
		core::Primitive(0),
		mLabel(""),
		mLowerValue_Ref(""),
		mLowerValue(NULL),
		mUpperValue_Ref(""),
		mUpperValue(NULL)
{}

IsBetweenVariable::IsBetweenVariable(const IsBetweenVariable& inOriginal) :
		core::Primitive(0),
		mLabel(inOriginal.mLabel.c_str()),
		mLowerValue_Ref(inOriginal.mLowerValue_Ref.c_str()),
		mUpperValue_Ref(inOriginal.mUpperValue_Ref.c_str())
{
	if (mLowerValue_Ref.empty()) {
		mLowerValue = core::castHandleT<core::Number>(inOriginal.mLowerValue->clone());
	} else {
		mLowerValue = core::castHandleT<core::Number>(inOriginal.mLowerValue);
	}

	if (mUpperValue_Ref.empty()) {
		mUpperValue = core::castHandleT<core::Number>(inOriginal.mUpperValue->clone());
	} else {
		mUpperValue = core::castHandleT<core::Number>(inOriginal.mUpperValue);
	}
}

void IsBetweenVariable::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			throw schnaps_IOExceptionNodeM(*inIter, "label of variable to compare expected!");
		}
		mLabel = inIter->getAttribute("label");

		// Retrieve lower value
		if (inIter->getAttribute("lowerValue").empty()) {
			if (inIter->getAttribute("lowerValue.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "lower value expected!");
			} else { // From parameter
				mLowerValue_Ref = inIter->getAttribute("lowerValue.ref");

				std::stringstream lSS;
				lSS << "ref." << mLowerValue_Ref;
				mLowerValue = core::castHandleT<core::Number>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // Explicitly given
			if (inIter->getAttribute("valueType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of values expected!");
			}

			core::Number::Alloc::Handle lAlloc = core::castHandleT<core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
			mLowerValue =  core::castHandleT<core::Number>(lAlloc->allocate());
			if (mLowerValue == NULL) {
				std::ostringstream lOSS;
				lOSS << "no number named '" <<  inIter->getAttribute("valueType");
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}
			mLowerValue->readStr(inIter->getAttribute("lowerValue"));
		}

		// Retrieve upper value
		if (inIter->getAttribute("upperValue").empty()) {
			if (inIter->getAttribute("upperValue.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "upper value expected!");
			} else { // From parameter
				mUpperValue_Ref = inIter->getAttribute("upperValue.ref");

				std::stringstream lSS;
				lSS << "ref." << mUpperValue_Ref;
				mUpperValue = core::castHandleT<core::Number>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // Explicitly given
			if (inIter->getAttribute("valueType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of values expected!");
			}

			core::Number::Alloc::Handle lAlloc = core::castHandleT<core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
			mUpperValue =  core::castHandleT<core::Number>(lAlloc->allocate());
			if (mUpperValue == NULL) {
				std::ostringstream lOSS;
				lOSS << "no number named '" <<  inIter->getAttribute("valueType");
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}
			mUpperValue->readStr(inIter->getAttribute("upperValue"));
		}
	schnaps_StackTraceEndM("void simulator::IsBetweenVariable::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void IsBetweenVariable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("label", mLabel);
		if (mLowerValue_Ref.empty()) {
			ioStreamer.insertAttribute("valueType", mLowerValue->getName());
			ioStreamer.insertAttribute("lowerValue", mLowerValue->writeStr());

			if (mUpperValue_Ref.empty()) {
				ioStreamer.insertAttribute("upperValue", mUpperValue->writeStr());
			} else {
				ioStreamer.insertAttribute("upperValue.ref", mUpperValue_Ref);
			}
		} else {
			ioStreamer.insertAttribute("lowerValue.ref", mLowerValue_Ref);

			if (mUpperValue_Ref.empty()) {
				ioStreamer.insertAttribute("valueType", mUpperValue->getName());
				ioStreamer.insertAttribute("upperValue", mUpperValue->writeStr());
			} else {
				ioStreamer.insertAttribute("upperValue.ref", mUpperValue_Ref);
			}
		}
	schnaps_StackTraceEndM("void simulator::IsBetweenVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle IsBetweenVariable::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::ExecutionContext& lContext = core::castObjectT<simulator::ExecutionContext&>(ioContext);

#ifndef SIMULATOR_NDEBUG
		if (lContext.getIndividual().getState().find(mLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mLabel + "' in the current individual state!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mLabel) != lContext.getIndividual().getState().end());
#endif

		core::Number::Handle lVariable = core::castHandleT<core::Number>(lContext.getIndividual().getState()[mLabel]);
		return new core::Bool(!(lVariable->isLess(*mLowerValue) || mUpperValue->isLess(*lVariable)));
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::IsBetweenVariable::execute(unsigned int, core::ExecutionContext&)");
}

const std::string& IsBetweenVariable::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Bool");
		return lType;
	schnaps_StackTraceEndM("const std::string& simulator::IsBetweenVariable::getReturnType(unsigned int, core::ExecutionContext&) const");
}
