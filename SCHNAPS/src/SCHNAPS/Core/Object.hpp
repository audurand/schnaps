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
 *  \file   SCHNAPS/Core/Object.hpp
 *  \brief  Class Object and related functions/operators.
 */

/*!
 *  \brief Object, smart pointers, and casting functions, part of the Object Oriented Foundations.
 *
 *  In SCHNAPS, most classes are derived from an abstract class called core::Object. A
 *  SCHNAPS object comprises some general methods that are characteristic of a complete C++
 *  object. The methods core::Object::read and core::Object::write are used to
 *  extract/insert an object into devices, in XML.
 *
 *  A core::Object also implements a reference counter that remembers the number of references
 *  that point to it. The C++ language defines two ways of allocating an object instance. First, the
 *  instance can be allocated on the stack, it will then be destructed at the end of the scope.
 *  Second, the instance can be allocated on the heap with a call to the \c new operator, which
 *  means that it is necessary to apply a symmetrical \c delete at the end of the instance
 *  lifetime. With SCHNAPS, there is a variation on the second way to allocate objects: a SCHNAPS
 *  object can be allocated on the heap and affected to SCHNAPS smart pointers that
 *  interact with the object reference counter. The destruction of the object is then the
 *  responsibility of the smart pointers.
 *
 *  A SCHNAPS smart pointer acts like a standard C++ pointer that manage the memory used by the
 *  pointed objects. It is implemented it in class core::Pointer, which is tightly linked to the
 *  reference counter of class core::Object. The implementation of the core::Pointer class
 *  guarantees that the pointed object exists since there is a smart pointer that refer to it. It also
 *  guarantees that the object will be destroyed when its last reference will vanish. This emulates
 *  the appreciated garbage collection mechanism of some programming languages. In fact, the SCHNAPS
 *  smart pointers are coherent with the creation and the garbage collecting of objects of the
 *  \e Java language. The two things that the user should remember is to allocate objects on the heap
 *  (with a \c new) and never interfere with object destruction after assigning them to smart
 *  pointers. Once an object is referred by smart pointer, the memory management responsibility is
 *  held by the smart pointer.
 *
 *  Exceptionally, the core::Pointer class and its subclasses do not inherit from superclass
 *  core::Object. Like a C++ pointer, a SCHNAPS pointer can be assigned to a null pointer. The
 *  class also provides the two usual dereferencing operators, \c operator* and \c operator->,
 *  which return a reference to the pointed object. There is also two comparison operators
 *  (\c operator== and \c operator!=) between two core::Pointer, between a core::Pointer and an
 *  core::Object* and the null pointer testing operator (\c operator!).
 *
 *  Since any instance are concrete objects and smart pointers gives references to the abstract
 *  core::Object type, each access to methods or attributes that are not declared in the object
 *  interface needs a cast of the reference given by the smart pointer to the desired derived type.
 *  This could lead to inelegant code, or even type inconsistencies if old C-style casts were used. To
 *  avoid these problems, there is a templated class, core::PointerT, that defines the pointer
 *  unreferencing operators to the desired type.
 *  \code
 *  template <class T, class BaseType>
 *  class PointerT : public BaseType {
 *  public:
 *    inline T& operator*();
 *    inline T* operator->();
 *  };
 *  \endcode
 *  The core::PointerT template also emulates the mechanism of automatic pointer type binding for
 *  inheritance-related classes. This allows a compile-time binding of T-type smart pointers
 *  that inherit from the \c BaseType, when a \c BaseType object is needed. For example, suppose a
 *  method taking an argument of the type \c Base::Handle, which is a smart pointer to an
 *  object instance of the class \c Base. With the automatic type binding of smart pointer, the
 *  method can get as argument a smart pointer to the type \c Derived::Handle, supposing that the
 *  class \c Derived inherits from the class \c Base. This can be done without any explicit cast
 *  of the smart pointers. core::PointerT has two templated parameters: the type of object handled,
 *  the T-type, and the parent type of the smart pointer, the \c BaseType. The \c BaseType is useful
 *  for the automatic type binding emulation by inheritance.
 *
 *  For each class of SCHNAPS, the nested type \c Handle is declared. This is the type of
 *  handle to the class, that is, a smart pointer that gives exact reference type. Usually, this type
 *  is simply declared as a synonym of a parametrized core::PointerT.
 *  \code
 *  class AnyClass : public SuperClass {
 *  public:
 *    typedef PointerT<AnyClass,SuperClass::Handle> Handle;
 *    ...
 *  };
 *  \endcode
 *  Doing so, a smart pointer can be used to return reference to the right type, the type \c
 *  AnyClass, simply by working with the right handle type, \c AnyClass::Handle. It is a good
 *  practice to define the nested \c Handle type for every class that inherits directly or
 *  indirectly from an core::Object.
 *
 *  With the ANSI/ISO C++ standard, it is highly recommended to use the new style cast operators
 *  instead of the old C-style one. This new style cast operators, such the operators \c static_cast
 *  and \c const_cast, are more specialized cast operations. The use of different cast operators
 *  prevent some unwanted casting operations that could lead to nasty hidden bugs. In the core
 *  namespace, three new cast operator is defined, core::castObjectT(core::Object*),
 *  core::castObjectT(core::Object&), and core::castHandleT(core::Object::Handle). These
 *  operators must be use to cast any SCHNAPS object type into another object type. The usage of
 *  this casting operator is identical to the usage of new style cast operators.
 *
 */

#ifndef core_Object_hpp
#define core_Object_hpp

#include "PACC/XML.hpp"

#include "SCHNAPS/macros.hpp"

namespace core {

// forward declarations.
class Allocator;
class Pointer;
class Container;
class System;

/*!
 *  \class Object SCHNAPS/Core/Object.hpp "SCHNAPS/Core/Object.hpp"
 *  \brief Base class for common objects.
 */
class Object {
public:
	//! Object allocator type.
	typedef Allocator Alloc;
	//! Object handle type.
	typedef Pointer Handle;
	//! Object bag type.
	typedef Container Bag;

	Object();
	Object(const Object& inOriginal);
	virtual ~Object();

	//! Return the name of object.
	virtual const std::string& getName() const;

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	Object& operator=(const Object& inOriginal);

	/*!
	 *  \brief  Gives the number of smart pointers that refer to the actual object.
	 *  \return Value of the reference counter.
	 */
	inline unsigned int getRefCounter() const {
		return mRefCounter;
	}

	/*!
	 *  \brief  Increments the reference counter and returns a pointer to the actual object.
	 *  \return Pointer to the actual object.
	 */
	inline Object* refer() {
		++mRefCounter;
		return this;
	}

	/*!
	 *  \brief  Decrement the reference counter and deletes the actual object if it reaches zero.
	 */
	inline void unrefer() {
		if ((--mRefCounter) == 0)
			delete this;
	}

private:
	unsigned int mRefCounter; //!< Number of smart pointers that refer to the actual object.
};
}

#endif // core_Object_hpp
