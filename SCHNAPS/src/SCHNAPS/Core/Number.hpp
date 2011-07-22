/*
 * Number.hpp
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

#ifndef SCHNAPS_Core_Number_hpp
#define SCHNAPS_Core_Number_hpp

#include "SCHNAPS/Core/Atom.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Double;
class Int;
class Long;
class UInt;
class ULong;

/*!
 *  \class Number SCHNAPS/Core/Number.hpp "SCHNAPS/Core/Number.hpp"
 *  \brief Base class for the implementation of any number types as a Object.
 */
class Number: public Atom {
public:
	//! Number allocator type.
	typedef AbstractAllocT<Number, Atom::Alloc> Alloc;
	//! Number handle type.
	typedef PointerT<Number, Atom::Handle> Handle;
	//! Number bag type.
	typedef ContainerT<Number, Atom::Bag> Bag;

	Number() {}
	Number(const Number& inOriginal) {}
	virtual ~Number() {}

	//! Copy operator.
	Number& operator=(const Number& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Number");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Number::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Number");
		return lType;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Number::getType() const");
	}

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
	//! Compute the raise to the power exponent.
	virtual Number::Handle pow(Number& inRightNumber);
	//! Compute the subtraction by a number.
	virtual Number::Handle sub(Number& inRightNumber);

	//! Casting operator to double.
	virtual operator Double() const;
	//! Casting operator to integer.
	virtual operator Int() const;
	//! Casting operator to long.
	virtual operator Long() const;
	//! Casting operator to unsigned integer.
	virtual operator UInt() const;
	//! Casting operator to unsigned long.
	virtual operator ULong() const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_Number_hpp */
