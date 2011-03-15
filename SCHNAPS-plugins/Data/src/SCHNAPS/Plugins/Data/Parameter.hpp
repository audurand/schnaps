/*
 * Parameter.hpp
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

#ifndef SCHNAPS_Plugins_Data_Parameter_hpp
#define SCHNAPS_Plugins_Data_Parameter_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class Parameter SCHNAPS/Plugins/Data/Parameter.hpp "SCHNAPS/Plugins/Data/Parameter.hpp"
 *  \brief Access the value of a simulation parameter.
 */
class Parameter: public Core::Primitive {
public:
	//! Parameter allocator type.
	typedef Core::AllocatorT<Parameter, Core::Primitive::Alloc> Alloc;
	//! Parameter handle type.
	typedef Core::PointerT<Parameter, Core::Primitive::Handle> Handle;
	//! Parameter bag type.
	typedef Core::ContainerT<Parameter, Core::Primitive::Bag> Bag;

	Parameter();
	Parameter(const Parameter& inOriginal);
	virtual ~Parameter() {}

	//! Copy operator.
	Parameter& operator=(const Parameter& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Data_Parameter");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Parameter::getName() const");
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
	std::string mLabel;						//!< Label of parameter.
	Core::AnyType::Handle mParameterHandle;	//!< A handle to the parameter.
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_Parameter_hpp */
