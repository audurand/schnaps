/*
 * String.hpp
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

#ifndef core_String_hpp
#define core_String_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ArrayT.hpp"

namespace core {

//! Array of the atomic std::string type.
typedef ArrayT<std::string> StringArray;

/*!
 *  \class String SCHNAPS/Core/String.hpp "SCHNAPS/Core/String.hpp"
 *  \brief String class, the std::string implementation as LSD Object.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("String");
			return lName;
		schnaps_StackTraceEndM("const std::string& String::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("String");
			return lType;
		schnaps_StackTraceEndM("const std::string& String::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	String& operator=(const String& inOriginal);

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;
	virtual AnyType::Handle clone() const;

	//! Return the value of string object.
	inline const std::string& getValue() const {
		return mValue;
	}

	//! Set the value of string object.
	inline void setValue(const std::string& inValue) {
		mValue = inValue;
	}

private:
	std::string mValue; //! Value of string object.
};
} // end of core namespace

#endif // core_String_hpp
