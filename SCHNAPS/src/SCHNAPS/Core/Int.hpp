/*
 * Int.hpp
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

#ifndef core_Int_hpp
#define core_Int_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace core {

class Double;	// Forward declaration
class Float;	// Forward declaration
class Long;		// Forward declaration
class UInt;		// Forward declaration
class ULong;	// Forward declaration

//! Array of the atomic integer type.
typedef ArrayT<int> IntArray;

/*!
 *  \class Int SCHNAPS/Core/Int.hpp "SCHNAPS/Core/Int.hpp"
 *  \brief Int class, the double implementation as Object.
 */
class Int: public Number {
public:
	//! Int allocator type.
	typedef AllocatorT<Int, Number::Alloc> Alloc;
	//! Int handle type.
	typedef PointerT<Int, Number::Handle> Handle;
	//! Int bag type.
	typedef ContainerT<Int, Number::Bag> Bag;

	Int() {}
	Int(int inValue);
	Int(const Int& inOriginal);
	virtual ~Int() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Int");
			return lName;
		schnaps_StackTraceEndM("const std::string& Int::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Int");
			return lType;
		schnaps_StackTraceEndM("const std::string& Int::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Int& operator=(const Int& inOriginal);

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

	//! Return the value of integer object.
	const int& getValue() const {
		return mValue;
	}

	//! Set the value of integer object.
	void setValue(const int& inValue) {
		mValue = inValue;
	}

private:
	int mValue; //! Value of integer object.
};
} // end of core namespace

#endif // core_Int_hpp
