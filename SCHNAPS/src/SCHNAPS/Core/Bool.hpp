/*
 * Bool.hpp
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
 *  \brief Bool class, the boolean implementation as Object.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Bool");
			return lName;
		schnaps_StackTraceEndM("const std::string& Bool::getName() const");
	}

	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Bool");
			return lType;
		schnaps_StackTraceEndM("const std::string& Bool::getType() const");
	}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Bool& operator=(const Bool& inOriginal);

	virtual void readStr(const std::string& inStr);
	virtual std::string writeStr() const;

	virtual AnyType::Handle clone() const;

	//! Return the value of boolean object.
	const bool& getValue() const {
		return mValue;
	}

	//! Set the value of boolean object.
	void setValue(const bool& inValue) {
		mValue = inValue;
	}

private:
	bool mValue; //! Value of boolean object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Bool_hpp
