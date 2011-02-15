/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \brief Object oriented data structures, part of the Object Oriented Foundations.
 *
 *  The SCHNAPS container is a random access table of heterogeneous objects. It is implemented as
 *  a dynamic array of specified smart pointers. Any objects that is a specialization of the contained
 *  type could be put in a container. The constraints are the same as those of the smart pointers: the
 *  objects must be allocated on the heap (via the \c new operator) and these objects must not be
 *  unallocated explicitly (i.e. \c delete of the objects are forbidden). Our container is
 *  implemented as a Beagle object that is also a STL vector of object handles.
 *  \code
 *  class Container : public Object, public std::vector<Object::Handle> { ... };
 *  \endcode
 *  Built this way, our container could be manipulated by the STL generic algorithms. It is also possible
 *  to make container of containers, since a container is an object that can be smart pointed.
 *
 *  As with the smart pointers and allocators, every SCHNAPS type has a nested type that define a
 *  container of the specified type, the nested type named \c Bag. For example, the type that is a
 *  container of floating-point values, Float, is the type Float::Bag. The nested type
 *  is declared in classes as usual for the nested types \c Handle and \c Alloc, using a \c typedef
 *  to a template, in this case to template class ContainerT.
 *  \code
 *  class MyClass : public SuperClass {
 *  public:
 *    typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
 *    typedef PointerT<MyClass,SuperClass::Handle> Handle;
 *    typedef ContainerT<MyClass,SuperClass::Bag> Bag;
 *  };
 *  \endcode
 *
 */

#ifndef SCHNAPS_Core_Container_hpp
#define SCHNAPS_Core_Container_hpp

#include <vector>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AssertException.hpp"

namespace SCHNAPS {
namespace Core {
	
// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class ContainerT;
template<class T, class BaseType> class PointerT;

/*!
 *  \class Container SCHNAPS/Core/Container.hpp "SCHNAPS/Core/Container.hpp"
 *  \brief Basic container data structure, implemented as a vector of smart pointers.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Bag");
			return lName;
		schnaps_StackTraceEndM("const std::string& Container::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	/*!
	 *  \brief  Standard const operator for accessing elements by index.
	 *  \param  inN Index of element (first is 0).
	 *  \return Smart pointer to element.
	 */
	const Object::Handle& operator[](unsigned int inN) const {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,size()-1);
			return std::vector<Pointer>::operator[](inN);
		schnaps_StackTraceEndM("const SCHNAPS::Core::Object::Handle& SCHNAPS::Core::Container::operator[](unsigned int) const");
	}

	/*!
	 *  \brief Standard  operator for accessing elements by index.
	 *  \param inN Index of element (first is 0).
	 *  \return Smart pointer to element.
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
