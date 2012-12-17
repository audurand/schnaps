/*
 * Value.hpp
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

#ifndef SCHNAPS_Plugins_Data_Value_hpp
#define SCHNAPS_Plugins_Data_Value_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class Value SCHNAPS-plugins/Data/Value.hpp "SCHNAPS-plugins/Data/Value.hpp"
 *  \brief Get a value of any type.
 */
class Value: public Core::Primitive {
public:
	//! Value allocator type.
	typedef Core::AllocatorT<Value, Core::Primitive::Alloc> Alloc;
	//! Value handle type.
	typedef Core::PointerT<Value, Core::Primitive::Handle> Handle;
	//! Value bag type.
	typedef Core::ContainerT<Value, Core::Primitive::Bag> Bag;

	Value();
	Value(const Value& inOriginal);
	virtual ~Value() {}

	//! Copy operator.
	Value& operator=(const Value& inOriginal);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Data_Value");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Value::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mValue_Ref; 		//!< Reference to the value.
	Core::AnyType::Handle mValue;	//!< A Handle to the value.
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_Value_hpp */
