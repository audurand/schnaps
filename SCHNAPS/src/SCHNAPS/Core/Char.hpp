/*
 * Char.hpp
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

#ifndef SCHNAPS_Core_Char_hpp
#define SCHNAPS_Core_Char_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class Char SCHNAPS/Core/Char.hpp "SCHNAPS/Core/Char.hpp"
 *  \brief The char implementation as Object.
 */
class Char: public Atom {
public:
	//! Char allocator type.
	typedef AllocatorT<Char, Atom::Alloc> Alloc;
	//! Char handle type.
	typedef PointerT<Char, Atom::Handle> Handle;
	//! Char bag type.
	typedef ContainerT<Char, Atom::Bag> Bag;

	Char() {};
	Char(char inValue);
	Char(const Char& inOriginal);
	virtual ~Char() {}

	//! Copy operator.
	Char& operator=(const Char& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Char");
		return lName;
		schnaps_StackTraceEndM("const std::string& Char::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Char");
		return lType;
		schnaps_StackTraceEndM("const std::string& Char::getType() const");
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
	 * \brief Return a const reference to the value of char.
	 * \return A const reference to the value of char.
	 */
	const char& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of char.
	 * \param inValue A const reference to the new value.
	 */
	void setValue(const char& inValue) {
		mValue = inValue;
	}

private:
	char mValue; //! Value of char.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Char_hpp
