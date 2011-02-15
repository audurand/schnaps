/*
 * Float.hpp
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
 *  \brief Float class, the float implementation as Object.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Float");
			return lName;
		schnaps_StackTraceEndM("const std::string& Float::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Float");
			return lType;
		schnaps_StackTraceEndM("const std::string& Float::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Float& operator=(const Float& inOriginal);

	virtual void readStr(std::string& inStr);
	virtual std::string writeStr() const;
	virtual AnyType::Handle clone() const;

	virtual Number& abs();
	virtual Number& add(Number& inRightNumber);
	virtual Number& div(Number& inRightNumber);
	virtual Number& exp();
	virtual Number& mult(Number& inRightNumber);
	virtual Number& pow(Number& inRightNumber);
	virtual Number& sub(Number& inRightNumber);

	operator Double();
	operator Float();
	operator Int();
	operator Long();
	operator UInt();
	operator ULong();

	//! Return the value of boolean object.
	inline const float& getValue() const {
		return mValue;
	}

	//! Set the value of boolean object.
	inline void setValue(const float& inValue) {
		mValue = inValue;
	}

private:
	float mValue; //! Value of float object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Float_hpp
