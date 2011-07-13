/*
 * SetIdle.hpp
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

#ifndef SCHNAPS_Plugins_Data_SetIdle_hpp
#define SCHNAPS_Plugins_Data_SetIdle_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class SetIdle SCHNAPS/Plugins/Data/SetIdle.hpp "SCHNAPS/Plugins/Data/SetIdle.hpp"
 *  \brief Set the current individual status to idle.
 */
class SetIdle: public Core::Primitive {
public:
	//! SetIdle allocator type.
	typedef Core::AllocatorT<SetIdle, Core::Primitive::Alloc> Alloc;
	//! SetIdle handle type.
	typedef Core::PointerT<SetIdle, Core::Primitive::Handle> Handle;
	//! SetIdle bag type.
	typedef Core::ContainerT<SetIdle, Core::Primitive::Bag> Bag;

	SetIdle();
	SetIdle(const SetIdle& inOriginal);
	virtual ~SetIdle() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Data_SetIdle");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetIdle::getName() const");
	}

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_SetIdle_hpp */
