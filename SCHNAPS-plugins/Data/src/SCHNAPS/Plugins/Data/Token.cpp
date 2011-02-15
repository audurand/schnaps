/*
 * TokenT.cpp
 *
 *  Created on: 2009-02-12
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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 *  \brief Construct a new token terminal primitive.
 */
Token::Token() :
		Primitive(0),
		mTokenHandle(NULL)
{}

Token::Token(const Token& inOriginal) :
		Primitive(0),
		mTokenHandle(Core::castHandleT<Core::Atom>(inOriginal.mTokenHandle->clone()))
{}

void Token::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	if (inIter->getAttribute("type").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "type of token expected!");
	}
	if (inIter->getAttribute("value").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "value of token expected!");
	}

	Core::Atom::Alloc::Handle lAtomAlloc =  Core::castHandleT<Core::Atom::Alloc>(ioSystem.getFactory().getAllocator(inIter->getAttribute("type")));
	mTokenHandle =  Core::castHandleT<Core::Atom>(lAtomAlloc->allocate());
	if (mTokenHandle == NULL) {
		std::ostringstream lOSS;
		lOSS << "no atom named '" <<  inIter->getAttribute("type");
		lOSS << "' found in the factory";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	mTokenHandle->readStr(inIter->getAttribute("value"));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Token::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void Token::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mTokenHandle);
		ioStreamer.insertAttribute("type", mTokenHandle->getName());
		ioStreamer.insertAttribute("value", mTokenHandle->writeStr());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Token::writeContent(PACC::XML::Streamer&, bool) const");
}

Token& Token::operator=(const Token& inOriginal) {
	schnaps_StackTraceBeginM();
		mTokenHandle = Core::castHandleT<Core::Atom>(inOriginal.mTokenHandle->clone());
		return *this;
	schnaps_StackTraceEndM("Core::Token& SCHNAPS::Plugins::Data::Token::operator=(const Core::Token&)");
}

Core::AnyType::Handle Token::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mTokenHandle);
		return mTokenHandle->clone();
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::Token::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& Token::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (mTokenHandle != NULL) {
			return mTokenHandle->getType();
		}
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Token::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
