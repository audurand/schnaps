/*
 * ULong.hpp
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

#ifndef SCHNAPS_Core_ULong_hpp
#define SCHNAPS_Core_ULong_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Double;
class Int;
class Long;
class UInt;

//! Array of the atomic unsigned long type.
typedef ArrayT<unsigned long> ULongArray;

/*!
 *  \class ULong SCHNAPS/Core/ULong.hpp "SCHNAPS/Core/ULong.hpp"
 *  \brief The unsigned long implementation as Object.
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

	//! Copy operator.
	ULong& operator=(const ULong& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("ULong");
		return lName;
		schnaps_StackTraceEndM("const std::string& ULong::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("ULong");
		return lType;
		schnaps_StackTraceEndM("const std::string& ULong::getType() const");
	}

	//! Write the content of the object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if an object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if an object is less than another.
	virtual bool isLess(const Object& inRightObj) const;

	//! Read the object from string.
	virtual void readStr(const std::string& inStr);
	//! Write the object to string.
	virtual std::string writeStr() const;

	//! Return a handle to a clone of the current object.
	virtual AnyType::Handle clone() const;

	//! Compute absolute value.
	virtual Number::Handle abs();
	//! Compute the addition with a number.
	virtual Number::Handle add(Number& inRightNumber);
	//! Compute the division by a number.
	virtual Number::Handle div(Number& inRightNumber);
	//! Compute the base-e exponential function, which is the e number raised to the power x.
	virtual Number::Handle exp();
	//! Compute the modulo by a number.
	virtual Number::Handle mod(Number& inRightNumber);
	//! Compute the multiplication by a number.
	virtual Number::Handle mult(Number& inRightNumber);
	//! Compute the subtraction by a number.
	virtual Number::Handle sub(Number& inRightNumber);

	//! Casting operator to double.
	operator Double() const;
	//! Casting operator to integer.
	operator Int() const;
	//! Casting operator to long.
	operator Long() const;
	//! Casting operator to unsigned integer.
	operator UInt() const;
	//! Casting operator to unsigned long.
	operator ULong() const;

	/*!
	 * \brief Return a const refernce to the value of unsigned long.
	 * \return A const reference to the value of unsigned long.
	 */
	const unsigned long& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of unsigned long.
	 * \param inValue A const reference to the new value of unsigned long.
	 */
	void setValue(const unsigned long& inValue) {
		mValue = inValue;
	}

private:
	unsigned long mValue; //! Value of unsigned long.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ULong_hpp
