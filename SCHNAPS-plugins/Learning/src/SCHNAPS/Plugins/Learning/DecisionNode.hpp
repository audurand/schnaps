/*
 * DecisionNode.hpp
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

#ifndef SCHNAPS_Plugins_Learning_DecisionNode_hpp
#define SCHNAPS_Plugins_Learning_DecisionNode_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

#include <vector>

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class DecisionNode SCHNAPS/Plugins/Learning/DecisionNode.hpp "SCHNAPS/Plugins/Learning/DecisionNode.hpp"
 *  \brief Make DecisionNode among different options.
 */
class DecisionNode: public Core::Primitive {
public:
	//! DecisionNode allocator type.
	typedef Core::AllocatorT<DecisionNode, Core::Primitive::Alloc> Alloc;
	//! DecisionNode handle type.
	typedef Core::PointerT<DecisionNode, Core::Primitive::Handle> Handle;
	//! DecisionNode bag type.
	typedef Core::ContainerT<DecisionNode, Core::Primitive::Bag> Bag;
	
	DecisionNode();
	DecisionNode(const DecisionNode& inOriginal);
	virtual ~DecisionNode() {}

	//! Copy operator.
	DecisionNode& operator=(const DecisionNode& inOriginal);
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_DecisionNode");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::DecisionNode::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	
private:
	std::string mLabel;
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_DecisionNode_hpp */
