/*
 * Allocator.cpp
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

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief  Allocate a new object on the heap.
 * \return Pointer to the allocated object.
 */
SCHNAPS::Core::Object* SCHNAPS::Core::Allocator::allocate() const {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
	Object* lObject = new Object;
	if(!lObject) throw schnaps_InternalExceptionM("Out-of-memory!");
	return lObject;
#else // SCHNAPS_NDEBUG
	return new Object;
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("SCHNAPS::Core::Object* SCHNAPS::Core::Allocator::allocate() const");
}

/*!
 * \brief  Allocate a new object on the heap that is a clone of an existing object.
 * \param  inOriginal Constant reference to the original object to clone.
 * \return Pointer to the allocated object.
 */
SCHNAPS::Core::Object* SCHNAPS::Core::Allocator::clone(const SCHNAPS::Core::Object& inOriginal) const {
	schnaps_StackTraceBeginM();
	#ifndef SCHNAPS_NDEBUG
	Object* lObject = new Object(inOriginal);
	if(!lObject) throw schnaps_InternalExceptionM("Out-of-memory!");
	return lObject;
	#else // SCHNAPS_NDEBUG
	return new Object(inOriginal);
	#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("SCHNAPS::Core::Object* SCHNAPS::Core::Allocator::clone(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Copy an object to another.
 * \param outCopy Constant reference to the original object to copy.
 * \param inOriginal  Reference to the object that is a copy of the original.
 */
void SCHNAPS::Core::Allocator::copy(SCHNAPS::Core::Object& outCopy, const SCHNAPS::Core::Object& inOriginal) const {
	schnaps_StackTraceBeginM();
	outCopy = inOriginal;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Allocator::copy(SCHNAPS::Core::Object&, const SCHNAPS::Core::Object&) const");
}
