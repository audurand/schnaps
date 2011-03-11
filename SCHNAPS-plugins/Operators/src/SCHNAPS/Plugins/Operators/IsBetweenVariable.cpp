/*
 * IsBetweenVariable.cpp
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 * \brief Default constructor.
 */
IsBetweenVariable::IsBetweenVariable() :
	Core::Primitive(0),
	mLabel(""),
	mLowerValue_Ref(""),
	mLowerValue(NULL),
	mUpperValue_Ref(""),
	mUpperValue(NULL)
{}

/*!
 * \brief Construct a comparison operator "is variable between values" as a copy of an original.
 * \param inOriginal A const reference to the original comparison operator "is variable between values".
 */
IsBetweenVariable::IsBetweenVariable(const IsBetweenVariable& inOriginal) :
	Core::Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mLowerValue_Ref(inOriginal.mLowerValue_Ref.c_str()),
	mUpperValue_Ref(inOriginal.mUpperValue_Ref.c_str())
{
	if (mLowerValue_Ref.empty()) {
		mLowerValue = Core::castHandleT<Core::Number>(inOriginal.mLowerValue->clone());
	} else {
		mLowerValue = Core::castHandleT<Core::Number>(inOriginal.mLowerValue);
	}

	if (mUpperValue_Ref.empty()) {
		mUpperValue = Core::castHandleT<Core::Number>(inOriginal.mUpperValue->clone());
	} else {
		mUpperValue = Core::castHandleT<Core::Number>(inOriginal.mUpperValue);
	}
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 * \throw SCHNAPS::Core::IOException if lowerValue attribute and lowerValue.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if lowerValue attribute is used and valueType attribute is missing.
 * \throw SCHNAPS::Core::IOException if upperValue attribute and upperValue.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if upperValue attribute is used and valueType attribute is missing.
 */
void IsBetweenVariable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve lower value
	if (inIter->getAttribute("lowerValue").empty()) {
		if (inIter->getAttribute("lowerValue.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "lower value expected!");
		} else { // from parameter
			mLowerValue_Ref = inIter->getAttribute("lowerValue.ref");

			std::stringstream lSS;
			lSS << "ref." << mLowerValue_Ref;
			mLowerValue = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else { // explicitly given
		if (inIter->getAttribute("valueType").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "type of values expected!");
		}

		Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
		mLowerValue =  Core::castHandleT<Core::Number>(lAlloc->allocate());
		mLowerValue->readStr(inIter->getAttribute("lowerValue"));
	}

	// retrieve upper value
	if (inIter->getAttribute("upperValue").empty()) {
		if (inIter->getAttribute("upperValue.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "upper value expected!");
		} else { // from parameter
			mUpperValue_Ref = inIter->getAttribute("upperValue.ref");

			std::stringstream lSS;
			lSS << "ref." << mUpperValue_Ref;
			mUpperValue = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else { // explicitly given
		if (inIter->getAttribute("valueType").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "type of values expected!");
		}

		Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("valueType")));
		mUpperValue =  Core::castHandleT<Core::Number>(lAlloc->allocate());
		mUpperValue->readStr(inIter->getAttribute("upperValue"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsBetweenVariable::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::IsBetweenVariable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle IsBetweenVariable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	SCHNAPS::Simulation::ExecutionContext& lContext = Core::castObjectT<SCHNAPS::Simulation::ExecutionContext&>(ioContext);

	Core::Number::Handle lVariable = Core::castHandleT<Core::Number>(lContext.getIndividual().getState().getVariableHandle(mLabel));
	return new Core::Bool(!(lVariable->isLess(*mLowerValue) || mUpperValue->isLess(*lVariable)));
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::IsBetweenVariable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& IsBetweenVariable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::IsBetweenVariable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
