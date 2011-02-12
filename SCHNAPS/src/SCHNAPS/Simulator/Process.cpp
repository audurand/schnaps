/*
 * Process.cpp
 *
 *  Created on: 2009-02-26
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

Process::Process() :
	mPrimitiveTree(NULL)
{}

Process::Process(const Process& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mPrimitiveTree(inOriginal.mPrimitiveTree)
{}

Process::Process(const std::string inLabel, core::PrimitiveTree::Handle inPrimitiveTree) :
	mLabel(inLabel.c_str()),
	mPrimitiveTree(inPrimitiveTree)
{}

void simulator::Process::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
			mPrimitiveTree = new core::PrimitiveTree();
			mPrimitiveTree->readWithSystem(inIter->getFirstChild(), ioSystem);
		} else {
			std::ifstream lIFS(inIter->getAttribute("file").c_str(), std::ios::in);
			PACC::XML::Document lDocument(lIFS);
			this->readWithSystem(lDocument.getFirstDataTag(), ioSystem);
			lIFS.close();
		}
	schnaps_StackTraceEndM("void simulator::Process::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void simulator::Process::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("label", mLabel);
	mPrimitiveTree->write(ioStreamer, inIndent);
}

core::AnyType::Handle Process::execute(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return mPrimitiveTree->interpret(ioContext);
	schnaps_StackTraceEndM("core::AnyType::Handle simulator::Process::execute(core::ExecutionContext&) const ");
}

const std::string& Process::getReturnType(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		return mPrimitiveTree->getReturnType(ioContext);
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(core::ExecutionContext&) const ");
}

void Process::validate(core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		mPrimitiveTree->validate(ioContext);
		return;
	schnaps_StackTraceEndM("const std::string& Process::getReturnType(core::ExecutionContext&) const ");
}

Process::Handle Process::deepCopy(const core::System& ioSystem) const {
	schnaps_StackTraceBeginM();
		return new Process(mLabel.c_str(), mPrimitiveTree->deepCopy(ioSystem));
	schnaps_StackTraceEndM("simulator::Process::Handle simulator::Process::deepCopy(const core::System&) const ");
}
