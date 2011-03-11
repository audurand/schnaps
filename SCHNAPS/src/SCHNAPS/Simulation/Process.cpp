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
 * \brief  Return a handle to a deep copy of the object.
 * \param  A const reference to the system.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle Process::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	return new Process(mLabel, Core::castHandleT<Core::PrimitiveTree>(mPrimitiveTree->deepCopy(inSystem)));
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Simulation::Process::deepCopy(const SCHNAPS::Core::System&) const ");
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
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "process label attribute expected!");
		}
		mLabel = inIter->getAttribute("label");
		mPrimitiveTree = new Core::PrimitiveTree();
		mPrimitiveTree->readWithSystem(inIter->getFirstChild(), ioSystem);
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
void SCHNAPS::Simulation::Process::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("label", mLabel);
	mPrimitiveTree->write(ioStreamer, inIndent);
}

/*!
 * \brief  Return a handle to the result of process execution.
 * \param  ioContext A reference to the execution context required for primitive tree function execution.
 * \return A handle to the result of process execution.
 */
Core::AnyType::Handle Process::execute(Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	return mPrimitiveTree->interpret(ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Simulation::Process::execute(SCHNAPS::Core::ExecutionContext&) const ");
}

/*!
 * \brief  Return a const reference to the type of process execution result.
 * \param  ioContext A reference to the execution context required for getting primitive tree function return type.
 * \return A const reference to the type of process execution result.
 */
const std::string& Process::getReturnType(SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	return mPrimitiveTree->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(SCHNAPS::Core::ExecutionContext&) const ");
}

/*!
 * \brief Validate the process.
 * \param ioContext A reference to the execution context required for the primitive tree function validation.
 */
void Process::validate(SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	mPrimitiveTree->validate(ioContext);
	return;
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(SCHNAPS::Core::ExecutionContext&) const ");
}
