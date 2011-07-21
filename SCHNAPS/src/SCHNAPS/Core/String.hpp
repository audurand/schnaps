/*
 * String.hpp
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

#ifndef SCHNAPS_Core_String_hpp
#define SCHNAPS_Core_String_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"

namespace SCHNAPS {
namespace Core {

//! Array of the atomic std::string type.
typedef ArrayT<std::string> StringArray;

/*!
 *  \class String SCHNAPS/Core/String.hpp "SCHNAPS/Core/String.hpp"
 *  \brief The (std) string implementation as Object.
 */
class String: public Atom {
public:
	//! String allocator type.
	typedef AllocatorT<String, Atom::Alloc> Alloc;
	//! String handle type.
	typedef PointerT<String, Atom::Handle> Handle;
	//! String bag type.
	typedef ContainerT<String, Atom::Bag> Bag;

	String() {}
	String(std::string inValue);
	String(const String& inOriginal);
	virtual ~String() {}

	//! Copy operator.
	String& operator=(const String& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("String");
		return lName;
		schnaps_StackTraceEndM("const std::string& String::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("String");
		return lType;
		schnaps_StackTraceEndM("const std::string& String::getType() const");
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

	/*!
	 * \brief Return a const reference to the value of strirng.
	 * \return A const reference to the value of string.
	 */
	const std::string& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of string.
	 * \param inValue A const reference to the new value.
	 */
	void setValue(const std::string& inValue) {
		mValue = inValue;
	}

private:
	std::string mValue; //! Value of string.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_String_hpp
