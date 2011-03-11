/*
 * Float.hpp
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

#ifndef SCHNAPS_Core_Float_hpp
#define SCHNAPS_Core_Float_hpp

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
class ULong;

//! Array of the atomic float type.
typedef ArrayT<float> FloatArray;

/*!
 *  \class Float SCHNAPS/Core/Float.hpp "SCHNAPS/Core/Float.hpp"
 *  \brief The float implementation as Object.
 */
class Float: public Number {
public:
	//! Float allocator type.
	typedef AllocatorT<Float, Number::Alloc> Alloc;
	//! Float handle type.
	typedef PointerT<Float, Number::Handle> Handle;
	//! Float bag type.
	typedef ContainerT<Float, Number::Bag> Bag;

	Float() {}
	Float(float inValue);
	Float(const Float& inOriginal);
	virtual ~Float() {}

	//! Copy operator.
	Float& operator=(const Float& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Float");
		return lName;
		schnaps_StackTraceEndM("const std::string& Float::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Float");
		return lType;
		schnaps_StackTraceEndM("const std::string& Float::getType() const");
	}

	//! Write the content of the object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if an object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if an object is less than another.
	virtual bool isLess(const Object& inRightObj) const;

	//! Read the object from string.
	virtual void readStr(std::string& inStr);
	//! Write the object to string.
	virtual std::string writeStr() const;

	//! Return a handle to a clone of the current object.
	virtual AnyType::Handle clone() const;

	//! Compute absolute value.
	virtual Number& abs();
	//! Add a number.
	virtual Number& add(Number& inRightNumber);
	//! Divide by a number.
	virtual Number& div(Number& inRightNumber);
	//! Compute the base-e exponential function, which is the e number raised to the power x.
	virtual Number& exp();
	//! Multiply by a number.
	virtual Number& mult(Number& inRightNumber);
	//! Raise the current number to the power exponent.
	virtual Number& pow(Number& inRightNumber);
	//! Subtract by a number.
	virtual Number& sub(Number& inRightNumber);

	//! Casting operator to double.
	operator Double() const;
	//! Casting operator to float.
	operator Float() const;
	//! Casting operator to integer.
	operator Int() const;
	//! Casting operator to long.
	operator Long() const;
	//! Casting operator to unsigned integer.
	operator UInt() const;
	//! Casting operator to unsigned long.
	operator ULong() const;

	/*!
	 * \brief Return a const refernce to the value of float.
	 * \return A const reference to the value of float.
	 */
	const float& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of float.
	 * \param inValue A const reference to the new value of float.
	 */
	void setValue(const float& inValue) {
		mValue = inValue;
	}

private:
	float mValue; //! Value of float.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Float_hpp
