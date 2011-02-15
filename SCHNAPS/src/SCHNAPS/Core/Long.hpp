/*
 * Long.hpp
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

#ifndef SCHNAPS_Core_Long_hpp
#define SCHNAPS_Core_Long_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class Double;
class Float;
class Int;
class UInt;
class ULong;

//! Array of the atomic long type.
typedef ArrayT<long> LongArray;

/*!
 *  \class Long SCHNAPS/Core/Long.hpp "SCHNAPS/Core/Long.hpp"
 *  \brief Long class, the long implementation as Object.
 */
class Long: public Number {
public:
	//! Long allocator type.
	typedef AllocatorT<Long, Number::Alloc> Alloc;
	//! Long handle type.
	typedef PointerT<Long, Number::Handle> Handle;
	//! Long bag type.
	typedef ContainerT<Long, Number::Bag> Bag;

	Long() {}
	Long(long inValue);
	Long(const Long& inOriginal);
	virtual ~Long() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Long");
			return lName;
		schnaps_StackTraceEndM("const std::string& Long::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Long");
			return lType;
		schnaps_StackTraceEndM("const std::string& Long::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Long& operator=(const Long& inOriginal);

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

	//! Return the value of long object.
	const long& getValue() const {
		return mValue;
	}

	//! Set the value of long object.
	void setValue(const long& inValue) {
		mValue = inValue;
	}

private:
	long mValue; //! Value of long object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Long_hpp
