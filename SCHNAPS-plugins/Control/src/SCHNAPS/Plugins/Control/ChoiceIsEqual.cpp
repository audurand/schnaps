/*
 * ChoiceIsEqual.cpp
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
ChoiceIsEqual::ChoiceIsEqual() :
	Primitive(),	// unknown number of children
	mValue_Ref(""),
	mValue(NULL),
	mChoices_Ref("")
{}

/*!
 * \brief Construct a choice primitive over a variable as a copy of an original.
 * \param inOriginal A const reference to the original choice primitive over a variable.
 */
ChoiceIsEqual::ChoiceIsEqual(const ChoiceIsEqual& inOriginal) :
	Primitive(inOriginal.getNumberArguments()),
	mValue_Ref(inOriginal.mValue_Ref.c_str()),
	mChoices_Ref(inOriginal.mChoices_Ref.c_str())
{
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
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = Core::castHandleT<Core::Atom>(inOriginal.mValue->clone());
			break;
	}
	
	mChoiceMap.clear();
	switch (mChoices_Ref[0]) {
		case '$':
			// parameter value
			for (ChoiceMap::const_iterator lIt = inOriginal.mChoiceMap.begin(); lIt != inOriginal.mChoiceMap.end(); lIt++) {
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(lIt->first, lIt->second));
			}
			break;
		default:
			// direct value
			for (ChoiceMap::const_iterator lIt = inOriginal.mChoiceMap.begin(); lIt != inOriginal.mChoiceMap.end(); lIt++) {
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
			}
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ChoiceIsEqual& ChoiceIsEqual::operator=(const ChoiceIsEqual& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	mValue_Ref.assign(inOriginal.mValue_Ref);
	mChoices_Ref.assign(inOriginal.mChoices_Ref);
	
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
			mValue = inOriginal.mValue;
			break;
		default:
			// direct value
			mValue = Core::castHandleT<Core::Atom>(inOriginal.mValue->clone());
			break;
	}
	
	mChoiceMap.clear();
	switch (mChoices_Ref[0]) {
		case '$':
			// parameter value
			for (ChoiceMap::const_iterator lIt = inOriginal.mChoiceMap.begin(); lIt != inOriginal.mChoiceMap.end(); lIt++) {
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(lIt->first, lIt->second));
			}
			break;
		default:
			// direct value
			for (ChoiceMap::const_iterator lIt = inOriginal.mChoiceMap.begin(); lIt != inOriginal.mChoiceMap.end(); lIt++) {
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
			}
			break;
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::ChoiceIsEqual& SCHNAPS::Plugins::Control::ChoiceIsEqual::operator=(const SCHNAPS::Plugins::Control::ChoiceIsEqual&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if choices or inValue attributes are missing.
 * \throw SCHNAPS::Core::IOException if choices attribute is given as direct value and inChoices_Type attribute is missing.
 * \throw SCHNAPS::Core::IOException if choice bounds are not given in crescent order.
 * \throw SCHNAPS::Core::IOException if less than one choice are given.
 * \throw SCHNAPS::Core::RunTimeException if the primitive is undefined for the specific choices source.
 */
void ChoiceIsEqual::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	
	// retrieve value to switch on
	if (inIter->getAttribute("inValue").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "value to switch on expected!");
	}
	mValue_Ref.assign(inIter->getAttribute("inValue"));
	
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
			mValue = Core::castHandleT<Core::Atom>(ioSystem.getParameters().getParameterHandle(mValue_Ref.substr(1)));
			break;
		default: {
			// direct value
			if (inIter->getAttribute("inChoices_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of choices expected!");
			}
			Core::Atom::Alloc::Handle lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inChoices_Type")));
			mValue = Core::castHandleT<Core::Atom>(lAlloc->allocate());
			mValue->readStr(mValue_Ref);
			break; }
	}

	// retrieve choices
	if (inIter->getAttribute("inChoices").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "possible choices expected!");
	}
	mChoices_Ref.assign(inIter->getAttribute("inChoices"));
	
	mChoiceMap.clear();
	switch (mChoices_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific choices source!");
			break;
		case '$': {
			// parameter value
			Core::Vector::Handle lChoices = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(mChoices_Ref.substr(1)));
			for (unsigned int i = 0; i < lChoices->size(); i++) {
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>((*lChoices)[i]), i));
			}
			break; }
		default: {
			// direct value
			// retrieve type of choices
			if (inIter->getAttribute("inChoices_Type").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of choices expected!");
			}
			Core::Atom::Alloc::Handle lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("inChoices_Type")));
			
			std::stringstream lSS(mChoices_Ref);
			PACC::Tokenizer lTokenizer(lSS);
			lTokenizer.setDelimiters("|", "");

			std::string lChoice;
			Core::Atom::Handle lAtom;

			unsigned int lIndex = 0;
			while (lTokenizer.getNextToken(lChoice)) {
				// add to map
				lAtom = Core::castHandleT<Core::Atom>(lAlloc->allocate());
				lAtom->readStr(lChoice);
				mChoiceMap.insert(std::pair<Core::Atom::Handle, unsigned int>(lAtom, lIndex));
				lIndex++;
			}
			break; }
	}

	if (mChoiceMap.empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "at least one choice is expected!");
	}

	setNumberArguments(mChoiceMap.size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ChoiceIsEqual::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inValue", mValue_Ref);
	ioStreamer.insertAttribute("inChoices", mChoices_Ref);
	ioStreamer.insertAttribute("inChoices_Type", mChoiceMap.begin()->first->getType());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the value is not in choices.
 */
Core::AnyType::Handle ChoiceIsEqual::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	ChoiceMap::const_iterator lIterChoice;
	Core::Atom::Handle lValue;
	
	switch (mValue_Ref[0]) {
		case '@':
			// individual variable value
			lValue = Core::castHandleT<Core::Atom>(lContext.getIndividual().getState().getVariableHandle(mValue_Ref.substr(1)));
			break;
		case '#':
			// environment variable value
			lValue = Core::castHandleT<Core::Atom>(lContext.getEnvironment().getState().getVariableHandle(mValue_Ref.substr(1))->clone());
			break;
		case '%':
			// local variable value
			lValue = Core::castHandleT<Core::Atom>(lContext.getLocalVariableHandle(mValue_Ref.substr(1)));
			break;
		case '$':
			// parameter value
			lValue =  Core::castHandleT<Core::Atom>(mValue->clone());
		default:
			// direct value
			lValue = mValue;
			break;
	}
	
	lIterChoice = mChoiceMap.find(lValue);
	if (lIterChoice == mChoiceMap.end()) {
		std::stringstream lOSS;
		lOSS << "Value " << lValue->writeStr() << " is not in choices; ";
		lOSS << "could not make choice.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	return getArgument(inIndex, lIterChoice->second, ioContext);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::ChoiceIsEqual::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 */
const std::string& ChoiceIsEqual::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsEqual::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ChoiceIsEqual::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
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
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsEqual::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
