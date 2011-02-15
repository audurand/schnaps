/*
 * ChoiceIsEqual.cpp
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new primitive that returns the result of a ChoiceIsEqual.
 */
ChoiceIsEqual::ChoiceIsEqual() :
		Primitive(),	// unknown number of children
		mChoiceVariableLabel(""),
		mChoices_Ref(""),
		mCatchError(true)
{}

ChoiceIsEqual::ChoiceIsEqual(const ChoiceIsEqual& inOriginal) :
		Primitive(inOriginal.getNumberArguments()),
		mChoiceVariableLabel(inOriginal.mChoiceVariableLabel.c_str()),
		mChoices_Ref(inOriginal.mChoices_Ref.c_str()),
		mCatchError(inOriginal.mCatchError)
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

				Core::Vector::Handle lChoices = Core::castHandleT<Core::Vector>(ioSystem.getParameters()[lSS.str().c_str()]);
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

		// retrieve flag for catching errors
		if (!inIter->getAttribute("catchError").empty()) {
			if ((inIter->getAttribute("catchError") == "true") || (inIter->getAttribute("catchError") == "1")) {
				mCatchError = true;
			} else if ((inIter->getAttribute("catchError") == "false") || (inIter->getAttribute("catchError") == "0")) {
				mCatchError = false;
			} else {
				throw schnaps_InternalExceptionM("Unknown bool value '" + inIter->getAttribute("choiceType") + "'!");
			}
		}

#ifndef SIMULATOR_NDEBUG
		if (mChoices.size() == 0) {
			throw schnaps_IOExceptionNodeM(*inIter, "at least one choice expected!");
		}
#else
		schnaps_AssertM(mChoices.size() > 0);
#endif

		setNumberArguments(mChoices.size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

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

		ioStreamer.insertAttribute("catchErrors", mCatchError);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ChoiceIsEqual::writeContent(PACC::XML::Streamer&, bool) const");
}

ChoiceIsEqual& ChoiceIsEqual::operator=(const ChoiceIsEqual& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		mChoiceVariableLabel = inOriginal.mChoiceVariableLabel.c_str();
		mChoices_Ref = inOriginal.mChoices_Ref.c_str();
		mCatchError = inOriginal.mCatchError;

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
	schnaps_StackTraceEndM("Core::ChoiceIsEqual& SCHNAPS::Plugins::Control::ChoiceIsEqual::operator=(const Core::ChoiceIsEqual&)");
}

Core::AnyType::Handle ChoiceIsEqual::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
#ifdef SIMULATOR_FULL_DEBUG
		if (lContext.getIndividual().getState().find(mChoiceVariableLabel) == lContext.getIndividual().getState().end()) {
			throw schnaps_InternalExceptionM("Could not find variable '" + mChoiceVariableLabel + "' in current individual state!");
		}
		if (lContext.getIndividual().getState().find(mChoiceVariableLabel)->second == NULL) {
			throw schnaps_InternalExceptionM("Variable '" + mChoiceVariableLabel + "' is empty for current individual!");
		}
#else
		schnaps_AssertM(lContext.getIndividual().getState().find(mChoiceVariableLabel) != lContext.getIndividual().getState().end());
		schnaps_NonNullPointerAssertM(lContext.getIndividual().getState().find(mChoiceVariableLabel)->second);
#endif

		ChoiceMap::const_iterator lIt = mChoices.find(lContext.getIndividual().getState()[mChoiceVariableLabel]);
		if (lIt != mChoices.end()) {
			return getArgument(inIndex, (*lIt).second, ioContext);
		} else {
			if (mCatchError) {
				throw schnaps_InternalExceptionM("Variable value " + lContext.getIndividual().getState()[mChoiceVariableLabel]->writeStr() + " not in choices!");
			} else {
				return NULL;
			}
		}
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::ChoiceIsEqual::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& ChoiceIsEqual::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsEqual::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

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
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsEqual::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
