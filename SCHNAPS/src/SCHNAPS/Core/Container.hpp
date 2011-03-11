/*
 * Container.hpp
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

/*!
 * \brief Object oriented data structures, part of the Object Oriented Foundations.
 *
 * The SCHNAPS container is a random access table of heterogeneous objects. It is implemented as
 * a dynamic array of specified smart pointers. Any objects that is a specialization of the contained
 * type could be put in a container. The constraints are the same as those of the smart pointers: the
 * objects must be allocated on the heap (via the \c new operator) and these objects must not be
 * unallocated explicitly (i.e. \c delete of the objects are forbidden). Our container is
 * implemented as a Beagle object that is also a STL vector of object handles.
 * \code
 * class Container : public Object, public std::vector<Object::Handle> { ... };
 * \endcode
 * Built this way, our container could be manipulated by the STL generic algorithms. It is also possible
 * to make container of containers, since a container is an object that can be smart pointed.
 *
 * As with the smart pointers and allocators, every SCHNAPS type has a nested type that define a
 * container of the specified type, the nested type named \c Bag. For example, the type that is a
 * container of floating-point values, Float, is the type Float::Bag. The nested type
 * is declared in classes as usual for the nested types \c Handle and \c Alloc, using a \c typedef
 * to a template, in this case to template class ContainerT.
 * \code
 * class MyClass : public SuperClass {
 * public:
 *   typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
 *   typedef PointerT<MyClass,SuperClass::Handle> Handle;
 *   typedef ContainerT<MyClass,SuperClass::Bag> Bag;
 * };
 * \endcode
 */

#ifndef SCHNAPS_Core_Container_hpp
#define SCHNAPS_Core_Container_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AssertException.hpp"

#include <vector>

namespace SCHNAPS {
namespace Core {
	
// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class ContainerT;
template<class T, class BaseType> class PointerT;

/*!
 * \class Container SCHNAPS/Core/Container.hpp "SCHNAPS/Core/Container.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Basic container data structure, implemented as a vector of smart pointers.
 */
class Container: public Object, public std::vector<Object::Handle> {
public:
	//! Container's allocator type.
	typedef AllocatorT<Container, Object::Alloc> Alloc;
	//! Container's handle type.
	typedef PointerT<Container, Object::Handle> Handle;
	//! Container's bag type.
	typedef ContainerT<Container, Object::Bag> Bag;

	explicit Container(unsigned int inN = 0, Object::Handle inModel = NULL);
	virtual ~Container() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Bag");
		return lName;
		schnaps_StackTraceEndM("const std::string& Container::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Read object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write object to XML.
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if object is less than another.
	virtual bool isLess(const Object& inRightObj) const;
	
	/*!
	 * \brief  Return a const handle to the Nth element of the ContainerT.
	 * \param  inN Indice of the const handle to get.
	 * \return A const handle at the indice N.
	 * \throw  SCHNAPS::Core::AssertException if the index is too large for the container.
	 */
	const Object::Handle& operator[](unsigned int inN) const {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,size()-1);
		return std::vector<Pointer>::operator[](inN);
		schnaps_StackTraceEndM("const SCHNAPS::Core::Object::Handle& SCHNAPS::Core::Container::operator[](unsigned int) const");
	}

	/*!
	 * \brief  Return a handle to the Nth element of the ContainerT.
	 * \param  inN Indice of the handle to get.
	 * \return A handle at the indice N.
	 * \throw  SCHNAPS::Core::AssertException if the index is too large for the container.
	 */
	Object::Handle& operator[](unsigned int inN) {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,size()-1);
		return std::vector<Pointer>::operator[](inN);
		schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle& SCHNAPS::Core::Container::operator[](unsigned int)");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Container_hpp
