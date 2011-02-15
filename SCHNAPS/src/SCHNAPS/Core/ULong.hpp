/*
 * ULong.hpp
 *
 *  Created on: 2009-01-23
 *  Updated on: 2011-01-12
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

#ifndef SCHNAPS_Core_ULong_hpp
#define SCHNAPS_Core_ULong_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {

namespace Core {

// forward declaration
class Double;
class Float;
class Int;
class Long;
class UInt;

//! Array of the atomic unsigned long type.
typedef ArrayT<unsigned long> ULongArray;

/*!
 *  \class ULong SCHNAPS/Core/ULong.hpp "SCHNAPS/Core/ULong.hpp"
 *  \brief ULong class, the unsigned long implementation as Object.
 */
class ULong: public Number {
public:
	//! ULong allocator type.
	typedef AllocatorT<ULong, Number::Alloc> Alloc;
	//! ULong handle type.
	typedef PointerT<ULong, Number::Handle> Handle;
	//! ULong bag type.
	typedef ContainerT<ULong, Number::Bag> Bag;

	ULong() {}
	ULong(unsigned long inValue);
	ULong(const ULong& inOriginal);
	virtual ~ULong() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("ULong");
			return lName;
		schnaps_StackTraceEndM("const std::string& ULong::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("ULong");
			return lType;
		schnaps_StackTraceEndM("const std::string& ULong::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	ULong& operator=(const ULong& inOriginal);

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;
	virtual AnyType::Handle clone() const;

	virtual Number& abs();
	virtual Number& add(Number& inRightNumber);
	virtual Number& div(Number& inRightNumber);
	virtual Number& mod(Number& inRightNumber);
	virtual Number& mult(Number& inRightNumber);
	virtual Number& sub(Number& inRightNumber);

	operator Double() const;
	operator Float() const;
	operator Int() const;
	operator Long() const;
	operator UInt() const;
	operator ULong() const;

	//! Return the value of unsigned long object.
	const unsigned long& getValue() const {
		return mValue;
	}

	//! Set the value of unsigned long object.
	void setValue(const unsigned long& inValue) {
		mValue = inValue;
	}

private:
	unsigned long mValue; //! Value of unsigned long object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ULong_hpp
