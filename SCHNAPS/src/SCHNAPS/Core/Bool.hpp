/*
 * Bool.hpp
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

#ifndef SCHNAPS_Core_Bool_hpp
#define SCHNAPS_Core_Bool_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"

namespace SCHNAPS {
namespace Core {

//! \brief Array of the atomic boolean type.
typedef ArrayT<bool> BoolArray;

/*!
 *  \class Bool SCHNAPS/Core/Bool.hpp "SCHNAPS/Core/Bool.hpp"
 *  \brief The boolean implementation as Object.
 */
class Bool: public Atom {
public:
	//! Bool allocator type.
	typedef AllocatorT<Bool, Atom::Alloc> Alloc;
	//! Bool handle type.
	typedef PointerT<Bool, Atom::Handle> Handle;
	//! Bool bag type.
	typedef ContainerT<Bool, Atom::Bag> Bag;

	Bool() {};
	Bool(bool inValue);
	Bool(const Bool& inOriginal);
	virtual ~Bool() {}

	//! Copy operator.
	Bool& operator=(const Bool& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Bool");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Bool::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Bool");
		return lType;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Bool::getType() const");
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
	 * \brief Return a const reference to the value of boolean.
	 * \return A const reference to the value of boolean.
	 */
	const bool& getValue() const {
		return mValue;
	}

	/*!
	 * \brief Set the value of boolean.
	 * \param inValue A const reference to the new value.
	 */
	void setValue(const bool& inValue) {
		mValue = inValue;
	}

private:
	bool mValue; //! Value of boolean.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Bool_hpp
