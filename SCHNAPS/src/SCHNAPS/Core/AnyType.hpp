/*
 * AnyType.hpp
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

#ifndef SCHNAPS_Core_AnyType_hpp
#define SCHNAPS_Core_AnyType_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AbstractAllocT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class AnyType SCHNAPS/Core/AnyType.hpp "SCHNAPS/Core/AnyType.hpp"
 *  \brief The implementation of any (std) base types as Object.
 */
class AnyType: public Object {
public:
	//! AnyType allocator type.
	typedef AbstractAllocT<AnyType, Object::Alloc> Alloc;
	//! AnyType handle type.
	typedef PointerT<AnyType, Object::Handle> Handle;
	//! AnyType bag type.
	typedef ContainerT<AnyType, Object::Bag> Bag;

	AnyType() {}
	AnyType(const AnyType& inOriginal) {}
	virtual ~AnyType() {}

	//! Copy operator.
	AnyType& operator=(const AnyType& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("AnyType");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::AnyType::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("AnyType");
		return lType;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::AnyType::getType() const");
	}

	//! Read data from string.
	virtual void readStr(const std::string& inStr);
	//! Write data to string.
	virtual std::string writeStr() const;
	
	//! Return a handle to a clone (deep copy).
	virtual AnyType::Handle clone() const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_AnyType_hpp */
