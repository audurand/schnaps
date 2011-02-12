/*
 * UInt.hpp
 *
 *  Created on: 2009-01-23
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

#ifndef core_UInt_hpp
#define core_UInt_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace core {

// Forward declaration
class Double;
class Float;
class Int;
class Long;
class ULong;

//! Array of the atomic unsigned integer type.
typedef ArrayT<unsigned int> UIntArray;

/*!
 *  \class UInt SCHNAPS/Core/UInt.hpp "SCHNAPS/Core/UInt.hpp"
 *  \brief UInt class, the unsigned int implementation as Object.
 */
class UInt: public Number {
public:
	//! UInt allocator type.
	typedef AllocatorT<UInt, Number::Alloc> Alloc;
	//! UInt handle type.
	typedef PointerT<UInt, Number::Handle> Handle;
	//! UInt bag type.
	typedef ContainerT<UInt, Number::Bag> Bag;

	UInt() {}
	UInt(unsigned int inValue);
	UInt(const UInt& inOriginal);
	virtual ~UInt() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("UInt");
			return lName;
		schnaps_StackTraceEndM("const std::string& UInt::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("UInt");
			return lType;
		schnaps_StackTraceEndM("const std::string& UInt::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	UInt& operator=(const UInt& inOriginal);

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

	//! Return the value of unsigned integer object.
	const unsigned int& getValue() const {
		return mValue;
	}

	//! Set the value of unsigned integer object.
	void setValue(const unsigned int& inValue) {
		mValue = inValue;
	}

private:
	unsigned int mValue; //! Value of unsigned integer object.
};
} // end of core namespace

#endif // core_UInt_hpp
