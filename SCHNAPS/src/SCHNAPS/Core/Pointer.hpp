/*
 * Pointer.hpp
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

#ifndef SCHNAPS_Core_Pointer_hpp
#define SCHNAPS_Core_Pointer_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AssertException.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 * \class Pointer SCHNAPS/Core/Pointer.hpp "SCHNAPS/Core/Pointer.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Basic class describing a smart pointer to an object.
 */
class Pointer {
public:
	/*!
	 * \brief Construct a NULL pointing smart pointer.
	 */
	inline Pointer() :
		mObjectPointer(NULL)
	{}

	/*!
	 * \brief Construct a smart pointer that refer to the object pointed given.
	 * \param inObjPtr A const pointer to the object to be referenced.
	 */
	inline Pointer(const Object* inObjPtr) :
		mObjectPointer((inObjPtr != NULL) ? (const_cast<Object*> (inObjPtr)->refer()) : NULL)
	{}

	/*!
	 * \brief Copy construct a smart pointer.
	 * \param inPointer A const reference to the smart pointer to copy.
	 */
	inline Pointer(const Pointer& inPointer) :
		mObjectPointer((inPointer.getPointer() != NULL) ? (const_cast<SCHNAPS::Core::Object*> (inPointer.getPointer())->refer()) : NULL)
	{}

	/*!
	 * \brief Destruct a smart pointer by unrefering the pointed object.
	 */
	inline ~Pointer() {
		schnaps_StackTraceBeginM();
		if (mObjectPointer != NULL) {
			mObjectPointer->unrefer();
		}
		mObjectPointer = NULL;
		schnaps_StackTraceEndM("SCHNAPS::Core::Pointer::~Pointer()");
	}

	/*!
	 * \brief  Copy operator. Affect the actual pointer to an object pointer.
	 * \param  inObjPtr A const pointer to the object to refer.
	 * \return A reference to the actual smart pointer.
	 */
	inline Pointer& operator=(const Object* inObjPtr) {
		schnaps_StackTraceBeginM();
		if (mObjectPointer == inObjPtr) {
			return *this;
		}
		if (mObjectPointer != NULL) {
			mObjectPointer->unrefer();
		}
		if (inObjPtr) {
			mObjectPointer = const_cast<Object*> (inObjPtr)->refer();
		} else {
			mObjectPointer = NULL;
		}
		return *this;
		schnaps_StackTraceEndM("SCHNAPS::Core::Pointer& SCHNAPS::Core::Pointer::operator=(const SCHNAPS::Core::Object*)");
	}

	/*!
	 * \brief Copy operator. Affect the actual pointer to another smart pointer.
	 * \param inPointer A const reference to the smart pointer to copy.
	 * \return A reference to the actual smart pointer.
	 */
	inline Pointer& operator=(const Pointer& inPointer) {
		schnaps_StackTraceBeginM();
		if (mObjectPointer == inPointer.getPointer())
			return *this;
		if (mObjectPointer)
			mObjectPointer->unrefer();
		if (inPointer.getPointer() != NULL)
			mObjectPointer = const_cast<Object*> (inPointer.getPointer())->refer();
		else
			mObjectPointer = NULL;
		return *this;
		schnaps_StackTraceEndM("SCHNAPS::Core::Pointer& SCHNAPS::Core::Pointer::operator=(const SCHNAPS::Core::Pointer&)");
	}

	/*!
	 *  \brief 	Return a reference the object pointed.
	 *  \return A reference to the object pointed.
	 *  \throw 	SCHNAPS::Core::AssertException if the pointer to the object is NULL.
	 */
	inline Object& operator*() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mObjectPointer);
		return *mObjectPointer;
		schnaps_StackTraceEndM("SCHNAPS::Core::Object& SCHNAPS::Core::Pointer::operator*()");
	}

	/*!
	 *  \brief  Return a const reference the object pointed.
	 *  \return A const reference to the object pointed.
	 *  \throw  SCHNAPS::Core::AssertException if the pointer to the object is NULL.
	 */
	inline const Object& operator*() const {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mObjectPointer);
		return *mObjectPointer;
		schnaps_StackTraceEndM("const Object& Pointer::operator*() const");
	}

	/*!
	 *  \brief  Return a pointer the object pointed.
	 *  \return A pointer to the object pointed.
	 *  \throw  SCHNAPS::Core::AssertException if the pointer to the object is NULL.
	 */
	inline Object* operator->() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mObjectPointer);
		return mObjectPointer;
		schnaps_StackTraceEndM("Object* Pointer::operator->()");
	}

	/*!
	 *  \brief  Return a const pointer the object pointed.
	 *  \return A const pointer to the object pointed.
	 *  \throw  SCHNAPS::Core::AssertException if the pointer to the object is NULL.
	 */
	inline const Object* operator->() const {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mObjectPointer);
		return mObjectPointer;
		schnaps_StackTraceEndM("const Object* Pointer::operator->() const");
	}

	/*!
	 *  \brief  Test whether the pointer is NULL or not.
	 *  \return True if the pointer is NULL, false if it is not.
	 */
	inline bool operator!() const {
		schnaps_StackTraceBeginM();
		return (!mObjectPointer);
		schnaps_StackTraceEndM("bool Pointer::operator!() const");
	}

	/*!
	 *  \brief  Compare a smart pointer to an object pointer.
	 *  \param  inObjPtr A const pointer to the right hand side object pointer to compare.
	 *  \return True if they both refer to the same object (or are both NULL), false if not.
	 */
	inline bool operator==(const Object* inObjPtr) const {
		schnaps_StackTraceBeginM();
		return (mObjectPointer == inObjPtr);
		schnaps_StackTraceEndM("bool Pointer::operator==(const Object* inObjPtr) const");
	}

	/*!
	 *  \brief  Compare two smart pointers.
	 *  \param  inPointer A const reference to the right hand side smart pointer to compare.
	 *  \return True if they both refer to the same object (or are both NULL), false if not.
	 */
	inline bool operator==(const Pointer& inPointer) const {
		schnaps_StackTraceBeginM();
		return (mObjectPointer == inPointer.getPointer());
		schnaps_StackTraceEndM("bool Pointer::operator==(const Pointer& inPointer) const");
	}

	/*!
	 *  \brief  Compare a smart pointer to an object pointer.
	 *  \param  inObjPtrA const pointer to the right hand side object pointer to compare.
	 *  \return False if they both refer to the same object (or are both NULL), true if not.
	 */
	inline bool operator!=(const Object* inObjPtr) const {
		schnaps_StackTraceBeginM();
		return (mObjectPointer != inObjPtr);
		schnaps_StackTraceEndM("bool Pointer::operator!=(const Object* inObjPtr) const");
	}

	/*!
	 *  \brief  Compare two smart pointers.
	 *  \param  inPointer A const reference to the right hand side smart pointer to compare.
	 *  \return False if they both refer to the same object (or are both NULL), true if not.
	 */
	inline bool operator!=(const Pointer& inPointer) const {
		schnaps_StackTraceBeginM();
		return (mObjectPointer != inPointer.getPointer());
		schnaps_StackTraceEndM("bool Pointer::operator!=(const Pointer& inPointer) const");
	}

	/*!
	 *  \brief  Return a pointer to the referenced object.
	 *  \return C++ pointer to the object.
	 */
	inline Object* getPointer() {
		schnaps_StackTraceBeginM();
		return mObjectPointer;
		schnaps_StackTraceEndM("Object* Pointer::getPointer()");
	}

	/*!
	 *  \brief  Return a const pointer to the referenced object.
	 *  \return Constant C++ pointer to the object.
	 */
	inline const Object* getPointer() const {
		schnaps_StackTraceBeginM();
		return mObjectPointer;
		schnaps_StackTraceEndM("const Object* Pointer::getPointer() const");
	}

private:
	Object* mObjectPointer; //!< Pointer to the referenced object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Pointer_hpp
