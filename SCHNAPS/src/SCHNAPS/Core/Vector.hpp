/*
 * Vector.hpp
 *
 *  Created on: 2010-03-22
 *  Updated on: 2010-08-26
 *      Author: Audrey Durand
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

#ifndef core_Vector_hpp
#define core_Vector_hpp

#include <vector>

#include "SCHNAPS/Core/AnyType.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace core {

/*!
 *  \class Vector SCHNAPS/Core/Vector.hpp "SCHNAPS/Core/Vector.hpp"
 *  \brief Vector class, the implementation of a std vector as a type.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Vector");
			return lName;
		schnaps_StackTraceEndM("const std::string& Vector::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Vector");
			return lType;
		schnaps_StackTraceEndM("const std::string& Vector::getType() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Vector& operator=(const Vector& inOriginal);

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;
};
} // end of core namespace

#endif /* core_Vector_hpp */
