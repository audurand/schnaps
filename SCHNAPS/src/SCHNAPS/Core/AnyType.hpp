/*
 * AnyType.hpp
 *
 *  Created on: 2009-11-19
 *  Author: Audrey Durand
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

#ifndef core_AnyType_hpp
#define core_AnyType_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AbstractAllocT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"

namespace core {

/*!
 *  \class AnyType SCHNAPS/Core/AnyType.hpp "SCHNAPS/Core/AnyType.hpp"
 *  \brief AnyType class, the implementation of any (std) base types as Object.
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

	AnyType& operator=(const AnyType& inOriginal);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("AnyType");
			return lName;
		schnaps_StackTraceEndM("const std::string& core::AnyType::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("AnyType");
			return lType;
		schnaps_StackTraceEndM("const std::string& core::AnyType::getType() const");
	}

	//! Read data from string.
	virtual void readStr(const std::string& inStr);
	//! Write data to string.
	virtual std::string writeStr() const;
	//! Return a clone (deep copy).
	virtual AnyType::Handle clone() const;
};
} // end of core namespace

#endif /* core_AnyType_hpp */
