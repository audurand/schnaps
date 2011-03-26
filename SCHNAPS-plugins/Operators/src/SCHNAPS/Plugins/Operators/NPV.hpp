/*
 * NPV.hpp
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

#ifndef SCHNAPS_Plugins_Operators_NPV_hpp
#define SCHNAPS_Plugins_Operators_NPV_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class NPV SCHNAPS/Plugins/Operators/NPV.hpp "SCHNAPS/Plugins/Operators/NPV.hpp"
 *  \brief Nearest present value operator with specific rate to compute actualisation relative to the simulation time.
 */
class NPV: public Core::Primitive {
public:
	//! NPV allocator type.
	typedef Core::AllocatorT<NPV, Core::Primitive::Alloc> Alloc;
	//! NPV handle type.
	typedef Core::PointerT<NPV, Core::Primitive::Handle> Handle;
	//! NPV bag type.
	typedef Core::ContainerT<NPV, Core::Primitive::Bag> Bag;

	NPV();
	NPV(const NPV& inOriginal);
	virtual ~NPV() {}

	//! Copy operator.
	NPV& operator=(const NPV& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Operators_NPV");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NPV::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mRate_Ref;		//!< Reference to actualization rate.
	Core::Double::Handle mRate;	//!< A handle to the actualization rate.
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_NPV_hpp */
