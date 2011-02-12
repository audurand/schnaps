/*
 * Char.hpp
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

#ifndef core_Char_hpp
#define core_Char_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"

namespace core {
/*!
 *  \class Char SCHNAPS/Core/Char.hpp "SCHNAPS/Core/Char.hpp"
 *  \brief Char class, the char implementation as Object.
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

	Char& operator=(const Char& inOriginal);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Char");
			return lName;
		schnaps_StackTraceEndM("const std::string& Char::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Char");
			return lType;
		schnaps_StackTraceEndM("const std::string& Char::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;
	virtual AnyType::Handle clone() const;

	//! Return the value of char object.
	const char& getValue() const {
		return mValue;
	}

	//! Set the value of char object.
	void setValue(const char& inValue) {
		mValue = inValue;
	}

private:
	char mValue; //! Value of charobject.
};
} // end of core namespace

#endif // core_Char_hpp
