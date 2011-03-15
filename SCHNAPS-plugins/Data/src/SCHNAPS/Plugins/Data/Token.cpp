/*
 * TokenT.cpp
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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 * \brief Default constructor.
 */
Token::Token() :
	Primitive(0),
	mTokenHandle(NULL)
{}

/*!
 * \brief Construct a token primitive as a copy of an original.
 * \param inOriginal A const reference to the original token.
 */
Token::Token(const Token& inOriginal) :
	Primitive(0),
	mTokenHandle(Core::castHandleT<Core::Atom>(inOriginal.mTokenHandle->clone()))
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Token& Token::operator=(const Token& inOriginal) {
	schnaps_StackTraceBeginM();
	mTokenHandle = Core::castHandleT<Core::Atom>(inOriginal.mTokenHandle->clone());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Data::Token& SCHNAPS::Plugins::Data::Token::operator=(const SCHNAPS::Plugins::Data::Token&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if type attribute is missing.
 * \throw SCHNAPS::Core::IOException if value attribute is missing.
 */
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
	mTokenHandle->readStr(inIter->getAttribute("value"));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Token::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 * \throw SCHNAPS::Core::AssertException if the handle to token value is NULL. 
 */
void Token::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mTokenHandle);
	ioStreamer.insertAttribute("type", mTokenHandle->getName());
	ioStreamer.insertAttribute("value", mTokenHandle->writeStr());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Token::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw SCHNAPS::Core::AssertException if the handle to token value is NULL. 
 */
Core::AnyType::Handle Token::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mTokenHandle);
	return mTokenHandle->clone();
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Data::Token::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw SCHNAPS::Core::AssertException if the handle to token value is NULL. 
 */
const std::string& Token::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mTokenHandle);
	return mTokenHandle->getType();
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Token::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
