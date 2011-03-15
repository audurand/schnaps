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
	mChoiceVariableLabel(""),
	mChoices_Ref("")
{}

/*!
 * \brief Construct a choice primitive over a variable as a copy of an original.
 * \param inOriginal A const reference to the original choice primitive over a variable.
 */
ChoiceIsEqual::ChoiceIsEqual(const ChoiceIsEqual& inOriginal) :
	Primitive(inOriginal.getNumberArguments()),
	mChoiceVariableLabel(inOriginal.mChoiceVariableLabel.c_str()),
	mChoices_Ref(inOriginal.mChoices_Ref.c_str())
{
	if (mChoices_Ref.empty()) {
		mChoices.clear();
		for (ChoiceMap::const_iterator lIt = inOriginal.mChoices.begin(); lIt != inOriginal.mChoices.end(); lIt++) {
			mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
		}
	} else {
		mChoices.clear();
		for (ChoiceMap::const_iterator lIt = inOriginal.mChoices.begin(); lIt != inOriginal.mChoices.end(); lIt++) {
			mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first), lIt->second));
		}
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ChoiceIsEqual& ChoiceIsEqual::operator=(const ChoiceIsEqual& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	mChoiceVariableLabel = inOriginal.mChoiceVariableLabel.c_str();
	mChoices_Ref = inOriginal.mChoices_Ref.c_str();

	mChoices.clear();
	if (mChoices_Ref.empty()) {
		for (ChoiceMap::const_iterator lIt = inOriginal.mChoices.begin(); lIt != inOriginal.mChoices.end(); lIt++) {
			mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first->clone()), lIt->second));
		}
	} else {
		for (ChoiceMap::const_iterator lIt = inOriginal.mChoices.begin(); lIt != inOriginal.mChoices.end(); lIt++) {
			mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>(lIt->first), lIt->second));
		}
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::ChoiceIsEqual& SCHNAPS::Plugins::Control::ChoiceIsEqual::operator=(const SCHNAPS::Plugins::Control::ChoiceIsEqual&)");
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
	// retrieve choice variable label
	if (inIter->getAttribute("choiceVariableLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "choice variable label expected!");
	}
	mChoiceVariableLabel = inIter->getAttribute("choiceVariableLabel");

	// retrieve choices
	if (inIter->getAttribute("choices").empty()) {
		if (inIter->getAttribute("choices.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "possible choices expected!");
		} else { // use referenced choices
			mChoices_Ref = inIter->getAttribute("choices.ref");
			std::stringstream lSS;
			lSS << "ref." << mChoices_Ref;

			Core::Vector::Handle lChoices = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
			for (unsigned int i = 0; i < lChoices->size(); i++) {
				mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(Core::castHandleT<Core::Atom>((*lChoices)[i]), i));
			}
		}
	} else { // use defined keys
		// retrieve key type
		if (inIter->getAttribute("choiceType").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "type of choice expected!");
		}
		Core::Atom::Alloc::Handle lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("choiceType")));

		std::stringstream lSS(inIter->getAttribute("choices"));
		PACC::Tokenizer lTokenizer(lSS);
		lTokenizer.setDelimiters("|", "");

		std::string lKey;
		Core::Atom::Handle lAtom;

		unsigned int lChoice = 0;
		while (lTokenizer.getNextToken(lKey)) {
			// add to map
			lAtom = Core::castHandleT<Core::Atom>(lAlloc->allocate());
			if (lAtom == NULL) {
				std::ostringstream lOSS;
				lOSS << "no atom named '" <<  inIter->getAttribute("choiceType");
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}
			lAtom->readStr(lKey);
			mChoices.insert(std::pair<Core::Atom::Handle, unsigned int>(lAtom, lChoice));
			lChoice++;
		}
	}

	if (mChoices.size() == 0) {
		throw schnaps_IOExceptionNodeM(*inIter, "at least one choice is expected!");
	}

	setNumberArguments(mChoices.size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ChoiceIsEqual::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("choiceVariableLabel", mChoiceVariableLabel);

	if (mChoices_Ref.empty()) {
		std::string lChoices;
		for (std::map<Core::Atom::Handle, unsigned int, Core::IsLessPointerPredicate>::const_iterator lIt = mChoices.begin(); lIt != mChoices.end(); lIt++) {
			lChoices += (*lIt).first->writeStr();
			lChoices += " ";
		}
		ioStreamer.insertAttribute("choiceType", mChoices.begin()->first->getName());
		ioStreamer.insertAttribute("choices", lChoices);
	} else {
		ioStreamer.insertAttribute("choices.ref", mChoices_Ref);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the variable is not in choices.
 */
Core::AnyType::Handle ChoiceIsEqual::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Atom::Handle lVariable;
	if (ioContext.getName() == "GenerationContext") {
		Simulation::GenerationContext& lContext = Core::castObjectT<Simulation::GenerationContext&>(ioContext);
		if (lContext.getIndividual().getState().hasVariable(mChoiceVariableLabel) == false) {
			// intialize the variable before continuing
			
			// save current primitive tree
			Core::PrimitiveTree::Handle lCurrentPrimitiveTree = lContext.getPrimitiveTreeHandle();
			
			if (lContext.getGenProfile().getDemography().hasVariable(mChoiceVariableLabel) == false) {
				// variable not in demography, check in simulation variables
				if (lContext.getGenProfile().getSimulationVariables().hasVariable(mChoiceVariableLabel)) {
					// variable not in simulation variables either, throw error
					throw schnaps_RunTimeExceptionM("Variable " + mChoiceVariableLabel + " is empty for current individual and is not contained in demography nor in simulation variables.");
				} else {
					// variable is in simulation variables
					lVariable = Core::castHandleT<Core::Atom>(lContext.getGenProfile().getSimulationVariables().getVariableInitTree(mChoiceVariableLabel).interpret(ioContext));
				}
			} else {
				// variable is in demography
				lVariable = Core::castHandleT<Core::Atom>(lContext.getGenProfile().getDemography().getVariableInitTree(mChoiceVariableLabel).interpret(ioContext));
			}
			// add newly computed variable to individual
			lContext.getIndividual().getState().insertVariable(mChoiceVariableLabel, Core::castHandleT<Core::Atom>(lVariable->clone()));
			
			// restore primitive tree
			lContext.setPrimitiveTree(lCurrentPrimitiveTree);
		} else {
			lVariable = lContext.getIndividual().getState().getVariableHandle(mChoiceVariableLabel);
		}
	} else {
		Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
		lVariable = lContext.getIndividual().getState().getVariableHandle(mChoiceVariableLabel);
	}

	ChoiceMap::const_iterator lIterChoice = mChoices.find(lVariable);
	if (lIterChoice == mChoices.end()) {
		std::stringstream lOSS;
		lOSS << "Variable value " << lVariable->writeStr() << " is not in choices; ";
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
