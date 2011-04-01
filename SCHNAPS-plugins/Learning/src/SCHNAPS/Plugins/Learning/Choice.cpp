/*
 * Choice.cpp
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

#include "SCHNAPS/Plugins/Learning/Learning.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

/*!
 * \brief Read variables local to function from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Choice::Function::readLocalVariables(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "LocalVariables") {
		std::ostringstream lOSS;
		lOSS << "tag <LocalVariables> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	mLocalVariables.clear();
	Core::AnyType::Alloc::Handle lAlloc;
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "LocalVariable") {
				std::ostringstream lOSS;
				lOSS << "tag <LocalVariable> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			
			// retrieve label
			if (lChild->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "label attribute expected!");
			}
			mLocalVariables.push_back(std::pair<std::string, Core::AnyType::Handle>(lChild->getAttribute("label"), NULL));

			// retrieve value
			if (lChild->getAttribute("type").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "type attribute expected!");
			}
			if (lChild->getAttribute("value").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "value attribute expected!");
			}
			lAlloc = Core::castHandleT<Core::AnyType::Alloc>(ioSystem.getFactory().getAllocator(lChild->getAttribute("type")));
			mLocalVariables.back().second = Core::castHandleT<Core::AnyType>(lAlloc->allocate());
			mLocalVariables.back().second->readStr(lChild->getAttribute("value"));
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::Function::readLocalVariables(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::Function::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("LocalVariables");
	for (unsigned int i = 0; i < mLocalVariables.size(); i++) {
		ioStreamer.openTag("LocalVariable");
		ioStreamer.insertAttribute("label", mLocalVariables[i].first);
		ioStreamer.insertAttribute("type", mLocalVariables[i].second->getType());
		ioStreamer.insertAttribute("value", mLocalVariables[i].second->writeStr());
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::Function::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Default constructor.
 */
Choice::Choice() :
	mLabel("")
{}

/*!
 * \brief Construct a choice primitive as a copy of an original.
 * \param inOriginal A const reference to the original choice primitive.
 */
Choice::Choice(const Choice& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mFunctionState(inOriginal.mFunctionState),
	mFunctionReward(inOriginal.mFunctionReward)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Choice::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
		throw schnaps_IOExceptionNodeM(*inIter, "choice label expected!");
	}
	mLabel.assign(inIter->getAttribute("label"));

	// read options
	inIter = inIter->getFirstChild();
	readOptions(inIter, ioSystem);

	// read state function
	inIter++;
	readFunctionState(inIter, ioSystem);

	// read reward function
	inIter++;
	readFunctionReward(inIter, ioSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write content of object to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	writeOptions(ioStreamer, inIndent);
	writeFunctionState(ioStreamer, inIndent);
	writeFunctionReward(ioStreamer, inIndent);
	writeChoiceMap(ioStreamer, inIndent);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Return the state computed using a specific learning context.
 * \param  A reference to learning context to use for computing state.
 * \return The state computed using a specific learning context.
 */
std::string Choice::computeState(LearningContext& ioContext) const {
	schnaps_StackTraceBeginM();
	// set local variables
	for (unsigned int i = 0; i < mFunctionState.mLocalVariables.size(); i++) {
		ioContext.insertLocalVariable(
			mFunctionState.mLocalVariables[i].first,
			Core::castHandleT<Core::AnyType>(mFunctionState.mLocalVariables[i].second->clone()));
	}
	
	std::string lState = Core::castHandleT<Core::String>(mFunctionState.mExecution->interpret(ioContext))->getValue();
	ioContext.clearLocalVariables();
	
	return lState;
	schnaps_StackTraceEndM("std::string SCHNAPS::Plugins::Learning::Choice::computeState(SCHNAPS::Plugins::Learning::LearningContext&) const");
}

/*!
 * \brief  Return the reward computed using a specific learning context.
 * \param  A reference to learning context to use for computing reward.
 * \return The reward computed using a specific learning context.
 */
double Choice::computeReward(LearningContext& ioContext) const {
	schnaps_StackTraceBeginM();
	// set local variables
	for (unsigned int i = 0; i < mFunctionReward.mLocalVariables.size(); i++) {
		ioContext.insertLocalVariable(
			mFunctionReward.mLocalVariables[i].first,
			Core::castHandleT<Core::AnyType>(mFunctionReward.mLocalVariables[i].second->clone()));
	}
	
	double lReward = Core::castHandleT<Core::Double>(mFunctionReward.mExecution->interpret(ioContext))->getValue();
	ioContext.clearLocalVariables();
	
	return lReward;
	schnaps_StackTraceEndM("double SCHNAPS::Plugins::Learning::Choice::computeReward(SCHNAPS::Plugins::Learning::LearningContext&) const");
}

// private

/*!
 * \brief Read options from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Choice::readOptions(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Options") {
		std::ostringstream lOSS;
		lOSS << "tag <Options> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	mOptions.clear();
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*lChild, "tag expected!");
		}
		if (lChild->getValue() != "Option") {
			std::ostringstream lOSS;
			lOSS << "tag <Option> expected, but ";
			lOSS << "got tag <" << lChild->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
		}
		
		// retrieve label
		if (lChild->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*lChild, "choice option label expected!");
		}
		mOptions.push_back(inIter->getAttribute("label"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::readOptions(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Read state function from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Choice::readFunctionState(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "FunctionState") {
		std::ostringstream lOSS;
		lOSS << "tag <FunctionState> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	if (inIter->getAttribute("file").empty()) {
		// retrieve function local variables
		inIter = inIter->getFirstChild();
		mFunctionState.readLocalVariables(inIter, ioSystem);
		
		// retrieve function execution tree
		inIter++;
		mFunctionState.mExecution->readWithSystem(inIter, ioSystem);
	} else {
		std::ifstream lIFS(inIter->getAttribute("file").c_str(), std::ios::in);
		PACC::XML::Document lDocument(lIFS);
		readFunctionState(lDocument.getFirstDataTag(), ioSystem);
		lIFS.close();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::readFunctionState(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Read reward function from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Choice::readFunctionReward(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "FunctionReward") {
		std::ostringstream lOSS;
		lOSS << "tag <FunctionReward> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	if (inIter->getAttribute("file").empty()) {
		// retrieve function local variables
		inIter = inIter->getFirstChild();
		mFunctionReward.readLocalVariables(inIter, ioSystem);
		
		// retrieve function execution tree
		inIter++;
		mFunctionReward.mExecution->readWithSystem(inIter, ioSystem);
	} else {
		std::ifstream lIFS(inIter->getAttribute("file").c_str(), std::ios::in);
		PACC::XML::Document lDocument(lIFS);
		readFunctionReward(lDocument.getFirstDataTag(), ioSystem);
		lIFS.close();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::readFunctionReward(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Read choice map from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if state label attribute is missing.
 */
void Choice::readChoiceMap(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "ChoiceMap") {
		std::ostringstream lOSS;
		lOSS << "tag <ChoiceMap> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	clear();
	std::string lState;
	unsigned int lActionID;
	for (PACC::XML::ConstIterator lChild_i = inIter->getFirstChild(); lChild_i; lChild_i++) {
		// read state
		if (lChild_i->getType() == PACC::XML::eData) {
			if (lChild_i->getValue() != "State") {
				std::ostringstream lOSS;
				lOSS << "tag <State> expected, but ";
				lOSS << "got tag <" << lChild_i->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild_i, lOSS.str());
			}
			
			if (lChild_i->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild_i, "choice map state label expected!");
			}
			lState = lChild_i->getAttribute("label");
			
			// init all actions for that state
			getActions(lState);
			lActionID = 0;
			if (lChild_i->getChildCount() != mOptions.size()) {
				throw schnaps_IOExceptionNodeM(*lChild_i, "expected an action description for each option in choice!");
			}
			for (PACC::XML::ConstIterator lChild_j = lChild_i->getFirstChild(); lChild_j; lChild_j++) {
				// read action descriptions for that state
				if (lChild_j->getType() == PACC::XML::eData) {
					(*this)[lState][lActionID].readWithSystem(lChild_j, ioSystem);
				}
                lActionID++;
			}
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::readChoiceMap(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write content of options to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::writeOptions(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Options");
	for (unsigned int i = 0; i < mOptions.size(); i++) {
		ioStreamer.openTag("Option");
		ioStreamer.insertAttribute("label", mOptions[i]);
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::writeOptions(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write content of state function to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::writeFunctionState(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("FunctionState");
	mFunctionState.writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::writeFunctionState(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write content of reward function to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::writeFunctionReward(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("FunctionReward");
	mFunctionReward.writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::writeFunctionReward(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write content of choice map to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Choice::writeChoiceMap(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("ChoiceMap");
	for (Choice::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
		ioStreamer.openTag("State");
		ioStreamer.insertAttribute("label", lIt->first);
		for (unsigned int i = 0; i < lIt->second.size(); i++) {
			lIt->second[i].write(ioStreamer, inIndent);
		}
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Choice::writeChoiceMap(PACC::XML::Streamer&, bool) const");
}
