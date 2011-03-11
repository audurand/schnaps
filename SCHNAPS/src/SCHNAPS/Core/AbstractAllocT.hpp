/*
 * AbstractAllocT.hpp
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

#ifndef SCHNAPS_Core_AbstractAllocT_hpp
#define SCHNAPS_Core_AbstractAllocT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 * \class AbstractAllocT SCHNAPS/Core/AbstractAllocT.hpp "SCHNAPS/Core/AbstractAllocT.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Abstract templated allocator class.
 * \param T Type (abstract) of object allocated.
 * \param BaseType Base type from which the actual allocator type is derived.
 * \par Note:
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

	//!Allocate a new object on the heap.
	virtual Object* allocate() const =0;

	//! Allocate a new object on the heap that is a clone of an existing object.
	virtual Object* clone(const Object& inOriginal) const =0;

	//! Copy an object to another.
	virtual void copy(Object& outCopy, const Object& inOriginal) const =0;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_AbstractAllocT_hpp
