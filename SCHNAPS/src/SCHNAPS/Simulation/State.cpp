/*
 * State.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Construct a state as a copy of an original.
 * \param inOriginal A const reference to the original state.
 */
State::State(const State& inOriginal) {
	for (VariablesMap::const_iterator lIt = inOriginal.mVariablesMap.begin(); lIt != inOriginal.mVariablesMap.end(); lIt++) {
		mVariablesMap[lIt->first] = Core::castHandleT<Core::Atom>(lIt->second->clone());
	}
}

/*!
 * \brief Copy operator.
 * \return A reference to the current object.
 */
State& State::operator=(const State& inOriginal) {
	schnaps_StackTraceBeginM();
	mVariablesMap.clear();
	for (VariablesMap::const_iterator lIt = inOriginal.mVariablesMap.begin(); lIt != inOriginal.mVariablesMap.end(); lIt++) {
		mVariablesMap[lIt->first] = Core::castHandleT<Core::Atom>(lIt->second->clone());
	}

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::State& SCHNAPS::Simulation::State::operator=(const SCHNAPS::Simulation::State&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if variable label attribute is missing.
 * \throw SCHNAPS::Core::IOException if variable type attribute is missing.
 * \throw SCHNAPS::Core::IOException if variable value attribute is missing.
 */
void State::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	Core::Atom::Alloc::Handle lAlloc;

	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" + getName() + "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Variable") {
				std::ostringstream lOSS;
				lOSS << "tag <Variable> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			if (lChild->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "label attribute expected!");
			}
			if (lChild->getAttribute("type").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "type attribute expected!");
			}
			if (lChild->getAttribute("value").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "value attribute expected!");
			}

			lAlloc = Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(lChild->getAttribute("type")));
			mVariablesMap[lChild->getAttribute("label")] = Core::castHandleT<Core::Atom>(lAlloc->allocate());
			mVariablesMap[lChild->getAttribute("label")]->readStr(lChild->getAttribute("value"));
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void State::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (VariablesMap::const_iterator lIt = mVariablesMap.begin(); lIt != mVariablesMap.end(); lIt++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", lIt->first);
		ioStreamer.insertAttribute("type", lIt->second->getName());
		ioStreamer.insertAttribute("value", lIt->second->writeStr());
		ioStreamer.closeTag();
	}
}

/*!
 * \brief Print state to file stream.
 * \param ioStream A reference to the file stream.
 * \param inVariables A const reference to the labels of variables to print.
 */
void State::print(std::ostream& ioStream, const std::vector<std::string>& inVariables) const {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < inVariables.size(); i++) {
		ioStream << "," << this->getVariable(inVariables[i]).writeStr();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::print(std::ostream&, const std::vector<std::string>&)");
}

/*!
 * \brief Erase all variables.
 */
void State::clear() {
	schnaps_StackTraceBeginM();
	mVariablesMap.clear();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::clear()");
}

/*!
 * \brief Insert a variable.
 * \param inLabel A const reference to the label of the variable.
 * \param inValue A handle to new value of the variable.
 * \throw SCHNAPS::Core::RunTimeException if the variable already exists.
 */
void State::insertVariable(const std::string& inLabel, Core::Atom::Handle inValue) {
	schnaps_StackTraceBeginM();
	if (mVariablesMap.find(inLabel) != mVariablesMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The variable '" << inLabel << "' already exists; ";
		lOSS << "could not insert it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mVariablesMap.insert(std::pair<std::string, Core::Atom::Handle>(inLabel.c_str(), inValue));
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::insertVariable(const std::string&, SCHNAPS::Core::Atom::Handle) const");
}
	
/*!
 * \brief Remove a variable.
 * \param inLabel A const reference to the label of the variable.
 * \throw SCHNAPS::Core::RunTimeException if the variable does not exist.
 */
void State::removeVariable(const std::string& inLabel) {
	schnaps_StackTraceBeginM();
	VariablesMap::const_iterator lIterVariables = mVariablesMap.find(inLabel);
	if (lIterVariables == mVariablesMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The variable '" << inLabel << "' does not exist; ";
		lOSS << "could not remove it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mVariablesMap.erase(inLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::removeVariable(const std::string&)");
}
	
/*!
 * \brief  Check if the state contains a variable.
 * \param  inLabel A const reference to the label of the variable.
 * \return True if the state has the variable, false if not.
 */
bool State::hasVariable(const std::string& inLabel) const {
	schnaps_StackTraceBeginM();
	VariablesMap::const_iterator lIterVariables = mVariablesMap.find(inLabel);
	if (lIterVariables == mVariablesMap.end()) {
		return false;
	}
	return true;
	schnaps_StackTraceEndM("bool SCHNAPS::Simulation::State::hasVariable(const std::string&) const");
}
