/*
 * Allocator.hpp
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
 *  \brief Object allocators of the framework, part of the Object Oriented Foundations.
 *
 *  An important design choice that gives flexibility to SCHNAPS is the allocators, a kind of
 *  object factory that generates objects in an abstract fashion. An abstract allocator class named
 *  Allocator is implemented to define the methods to create objects on the heap.
 *  \code
 *  class Allocator : public Object {
 *  public:
 *    virtual Object* allocate() const =0;
 *    virtual Object* clone(const Object&) const =0;
 *    virtual void    copy(Object&, const Object&) const =0;
 *  };
 *  \endcode
 *
 *  The purpose of the allocators is to provide factory methods to create and clone new types of
 *  objects derived from SCHNAPS constituents. With such mechanism, any user could create new type
 *  of objects that redefine the default one and use associated allocators that return pointers to
 *  this new type of objects. This mechanism is similar and coherent with the \e Factory \e Method
 *  design pattern. Usually, an allocator is used to allocate objects through their default
 *  constructor and clones the objects by calling their copy constructor. An allocator can also copy
 *  an existing object of a given type into an other. To simplify the task, a simple standard
 *  parametrized allocator named AllocatorT is defined to override the virtual method of the
 *  abstract class Allocator. Like the smart pointers \c Handle type, each component of
 * SCHNAPS has a nested allocator type called \c Alloc. The users are encouraged to define it
 *  in their classes.
 *  \code
 *  class AnyClass : public SuperClass {
 *  public:
 *    typedef PointerT<AnyClass,SuperClass::Handle>  Handle;
 *    typedef AllocatorT<AnyClass,SuperClass::Alloc> Alloc
 *  };
 *  \endcode
 *  However, the template AllocatorT cannot be used to define an \c Alloc type for an
 *  abstract type. The reason is that the method \c allocate and \c clone cannot be implemented
 *  to instantiate object of the abstract allocated type. To solve this problem, another templated
 *  allocator type is implemented, AbstractAllocT. This allocator does not define the methods
 *  \c allocate and \c clone, but is usable in the same way than AllocatorT as an allocator
 *  type for abstract types.
 *
 */

#ifndef SCHNAPS_Core_Allocator_hpp
#define SCHNAPS_Core_Allocator_hpp

#include "SCHNAPS/Core/Object.hpp"

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AbstractAllocT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class Allocator SCHNAPS/Core/Allocator.hpp "SCHNAPS/Core/Allocator.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Basic allocator class.
 */
class Allocator: public Object {
public:
	//! Allocator allocator type.
	typedef AbstractAllocT<Allocator, Object::Alloc> Alloc;
	//! Allocator handle type.
	typedef PointerT<Allocator, Object::Handle> Handle;
	//! Allocator bag type.
	typedef ContainerT<Allocator, Object::Bag> Bag;

	Allocator() {}
	virtual ~Allocator() {}

	//! Allocate a new object on the heap.
	virtual Object* allocate() const;

	//! Allocate a new object on the heap that is a clone of an existing object.
	virtual Object* clone(const Object& inOriginal) const;

	//! Copy an object to another.
	virtual void copy(Object& outCopy, const Object& inOriginal) const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Allocator_hpp
