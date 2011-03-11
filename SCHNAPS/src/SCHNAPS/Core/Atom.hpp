/*
 * Atom.hpp
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

#ifndef SCHNAPS_Core_Atom_hpp
#define SCHNAPS_Core_Atom_hpp

#include "SCHNAPS/Core/AnyType.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class Atom SCHNAPS/Core/Atom.hpp "SCHNAPS/Core/Atom.hpp"
 *  \brief The implementation of any (std) base types as a SCHNAPS Object.
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

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Atom");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Atom::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the type of object.
	 * \return A const reference to the type of object.
	 */
	virtual const std::string& getType() const {
		schnaps_StackTraceBeginM();
		const static std::string lType("Atom");
		return lType;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Atom::getType() const");
	}

	//! Read the object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Read the object from XML using System.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_Atom_hpp */
