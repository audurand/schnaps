/*
 * Vector.hpp
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

#ifndef SCHNAPS_Core_Vector_hpp
#define SCHNAPS_Core_Vector_hpp

#include "SCHNAPS/Core/AnyType.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

#include <vector>

namespace SCHNAPS {
namespace Core {

/*!
 * \class Vector SCHNAPS/Core/Vector.hpp "SCHNAPS/Core/Vector.hpp"
 * \brief The implementation of a (std) vector as a type.
 */
class Vector: public AnyType, public std::vector<AnyType::Handle> {
public:
	//! Vector allocator type.
	typedef AllocatorT<Vector, AnyType::Alloc> Alloc;
	//! Vector handle type.
	typedef PointerT<Vector, AnyType::Handle> Handle;
	//! Vector bag type.
	typedef ContainerT<Vector, AnyType::Bag> Bag;

	Vector() {}
	Vector(const Vector& inOriginal);
	virtual ~Vector() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Vector");
		return lName;
		schnaps_StackTraceEndM("const std::string& Vector::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Vector");
		return lType;
		schnaps_StackTraceEndM("const std::string& Vector::getType() const");
	}

	//! Copy operator.
	Vector& operator=(const Vector& inOriginal);

	//! Read the object from XML using System.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write the content of the object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if an object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if an object is less than another.
	virtual bool isLess(const Object& inRightObj) const;

	//! Read data from string.
	virtual void readStr(const std::string& inStr);
	//! Write data to string.
	virtual std::string writeStr() const;
	
	//! Return a handle to a clone (deep copy).
	virtual AnyType::Handle clone() const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_Vector_hpp */
