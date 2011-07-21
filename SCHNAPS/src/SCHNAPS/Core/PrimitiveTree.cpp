/*
 * PrimitiveTree.cpp
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

#include <algorithm>
#include <cstring>

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Construct a node as a copy of an original.
 * \param inOriginal A const reference to the original.
 */
Node::Node(const Node& inOriginal) :
	mPrimitive(inOriginal.mPrimitive),
	mSubTreeSize(inOriginal.mSubTreeSize)
{}

/*!
 *  \brief Construct a node.
 *  \param inPrimitive A handle to the primitive referred by the node.
 *  \param inSubTreeSize Sub-tree size, including actual node.
 */
Node::Node(Primitive::Handle inPrimitive, unsigned int inSubTreeSize) :
		mPrimitive(inPrimitive),
		mSubTreeSize(inSubTreeSize)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
bool Node::operator==(const Node& inRightNode) const {
	schnaps_StackTraceBeginM();
	return (mPrimitive == inRightNode.mPrimitive) && (mSubTreeSize == inRightNode.mSubTreeSize);
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Node::operator==(const SCHNAPS::Core::Node&) const");
}

/*!
 * \brief Construct a primitive tree as a copy of an original.
 * \param inOriginal A const reference to the original.
 */
PrimitiveTree::PrimitiveTree(const PrimitiveTree& inOriginal) {
	this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
}

/*!
 *  \brief Construct a primitive tree of the size given.
 *  \param inSize Size of the tree.
 */
PrimitiveTree::PrimitiveTree(unsigned int inSize) :
	std::vector<Node>(inSize)
{}

/*!
 *  \brief Construct a primitive tree with the given root.
 *  \param inRoot A handle to the root of the tree.
 */
PrimitiveTree::PrimitiveTree(Primitive::Handle inRoot) :
	std::vector<Node>(1)
{
	(*this)[0].mPrimitive = inRoot;
	(*this)[0].mSubTreeSize = 1;
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
PrimitiveTree& PrimitiveTree::operator=(const PrimitiveTree& inOriginal) {
	schnaps_StackTraceBeginM();
	this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::PrimitiveTree& SCHNAPS::Core::PrimitiveTree::operator=(const SCHNAPS::Core::PrimitiveTree&)");
}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \param  ioSystem A const reference to the system.
 * \return A handle to a deep copy of the object.
 */
Object::Handle PrimitiveTree::deepCopy(const System& inSystem) const {
	schnaps_StackTraceBeginM();
	PrimitiveTree::Handle lCopy = new PrimitiveTree();
	lCopy->reserve(this->size());
	for (unsigned int i = 0; i < this->size(); i++) {
		lCopy->push_back(Node(
			castHandleT<Primitive>(inSystem.getFactory().getAllocator(this->at(i).mPrimitive->getName())->clone(*this->at(i).mPrimitive)),
			this->at(i).mSubTreeSize));
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Core::PrimitiveTree::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void PrimitiveTree::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter) {
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		// Get size attribute and reserve size accordingly
		std::string lSizeText = inIter->getAttribute("size");
		if (lSizeText.empty() == false) {
			reserve(str2uint(lSizeText));
		}

		PACC::XML::ConstIterator lChild = inIter->getFirstChild();
		if ((!lChild) || (lChild->getType() != PACC::XML::eData)) {
			throw schnaps_IOExceptionNodeM(*lChild, "expected a XML tag for the primitive tree!");
		}
		clear();
		readSubTree(lChild, ioSystem);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::PrimitiveTree::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 * \throw SCHNAPS::Core::AssertException if 
 */
void PrimitiveTree::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("size", uint2str(size()));
	if(size() > 0) {
		unsigned int lSizeSubTree = writeSubTree(ioStreamer, 0, inIndent);
		schnaps_AssertM(lSizeSubTree == size());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::PrimitiveTree::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Interpret the primitive tree.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the result of the interpretation.
 * \throw  SCHNAPS::Core::AssertException if the primitive tree is empty.
 */
AnyType::Handle PrimitiveTree::interpret(ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(empty() == false);
	ioContext.setPrimitiveTree(this);
	return (*this)[0].mPrimitive->execute(0, ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::Atom::Handle SCHNAPS::Core::PrimitiveTree::interpret(SCHNAPS::Core::ExecutionContext&)");
}

/*!
 *  \brief  Return a const reference to the type of data returned by the primitive tree.
 *  \param  ioContext A reference to the execution context.
 *  \return A const reference to the the type of data returned by the primitive tree.
 */
const std::string& PrimitiveTree::getReturnType(ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	ioContext.setPrimitiveTree(this);
	return (*this)[0].mPrimitive->getReturnType(0, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::PrimitiveTree::getReturnType(SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief  Valiadate the primitive tree.
 *  \param  ioContext A reference to the execution context.
 *  \throw  SCHNAPS::Core::InternalException if the primitive tree is not valid.
 */
void PrimitiveTree::validate(ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	ioContext.setPrimitiveTree(this);
	if ((*this)[0].mPrimitive->isValid(0, ioContext)) {
		return;
	} else {
		throw schnaps_InternalExceptionM("Primitive tree is not valid!");
	}
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::PrimitiveTree::getReturnType(SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief  Read a primitive subtree from a XML subtree.
 *  \param  inIter XML iterator to read primitive tree from.
 *  \param  ioSystem A reference to the system.
 *  \return The size of the subtree read.
 *  \throw  SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
unsigned int PrimitiveTree::readSubTree(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}

	Primitive::Alloc::Handle lPrimitiveAlloc =  castHandleT<Primitive::Alloc>(ioSystem.getFactory().getAllocator(inIter->getValue()));
	if(lPrimitiveAlloc == NULL) {
		std::ostringstream lOSS;
		lOSS << "no primitive named '" <<  inIter->getValue();
		lOSS << "' found in the factory";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	Primitive::Handle lPrimitive =  castHandleT<Primitive>(lPrimitiveAlloc->allocate());
	unsigned int lNodeIdx = size();
	push_back(Node(lPrimitive, 0));
	(*this)[lNodeIdx].mPrimitive->readWithSystem(inIter, ioSystem);

	unsigned int lSubTreeSize = 1;
	unsigned int lNbChild = 0;

	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			lSubTreeSize += readSubTree(lChild, ioSystem);
			lNbChild++;
		}
	}
	if ((*this)[lNodeIdx].mPrimitive->getNumberArguments() == Primitive::eAny) {
		(*this)[lNodeIdx].mPrimitive->setNumberArguments(lNbChild);
	} else {
		if (lNbChild != (*this)[lNodeIdx].mPrimitive->getNumberArguments()) {
			std::ostringstream lOSS;
			lOSS << "number of arguments stated by the primitives (";
			lOSS << (*this)[lNodeIdx].mPrimitive->getNumberArguments();
			lOSS << ") mismatch the number of arguments read for the tree node (";
			lOSS << lNbChild << ")!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
	}
	(*this)[lNodeIdx].mSubTreeSize = lSubTreeSize;
	return lSubTreeSize;
	schnaps_StackTraceEndM("void SCHNAPS::Core::PrimitiveTree::readSubTree(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 *  \brief Write a primitive sub-tree into a XML streamer.
 *  \param ioStreamer A reference to the XML streamer to write the tree into.
 *  \param inN Index of the actual subtree root node in the vector.
 *  \param inIndent Whether XML output should be indented.
 *  \throw SCHNAPS::Core::AssertException if the subtree size doesn't match.
 */
unsigned int PrimitiveTree::writeSubTree(PACC::XML::Streamer& ioStreamer, unsigned int inN, bool inIndent) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN < size());
	unsigned int lNumberArguments = (*this)[inN].mPrimitive->getNumberArguments();
	ioStreamer.openTag((*this)[inN].mPrimitive->getName(), inIndent);
	(*this)[inN].mPrimitive->writeContent(ioStreamer, inIndent);
	
	unsigned int lSubTreeSize = 1;
	for (unsigned int i = 0; i < lNumberArguments; ++i) {
		lSubTreeSize += writeSubTree(ioStreamer, (lSubTreeSize+inN), inIndent);
	}
	schnaps_AssertM(lSubTreeSize == (*this)[inN].mSubTreeSize);
	
	ioStreamer.closeTag();
	return lSubTreeSize;
	schnaps_StackTraceEndM("void SCHNAPS::Core::PrimitiveTree::writeSubTree(PACC::XML::Streamer&, bool) const");
}
