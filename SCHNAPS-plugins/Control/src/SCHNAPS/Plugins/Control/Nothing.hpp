/*
 * Nothing.hpp
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

#ifndef SCHNAPS_Plugins_Control_Nothing_hpp
#define SCHNAPS_Plugins_Control_Nothing_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Nothing SCHNAPS/Plugins/Control/Nothing.hpp "SCHNAPS/Plugins/Control/Nothing.hpp"
 *  \brief Nothing primitive class.
 */
class Nothing: public Core::Primitive {
public:
	//! Nothing allocator type.
	typedef Core::AllocatorT<Nothing, Core::Primitive::Alloc> Alloc;
	//! Nothing handle type.
	typedef Core::PointerT<Nothing, Core::Primitive::Handle> Handle;
	//! Nothing bag type.
	typedef Core::ContainerT<Nothing, Core::Primitive::Bag> Bag;

	Nothing();
	Nothing(const Nothing& inOriginal);
	virtual ~Nothing() {}

	//! Copy operator.
	Nothing& operator=(const Nothing& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_Nothing");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Nothing::getName() const");
	}

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Nothing_hpp */
