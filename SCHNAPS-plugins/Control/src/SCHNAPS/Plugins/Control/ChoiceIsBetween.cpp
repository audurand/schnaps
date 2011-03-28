/*
 * ChoiceIsBetween.cpp
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
ChoiceIsBetween::ChoiceIsBetween() :
	Primitive(),	// unknown number of children
	mValue_Ref(""),
	mValue(NULL),
	mBounds_Ref("")
{}

/*!
 * \brief Construct a choice primitive over a value between bounds as a copy of an original.
 * \param inOriginal A const reference to the original choice primitive over a value between bounds.
 */
ChoiceIsBetween::ChoiceIsBetween(const ChoiceIsBetween& inOriginal) :
	Primitive(inOriginal.getNumberArguments()),
	mValue_Ref(inOriginal.mValue_Ref.c_str()),
	mBounds_Ref(inOriginal.mBounds_Ref.c_str())
{
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = Core::castHandleT<Core::Number>(inOriginal.mValue->clone());
			break;
	}
	
	switch (mBounds_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			break;
		case '$':
			// parameter value
			mBounds = inOriginal.mBounds;
			break;
		default:
			// direct value
			mBounds = new Core::Vector();
			for (unsigned int i = 0; i < inOriginal.mBounds->size(); i++) {
				mBounds->push_back((*inOriginal.mBounds)[i]->clone());
			}
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ChoiceIsBetween& ChoiceIsBetween::operator=(const ChoiceIsBetween& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	mValue_Ref.assign(inOriginal.mValue_Ref);
	mBounds_Ref.assign(inOriginal.mBounds_Ref);
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = Core::castHandleT<Core::Number>(inOriginal.mValue->clone());
			break;
	}
	
	switch (mBounds_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			break;
		case '$':
			// parameter value
			mBounds = inOriginal.mBounds;
			break;
		default:
			// direct value
			mBounds = new Core::Vector();
			for (unsigned int i = 0; i < inOriginal.mBounds->size(); i++) {
				mBounds->push_back((*inOriginal.mBounds)[i]->clone());
			}
			break;
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::ChoiceIsBetween& SCHNAPS::Plugins::Control::ChoiceIsBetween::operator=(const SCHNAPS::Plugins::Control::ChoiceIsBetween&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if choiceVariableLabel attribute is missing.
 * \throw SCHNAPS::Core::IOException if choices attribute and choices.ref attribute are missing.
 * \throw SCHNAPS::Core::IOException if choices attribute is used and choiceType attribute is missing.
 * \throw SCHNAPS::Core::IOException if choice bounds are not given in crescent order.
 * \throw SCHNAPS::Core::IOException if less than one choice are given.
 * \throw SCHNAPS::Core::RunTimeException if the primitive is undefined for the specific bounds source.
 */
void ChoiceIsBetween::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	
	// retrieve value
	if (inIter->getAttribute("inValue").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "choice value expected!");
	}
	mValue_Ref = inIter->getAttribute("value");
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mValue = NULL;
			break;
		case '$':
			// parameter value
			mValue = Core::castHandleT<Core::Number>(ioSystem.getParameters().getParameterHandle(mValue_Ref.substr(1)));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inBounds_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of bounds expected!");
			}
			Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inBounds_Type")));
			mValue = Core::castHandleT<Core::Number>(lAlloc->allocate());
			mValue->readStr(mValue_Ref);
			break; }
	}

	// retrieve choice boundaries
	if (inIter->getAttribute("inBounds").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "choices bounds expected!");
	} else {
		mBounds_Ref.assign(inIter->getAttribute("inBounds"));
		
		switch (mBounds_Ref[0]) {
			case '@':
				// individual variable value
			case '#':
				// environment variable value
			case '%':
				// local variable value
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific bounds source!");
				break;
			case '$':
				// parameter value
				mBounds = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(mBounds_Ref.substr(1)));
				break;
			default: {
				// direct value
				// retrieve type of choices
				if (inIter->getAttribute("inChoices_Type").empty()) {
					throw schnaps_IOExceptionNodeM(*inIter, "type of choices expected!");
				}
				Core::Number::Alloc::Handle lAlloc = Core::castHandleT<Core::Number::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inChoices_Type")));
				
				std::stringstream lSS(mBounds_Ref);
				PACC::Tokenizer lTokenizer(lSS);
				lTokenizer.setDelimiters("|", "");

				std::string lBound;

				mBounds = new Core::Vector();
				while (lTokenizer.getNextToken(lBound)) {
					// add to map
					mBounds->push_back(Core::castHandleT<Core::Number>(lAlloc->allocate()));
					mBounds->back()->readStr(lBound);
				}
				break; }
		}
	}

	if (mBounds->size() == 0) {
		throw schnaps_IOExceptionNodeM(*inIter, "at least one choice lower bound is expected!");
	}

	setNumberArguments(mBounds->size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsBetween::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ChoiceIsBetween::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	ioStreamer.insertAttribute("inBounds", mBounds_Ref);
	ioStreamer.insertAttribute("inBounds_Type", mBounds->back()->getType());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsBetween::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the variable is less than the lower choice bound.
 */
Core::AnyType::Handle ChoiceIsBetween::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Atom lValue;
	
	if (mValue == NULL) {
		switch (mValue_Ref[0]) {
			case '@': {
				// individual variable value
				Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
				lValue = Core::castObjectT<const Core::Atom&>(lContext.getIndividual().getState().getVariable(mValue_Ref.substr(1)));
				break; }
			case '#': {
				// environment variable value
				Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
				lValue = Core::castObjectT<const Core::Atom&>(lContext.getEnvironment().getState().getVariable(mValue_Ref.substr(1)));
				break; }
			case '%':
				// TODO: local variable value
				break;
			default:
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific source!");
				break;
		}
	} else {
		// parameter value or direct value
		lValue = *mValue;
	}
	
	if (lValue.isLess(*(*mBounds)[0])) {
		printf("Value: %s\n", lValue.writeStr().c_str());
		printf("Lower bound: %s\n", (*mBounds)[0]->writeStr().c_str());
		throw schnaps_RunTimeExceptionM("Value '" + lValue.writeStr() + "' is not in any range of choices!");
	}

	for (unsigned int i = 0; i < mBounds->size(); i++) {
		if (lValue.isLess(*(*mBounds)[i])) {
			return getArgument(inIndex, i-1, ioContext);
		}
	}
	return getArgument(inIndex, mBounds->size()-1, ioContext);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::ChoiceIsBetween::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& ChoiceIsBetween::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsBetween::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ChoiceIsBetween::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	if (getNumberArguments() < 2) {
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	}

	std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));

	for (unsigned int i = 2; i < getNumberArguments(); i++) {
		lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
	}

	const static std::string lCommonType_Final(lCommonType);
	return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsBetween::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
