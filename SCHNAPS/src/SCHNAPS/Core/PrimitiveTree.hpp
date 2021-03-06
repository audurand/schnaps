/*
 * PrimitiveTree.hpp
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

#ifndef SCHNAPS_Core_PrimitiveTree_hpp
#define SCHNAPS_Core_PrimitiveTree_hpp

#include "SCHNAPS/Core/Primitive.hpp"

#include <vector>

namespace SCHNAPS {
namespace Core {

/*!
 *  \struct Node SCHNAPS/Core/PrimitiveTree.hpp "SCHNAPS/Core/PrimitiveTree.hpp"
 *  \brief Tree node structure for using in vectors.
 */
struct Node {
	Primitive::Handle mPrimitive;	//!< Smart pointer to the associated primitive.
	unsigned int mSubTreeSize;		//!< Sub-tree size, including actual node.

	Node(const Node& inOriginal);
	explicit Node(Primitive::Handle inPrimitive = NULL, unsigned int inSubTreeSize = 0);
	bool operator==(const Node& inRightNode) const;
};

/*!
 *  \class Tree SCHNAPS/Core/PrimitiveTree.hpp "SCHNAPS/Core/PrimitiveTree.hpp"
 *  \brief Tree class to store primitives.
 */
class PrimitiveTree: public Object, public std::vector<Node> {
public:
	//! PrimitiveTree allocator type.
	typedef AllocatorT<PrimitiveTree, Object::Alloc> Alloc;
	//! PrimitiveTree handle type.
	typedef PointerT<PrimitiveTree, Object::Handle> Handle;
	//! PrimitiveTree bag type.
	typedef ContainerT<PrimitiveTree, Object::Bag> Bag;

	PrimitiveTree(const PrimitiveTree& inOriginal);
	explicit PrimitiveTree(unsigned int inSize = 0);
	explicit PrimitiveTree(Primitive::Handle inRoot);

	//! Copy operator.
	PrimitiveTree& operator=(const PrimitiveTree& inOriginal);

	//! Return a handle to a deep copy of the object.
	Object::Handle deepCopy(const System& inSystem) const;

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("PrimitiveTree");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::PrimitiveTree::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Interpret the primitive tree.
	AnyType::Handle interpret(ExecutionContext& ioContext) const;
	//! Return the tag of the type of data return by the primitive tree.
	const std::string& getReturnType(ExecutionContext& ioContext) const;
	//! Validate the primitive tree.
	void validate(ExecutionContext& ioContext) const;

private:
	//! Read a primitive subtree from a XML subtree.
	unsigned int readSubTree(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Write a primitive sub-tree into a XML streamer.
	unsigned int writeSubTree(PACC::XML::Streamer& ioStreamer, unsigned int inN, bool inIndent = true) const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_PrimitiveTree_hpp */
