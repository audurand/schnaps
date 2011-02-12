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

#ifndef core_AbstractAllocT_hpp
#define core_AbstractAllocT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"

namespace core {

/*!
 *  \class AbstractAllocT SCHNAPS/Core/AbstractAllocT.hpp "SCHNAPS/Core/AbstractAllocT.hpp"
 *  \brief Abstract templated allocator class.
 *  \param T Type (abstract) of object allocated.
 *  \param BaseType Base type from which the actual allocator type is derived.
 *  \par Note:
 *    This class is usually used to define the Alloc type of an abstract class.
 */
template<class T, class BaseType>
class AbstractAllocT: public BaseType {
public:
	//! AbstractAllocT allocator type.
	typedef AbstractAllocT<AbstractAllocT<T, BaseType> , typename BaseType::Alloc> Alloc;
	//! AbstractAllocT handle type.
	typedef PointerT<AbstractAllocT<T, BaseType> , typename BaseType::Handle> Handle;
	//! AbstractAllocT bag type.
	typedef ContainerT<AbstractAllocT<T, BaseType> , typename BaseType::Bag> Bag;

	AbstractAllocT() {}
	virtual ~AbstractAllocT() {}

	/*!
	 *  \brief  Allocate a new object on the heap.
	 *  \return Pointer to the allocated object.
	 */
	virtual Object* allocate() const =0;

	/*!
	 *  \brief  Allocate a new object on the heap that is a clone of an existing object.
	 *  \param  inOriginal Constant reference to the original object to clone.
	 *  \return Pointer to the allocated object.
	 */
	virtual Object* clone(const Object& inOriginal) const =0;

	/*!
	 *  \brief Copy an object to another.
	 *  \param outCopy Constant reference to the original object to copy.
	 *  \param inOriginal  Reference to the object that is a copy of the original.
	 */
	virtual void copy(Object& outCopy, const Object& inOriginal) const =0;
};
}

#endif // core_AbstractAllocT_hpp
