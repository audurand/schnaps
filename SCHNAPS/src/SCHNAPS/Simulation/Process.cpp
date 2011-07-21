/*
 * Process.cpp
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
 * \brief Default constructor.
 */
Process::Process() :
	mLabel(""),
	mPrimitiveTree(NULL)
{}

/*!
 * \brief Construct a process as a copy of an original.
 * \pram inOriginal A const reference to the original process.
 */
Process::Process(const Process& inOriginal) :
	mLabel(inOriginal.mLabel),
	mPrimitiveTree(inOriginal.mPrimitiveTree)
{}

/*!
 * \brief Construct a process with specific label and function tree.
 * \param inLabel A const reference to the label.
 * \param inPrimitiveTree A handle to the primitive tree function.
 */
Process::Process(const std::string& inLabel, Core::PrimitiveTree::Handle inPrimitiveTree) :
	mLabel(inLabel.c_str()),
	mPrimitiveTree(inPrimitiveTree)
{}

/*!
 * \brief  Return a handle to a deep copy of the process.
 * \param  A const reference to the system.
 * \return A handle to a deep copy of the process.
 */
Process::Handle Process::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	Process::Handle lCopy = new Process(mLabel, Core::castHandleT<Core::PrimitiveTree>(mPrimitiveTree->deepCopy(inSystem)));
	for (unsigned int i = 0; i < this->mLocalVariables.size(); i++) {
		lCopy->mLocalVariables.push_back(std::pair<std::string, Core::AnyType::Handle>(
			this->mLocalVariables[i].first,
			this->mLocalVariables[i].second));
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::Process::Handle SCHNAPS::Simulation::Process::deepCopy(const SCHNAPS::Core::System&) const ");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if a label attribute is missing.
 */
void Process::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	if (inIter->getAttribute("file").empty()) {
		// retrieve process label
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "process label attribute expected!");
		}
		mLabel.assign(inIter->getAttribute("label"));
		
		// retrieve local variables
		inIter = inIter->getFirstChild();
		readLocalVariables(inIter, ioSystem);
		
		// retrieve execution tree
		inIter++;
		mPrimitiveTree = new Core::PrimitiveTree();
		mPrimitiveTree->readWithSystem(inIter, ioSystem);
	} else {
		std::ifstream lIFS(inIter->getAttribute("file").c_str(), std::ios::in);
		PACC::XML::Document lDocument(lIFS);
		this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
		lIFS.close();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Process::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Process::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("label", mLabel);
	
	// write process local variables
	ioStreamer.openTag("LocalVariables");
	for (unsigned int i = 0; i < mLocalVariables.size(); i++) {
		ioStreamer.openTag("LocalVariable");
		ioStreamer.insertAttribute("label", mLocalVariables[i].first);
		ioStreamer.insertAttribute("type", mLocalVariables[i].second->getType());
		ioStreamer.insertAttribute("value", mLocalVariables[i].second->writeStr());
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	
	// write process execution tree
	mPrimitiveTree->write(ioStreamer, inIndent);
}

/*!
 * \brief Read local variables from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label, type or value attributes are missing.
 */
void Process::readLocalVariables(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getValue() != "LocalVariables") {
		std::ostringstream lOSS;
		lOSS << "tag <LocalVariables> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	mLocalVariables.clear();
	
	Core::AnyType::Handle lValue;
	Core::Object::Alloc::Handle lAlloc;
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
		
#ifdef SCHNAPS_FULL_DEBUG
		printf("\tReading local variable: %s\n", lChild->getAttribute("label").c_str());
#endif

		lAlloc = ioSystem.getFactory().getAllocator(lChild->getFirstChild()->getValue());
		lValue = Core::castHandleT<Core::AnyType>(lAlloc->allocate());
		lValue->readWithSystem(lChild->getFirstChild(), ioSystem);
		
		mLocalVariables.push_back(std::pair<std::string, Core::AnyType::Handle>(lChild->getAttribute("label"), lValue));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Process::readLocalVariables(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief  Return a handle to the result of process execution.
 * \param  ioContext A reference to the execution context required for primitive tree function execution.
 * \return A handle to the result of process execution.
 */
Core::AnyType::Handle Process::execute(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	SimulationContext& lContext = Core::castObjectT<SimulationContext&>(ioContext);
	Core::AnyType::Handle lResult;
	
	// set process local variables
	for (unsigned int i = 0; i < mLocalVariables.size(); i++) {
		lContext.insertLocalVariable(
			mLocalVariables[i].first,
			Core::castHandleT<Core::AnyType>(mLocalVariables[i].second->clone()));
	}
	lResult = mPrimitiveTree->interpret(ioContext);
	lContext.clearLocalVariables();
	
	return lResult;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Simulation::Process::execute(SCHNAPS::Core::ExecutionContext&) const ");
}

/*!
 * \brief  Return a const reference to the type of process execution result.
 * \param  ioContext A reference to the execution context required for getting primitive tree function return type.
 * \return A const reference to the type of process execution result.
 */
const std::string& Process::getReturnType(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	return mPrimitiveTree->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(SCHNAPS::Core::ExecutionContext&) const ");
}

/*!
 * \brief Validate the process.
 * \param ioContext A reference to the execution context required for the primitive tree function validation.
 */
void Process::validate(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	mPrimitiveTree->validate(ioContext);
	return;
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(SCHNAPS::Core::ExecutionContext&) const ");
}
