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

#ifndef core_AllocatorT_hpp
#define core_AllocatorT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/castObjectT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/InternalException.hpp"
#include "SCHNAPS/Core/BadCastException.hpp"

namespace core {

// Forward declarations.
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;

/*!
 *  \class AllocatorTSCHNAPS/Core/Allocator.hpp "SCHNAPS/Core/Allocator.hpp"
 *  \brief Templated class defining a T-type allocator.
 *  \param T Type of object allocated.
 *  \param BaseType Base type from which the actual allocator type is derived.
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
}

/*!
 *  \brief  Allocate a new T-type object and the heap.
 *  \return Pointer to the allocated T-type object.
 */
template<class T, class BaseType>
core::Object* core::AllocatorT<T, BaseType>::allocate() const {
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
 *  \brief  Allocate a new T-type object on the heap that is a clone of an existing object.
 *  \param  inOriginal Constant reference to the original T-type object to clone.
 *  \return Pointer to the allocated T-type object.
 *  \throw  BasCastException If the type of the parameter is not as specified.
 */
template<class T, class BaseType>
core::Object* core::AllocatorT<T, BaseType>::clone(const core::Object& inOriginal) const {
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
 *  \brief Copy a T-type object into another.
 *  \param outCopy Reference to the object that is a copy of the original.
 *  \param inOriginal Constant reference to the original object to copy.
 *  \throw BasCastException If the types of the parameters are not as specified.
 */
template<class T, class BaseType>
void core::AllocatorT<T, BaseType>::copy(core::Object& outCopy, const core::Object& inOriginal) const {
	schnaps_StackTraceBeginM();
		T& lCopyT = castObjectT<T&> (outCopy);
		const T& lOriginalT = castObjectT<const T&> (inOriginal);
		lCopyT = lOriginalT;
	schnaps_StackTraceEndM("void AllocatorT<T,BaseType>::copy(Object&, const Object&) const");
}

#endif // core_AllocatorT_hpp
