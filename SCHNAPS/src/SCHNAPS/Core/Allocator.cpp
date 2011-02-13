/*
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
 *  \file   SCHNAPS/Core/Allocator.cpp
 *  \brief  Implementation of class Allocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 */

#include "SCHNAPS/Core.hpp"

using namespace core;

/*!
 *  \brief  Allocate a new object on the heap.
 *  \return Pointer to the allocated object.
 */
core::Object* core::Allocator::allocate() const {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
		Object* lObject = new Object;
		if(!lObject) throw schnaps_InternalExceptionM("Out-of-memory!");
		return lObject;
#else // SCHNAPS_NDEBUG
		return new Object;
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("core::Object* core::Allocator::allocate() const");
}

/*!
 *  \brief  Allocate a new object on the heap that is a clone of an existing object.
 *  \param  inOriginal Constant reference to the original object to clone.
 *  \return Pointer to the allocated object.
 */
core::Object* core::Allocator::clone(const core::Object& inOriginal) const {
  schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
	  Object* lObject = new Object(inOriginal);
	  if(!lObject) throw schnaps_InternalExceptionM("Out-of-memory!");
	  return lObject;
#else // SCHNAPS_NDEBUG
	  return new Object(inOriginal);
#endif // SCHNAPS_NDEBUG
  schnaps_StackTraceEndM("core::Object* core::Allocator::clone(const core::Object&) const");
}

/*!
 *  \brief Copy an object to another.
 *  \param outCopy Constant reference to the original object to copy.
 *  \param inOriginal  Reference to the object that is a copy of the original.
 */
void core::Allocator::copy(core::Object& outCopy, const core::Object& inOriginal) const {
  schnaps_StackTraceBeginM();
	  outCopy = inOriginal;
  schnaps_StackTraceEndM("void core::Allocator::copy(core::Object&, const core::Object&) const");
}

