/*
 * Number.hpp
 *
 *  Created on: 2009-02-17
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

#ifndef SCHNAPS_Core_Number_hpp
#define SCHNAPS_Core_Number_hpp

#include "SCHNAPS/Core/Atom.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Double;
class Float;
class Int;
class Long;
class UInt;
class ULong;

/*!
 *  \class Number SCHNAPS/Core/Number.hpp "SCHNAPS/Core/Number.hpp"
 *  \brief Number class, the implementation of any (std) number types as a SCHNAPS object.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Number");
			return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Number::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Number");
			return lType;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Number::getType() const");
	}

	Number& operator=(const Number& inOriginal);

	//! Compute absolute value.
	virtual Number& abs();
	//! Adds a number.
	virtual Number& add(Number& inRightNumber);
	//! Divide by a number.
	virtual Number& div(Number& inRightNumber);
	//! Compute the base-e exponential function, which is the e number raised to the power x.
	virtual Number& exp();
	//! Compute the modulo by number.
	virtual Number& mod(Number& inRightNumber);
	//! Multiply by a number.
	virtual Number& mult(Number& inRightNumber);
	//! Raise to the power exponent.
	virtual Number& pow(Number& inRightNumber);
	//! Subtract a number.
	virtual Number& sub(Number& inRightNumber);

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
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_Number_hpp */
