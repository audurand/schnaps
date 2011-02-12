/*
 * Double.hpp
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

#ifndef core_Double_hpp
#define core_Double_hpp

#include "SCHNAPS/Core/Number.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace core {

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
 *  \brief Double class, the double implementation as Object.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Double");
			return lName;
		schnaps_StackTraceEndM("const std::string& Double::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Double");
			return lType;
		schnaps_StackTraceEndM("const std::string& Double::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Double& operator=(const Double& inOriginal);

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;
	virtual AnyType::Handle clone() const;

	virtual Number& abs();
	virtual Number& add(Number& inRightNumber);
	virtual Number& div(Number& inRightNumber);
	virtual Number& exp();
	virtual Number& mult(Number& inRightNumber);
	virtual Number& pow(Number& inRightNumber);
	virtual Number& sub(Number& inRightNumber);

	operator Double() const;
	operator Float() const;
	operator Int() const;
	operator Long() const;
	operator UInt() const;
	operator ULong() const;

	//! Return the value of double object.
	const double& getValue() const {
		return mValue;
	}

	//! Set the value of double object.
	void setValue(const double& inValue) {
		mValue = inValue;
	}

private:
	double mValue; //! Value of double object.
};
} // end of core namespace

#endif // core_Double_hpp
