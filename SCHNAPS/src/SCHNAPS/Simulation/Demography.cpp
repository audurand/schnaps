/*
 * Demography.cpp
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
 * \brief Construct a demography as a copy of an original.
 * \param inOriginal A const reference to the original demography.
 */
Demography::Demography(const Demography& inOriginal) {
	this->mVariables.clear();
	for (unsigned int i = 0; i < inOriginal.mVariables.size(); i++) {
		this->mVariables.push_back(Variable(
			inOriginal.mVariables[i].mLabel,
			inOriginal.mVariables[i].mInitTree));
		
		for (unsigned int j = 0; j < inOriginal.mVariables[i].mLocalVariables.size(); j++) {
			this->mVariables.back().mLocalVariables.push_back(LocalVariable(
				inOriginal.mVariables[i].mLocalVariables[j].first,
				inOriginal.mVariables[i].mLocalVariables[j].second));
		}
	}
}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle Demography::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	Demography::Handle lCopy = new Demography();
	
	for (unsigned int i = 0; i < this->mVariables.size(); i++) {
		lCopy->mVariables.push_back(Variable(
			this->mVariables[i].mLabel,
			Core::castHandleT<Core::PrimitiveTree>(this->mVariables[i].mInitTree->deepCopy(inSystem))));
		for (unsigned int j = 0; j < this->mVariables[i].mLocalVariables.size(); j++) {
			lCopy->mVariables.back().mLocalVariables.push_back(LocalVariable(
				this->mVariables[i].mLocalVariables[j].first,
				Core::castHandleT<Core::AnyType>(this->mVariables[i].mLocalVariables[j].second->clone())));
		}
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Simulation::Demography::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Demography::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" + getName() + "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	PACC::XML::Document lDocument;
	std::string lFile, lLabel;

	lFile = inIter->getAttribute("file");
	if (lFile.empty()) {
#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading %s\n", getName().c_str());
#endif
		mVariables.clear();
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			if (lChild->getType() == PACC::XML::eData) {
				readVariable(lChild, ioSystem);
			}
		}
	} else {
#ifdef SCHNAPS_FULL_DEBUG
	printf("Opening file: %s\n", lFile.c_str());
#endif
		lDocument.parse(lFile);
		this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Demography::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Demography::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	for (unsigned int i = 0; i < mVariables.size(); i++) {
		ioStreamer.openTag("Variable");
		ioStreamer.insertAttribute("label", mVariables[i].mLabel);
		
		ioStreamer.openTag("LocalVariables");
		for (unsigned int j = 0; j < mVariables[i].mLocalVariables.size(); j++) {
			ioStreamer.openTag("LocalVariable");
			ioStreamer.insertAttribute("label", mVariables[i].mLocalVariables[j].first);
			ioStreamer.insertAttribute("type", mVariables[i].mLocalVariables[j].second->getType());
			ioStreamer.insertAttribute("value", mVariables[i].mLocalVariables[j].second->writeStr());
			ioStreamer.closeTag();
		}
		ioStreamer.closeTag();
		
		mVariables[i].mInitTree->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
}

/*!
 * \brief Read variable from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Demography::readVariable(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getValue() != "Variable") {
		std::ostringstream lOSS;
		lOSS << "tag <Variable> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label attribute expected!");
	}
	
	mVariables.push_back(Variable(inIter->getAttribute("label"), new Core::PrimitiveTree()));
	
#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading variable: %s\n", mVariables.back().mLabel.c_str());
#endif
	
	// read local variables
	inIter = inIter->getFirstChild();
	readLocalVariables(inIter, ioSystem);
	
	// read variable init tree
	inIter++;
	mVariables.back().mInitTree->readWithSystem(inIter, ioSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Demography::readVariable(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Read local variables from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Demography::readLocalVariables(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getValue() != "LocalVariables") {
		std::ostringstream lOSS;
		lOSS << "tag <LocalVariables> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	Core::AnyType::Alloc::Handle lAlloc;
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
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
		mVariables.back().mLocalVariables.push_back(std::pair<std::string, Core::AnyType::Handle>(lChild->getAttribute("label"), NULL));
		
#ifdef SCHNAPS_FULL_DEBUG
		printf("\tReading local variable: %s\n", mVariables.back().mLocalVariables.back().first.c_str());
#endif

		// retrieve value
		if (lChild->getAttribute("type").empty()) {
			throw schnaps_IOExceptionNodeM(*lChild, "type attribute expected!");
		}
		if (lChild->getAttribute("value").empty()) {
			throw schnaps_IOExceptionNodeM(*lChild, "value attribute expected!");
		}
		lAlloc = Core::castHandleT<Core::AnyType::Alloc>(ioSystem.getFactory().getAllocator(lChild->getAttribute("type")));
		mVariables.back().mLocalVariables.back().second = Core::castHandleT<Core::AnyType>(lAlloc->allocate());
		mVariables.back().mLocalVariables.back().second->readStr(lChild->getAttribute("value"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Demography::readLocalVariables(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief  Return the number of variables contained in demography.
 * \return The number of variables contained in demography.
 */
unsigned int Demography::getVariablesSize() const {
	schnaps_StackTraceBeginM();
	return mVariables.size();
	schnaps_StackTraceEndM("unsigned int SCHNAPS::Simulation::Demography::getVariablesSize() const");
}

/*!
 * \brief  Return a const reference to a specific variable of demography.
 * \param  inIndex the index of variable.
 * \return A const reference to a specific variable of demography.
 * \thorw  SCHNAPS::Core::AssertException if inIndex is out of bounds.
 */
const Demography::Variable& Demography::getVariable(unsigned int inIndex) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inIndex, mVariables.size()-1);
	return mVariables[inIndex];
	schnaps_StackTraceEndM("const SCHNAPS::Simulation::Demography::Variable& SCHNAPS::Simulation::Demography::getVariable(unsigned int) const");
}
