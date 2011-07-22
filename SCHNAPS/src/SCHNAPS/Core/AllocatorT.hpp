/*
 * AllocatorT.hpp
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

#ifndef SCHNAPS_Core_AllocatorT_hpp
#define SCHNAPS_Core_AllocatorT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/castObjectT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/InternalException.hpp"
#include "SCHNAPS/Core/BadCastException.hpp"

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;

/*!
 * \class AllocatorTSCHNAPS/Core/Allocator.hpp "SCHNAPS/Core/Allocator.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Templated class defining a T-type allocator.
 * \param T Type of object allocated.
 * \param BaseType Base type from which the actual allocator type is derived.
 */
template<class T, class BaseType>
class AllocatorT: public BaseType {
public:
	//! AllocatorT allocator type.
	typedef AllocatorT<AllocatorT<T, BaseType> , typename BaseType::Alloc> Alloc;
	//! AllocatorT handle type.
	typedef PointerT<AllocatorT<T, BaseType> , typename BaseType::Handle> Handle;
	//! AllocatorT bag type.
	typedef ContainerT<AllocatorT<T, BaseType> , typename BaseType::Bag> Bag;

	AllocatorT() {}
	virtual ~AllocatorT() {}

	//! Allocate a new T-type object and the heap.
	virtual Object* allocate() const;

	//! Allocate a new T-type object on the heap that is a clone of an existing object.
	virtual Object* clone(const Object& inOrigObj) const;

	//! Copy a T-type object into another.
	virtual void copy(Object& outCopy, const Object& inOriginal) const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

/*!
 * \brief  Allocate a new T-type object on the heap.
 * \return Pointer to the allocated T-type object.
 */
template<class T, class BaseType>
SCHNAPS::Core::Object* SCHNAPS::Core::AllocatorT<T, BaseType>::allocate() const {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
	T* lT = new T;
	if (!lT)
		throw schnaps_InternalExceptionM("Out of memory!");
	return lT;
#else // SCHNAPS_NDEBUG
	return new T;
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("Object* AllocatorT<T,BaseType>::allocate() const");
}

/*!
 * \brief  Allocate a new T-type object on the heap that is a clone of an existing object.
 * \param  inOriginal Constant reference to the original T-type object to clone.
 * \return Pointer to the allocated T-type object.
 * \throw  BasCastException If the type of the parameter is not as specified.
 */
template<class T, class BaseType>
SCHNAPS::Core::Object* SCHNAPS::Core::AllocatorT<T, BaseType>::clone(const SCHNAPS::Core::Object& inOriginal) const {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
	const T& lOrigT = castObjectT<const T&> (inOriginal);
	T* lCopy = new T(lOrigT);
	if (!lCopy)
		throw schnaps_InternalExceptionM("Out of memory!");
	return lCopy;
#else // SCHNAPS_NDEBUG
	const T& lOrigT = castObjectT<const T&>(inOriginal);
	return new T(lOrigT);
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("Object* AllocatorT<T,BaseType>::clone(const Object&) const");
}

/*!
 * \brief Copy a T-type object into another.
 * \param outCopy Reference to the object that is a copy of the original.
 * \param inOriginal Constant reference to the original object to copy.
 * \throw BasCastException If the types of the parameters are not as specified.
 */
template<class T, class BaseType>
void SCHNAPS::Core::AllocatorT<T, BaseType>::copy(SCHNAPS::Core::Object& outCopy, const SCHNAPS::Core::Object& inOriginal) const {
	schnaps_StackTraceBeginM();
	T& lCopyT = castObjectT<T&> (outCopy);
	const T& lOriginalT = castObjectT<const T&> (inOriginal);
	lCopyT = lOriginalT;
	schnaps_StackTraceEndM("void AllocatorT<T,BaseType>::copy(Object&, const Object&) const");
}

#endif // SCHNAPS_Core_AllocatorT_hpp
