/*
 * Double.hpp
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

#ifndef SCHNAPS_Core_Double_hpp
#define SCHNAPS_Core_Double_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Float;
class Int;
class Long;
class UInt;
class ULong;

//! Array of the atomic double type.
typedef ArrayT<double> DoubleArray;

/*!
 *  \class Double SCHNAPS/Core/Double.hpp "SCHNAPS/Core/Double.hpp"
 *  \brief The double implementation as Object.
 */
class Double: public Number {
public:
	//! Double allocator type.
	typedef AllocatorT<Double, Number::Alloc> Alloc;
	//! Double handle type.
	typedef PointerT<Double, Number::Handle> Handle;
	//! Double bag type.
	typedef ContainerT<Double, Number::Bag> Bag;

	Double() {}
	Double(double inValue);
	Double(const Double& inOriginal);
	virtual ~Double() {}

	//! Copy operator.
	Double& operator=(const Double& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Double");
		return lName;
		schnaps_StackTraceEndM("const std::string& Double::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Double");
		return lType;
		schnaps_StackTraceEndM("const std::string& Double::getType() const");
	}

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

	//! Compute absolute value.
	virtual Number::Handle abs();
	//! Compute the addition with a number.
	virtual Number::Handle add(Number& inRightNumber);
	//! Compute the division by a number.
	virtual Number::Handle div(Number& inRightNumber);
	//! Compute the base-e exponential function, which is the e number raised to the power x.
	virtual Number::Handle exp();
	//! Compute the multiplication by a number.
	virtual Number::Handle mult(Number& inRightNumber);
	//! Compute the raise to the power exponent.
	virtual Number::Handle pow(Number& inRightNumber);
	//! Compute the subtraction by a number.
	virtual Number::Handle sub(Number& inRightNumber);

	//! Casting operator to double.
	virtual operator Double() const;
	//! Casting operator to float.
	virtual operator Float() const;
	//! Casting operator to integer.
	virtual operator Int() const;
	//! Casting operator to long.
	virtual operator Long() const;
	//! Casting operator to unsigned integer.
	virtual operator UInt() const;
	//! Casting operator to unsigned long.
	virtual operator ULong() const;

	/*!
	 * \brief Return a const reference to the value of double.
	 * \return A const reference to the value of double.
	 */
	const double& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of double.
	 * \param inValue A const reference to the new value.
	 */
	void setValue(const double& inValue) {
		mValue = inValue;
	}

private:
	double mValue; //! Value of double.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Double_hpp
