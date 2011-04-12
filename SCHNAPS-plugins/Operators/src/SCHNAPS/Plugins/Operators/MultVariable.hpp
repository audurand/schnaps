/*
 * MultVariable.hpp
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

#ifndef SCHNAPS_Plugins_Operators_MultVariable_hpp
#define SCHNAPS_Plugins_Operators_MultVariable_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class MultVariable SCHNAPS/Plugins/Operators/MultVariable.hpp "SCHNAPS/Plugins/Operators/MultVariable.hpp"
 *  \brief Multiply left argument with right argument and store result in a variable.
 */
class MultVariable: public Core::Primitive {
public:
	//! MultVariable allocator type.
	typedef Core::AllocatorT<MultVariable, Core::Primitive::Alloc> Alloc;
	//! MultVariable handle type.
	typedef Core::PointerT<MultVariable, Core::Primitive::Handle> Handle;
	//! MultVariable bag type.
	typedef Core::ContainerT<MultVariable, Core::Primitive::Bag> Bag;

	MultVariable();
	MultVariable(const MultVariable& inOriginal);
	virtual ~MultVariable() {}

	//! Copy operator.
	MultVariable& operator=(const MultVariable& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Operators_MultVariable");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::MultVariable::getName() const");
	}

	//! Read object from XML using system.
	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mResult_Ref;		//!< Reference of variable for storing result.
	std::string mArgLeft_Ref;		//!< Reference to left argument in multiplication.
	Core::Number::Handle mArgLeft;	//!< A handle to left argument in multiplication.
	std::string mArgRight_Ref;		//!< Reference to right argument in multiplication.
	Core::Number::Handle mArgRight;	//!< A handle to right argument in multiplication.
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_MultVariable_hpp */
