/*
 * Atom.hpp
 *
 *  Created on: 2009-02-17
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

#ifndef core_Atom_hpp
#define core_Atom_hpp

#include "SCHNAPS/Core/AnyType.hpp"

namespace core {

/*!
 *  \class Atom SCHNAPS/Core/Atom.hpp "SCHNAPS/Core/Atom.hpp"
 *  \brief Atom class, the implementation of any (std) base types as a SCHNAPS Object.
 */
class Atom: public AnyType {
public:
	//! Atom allocator type.
	typedef AbstractAllocT<Atom, AnyType::Alloc> Alloc;
	//! Atom handle type.
	typedef PointerT<Atom, AnyType::Handle> Handle;
	//! Atom bag type.
	typedef ContainerT<Atom, AnyType::Bag> Bag;

	Atom() {}
	Atom(const Atom& inOriginal) {}
	virtual ~Atom() {}

	//! Return the name of object.
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Atom");
			return lName;
		schnaps_StackTraceEndM("const std::string& core::Atom::getName() const");
	}

	//! Return the type of data (for use with type manager).
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
			const static std::string lType("Atom");
			return lType;
		schnaps_StackTraceEndM("const std::string& core::Atom::getType() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
};
} // end of core namespace

#endif /* core_Atom_hpp */
