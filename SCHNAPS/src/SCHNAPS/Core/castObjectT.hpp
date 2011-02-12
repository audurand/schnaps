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

#ifndef core_castObjectT_hpp
#define core_castObjectT_hpp

#include <typeinfo>
#include <string>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Pointer.hpp"
#include "SCHNAPS/Core/BadCastException.hpp"

namespace core {
/*!
 *  \brief   Cast an Object pointer into a CastType pointer.
 *  \param   inObject Object pointer to be casted.
 *  \return  Casted pointer to the object.
 *  \relates core::Object
 *  \warning If RTTI is enabled and the typecast is invalid, the function return a NULL pointer.
 */
template<class CastPointerType>
inline CastPointerType castObjectT(core::Object* inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<CastPointerType>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		return dynamic_cast<CastPointerType> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<CastPointerType>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("CastPointerType castObjectT<CastPointerType>(Object* inObject)");
}

/*!
 *  \brief   Cast an Object constant pointer into a CastType constant pointer.
 *  \param   inObject Object constant pointer to be casted.
 *  \return  Casted constant pointer to the object.
 *  \relates core::Object
 *  \warning If RTTI is enabled and the typecast is invalid, the function return a NULL pointer.
 */
template<class CastPointerType>
inline CastPointerType castObjectT(const core::Object* inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<CastPointerType>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		return dynamic_cast<CastPointerType> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<CastPointerType>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("CastPointerType castObjectT<CastPointerType>(const Object* inObject)");
}

/*!
 *  \brief   Cast an Object reference into a CastType reference.
 *  \param   inObject Object reference to be casted.
 *  \return  Casted reference to the object.
 *  \throw   BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates core::Object
 */
template<class CastRefType>
inline CastRefType castObjectT(core::Object& inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<CastRefType>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		try {
			return dynamic_cast<CastRefType> (inObject);
		} catch (...) {
			std::string lMessage("Unable to cast object reference into desired type! ");
			lMessage += "Type of the object to cast: ";
			lMessage += typeid(inObject).name();
			lMessage += ", \t";
			lMessage += "Desired type cast: ";
			lMessage += typeid(CastRefType).name();
			lMessage += ".";
			throw core_BadCastExceptionM(lMessage);
		}
		return static_cast<CastRefType> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<CastRefType>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("CastRefType castObjectT<CastRefType>(Object& inObject)");
}

/*!
 *  \brief   Cast an Object constant reference into a CastType constant reference.
 *  \param   inObject Object constant reference to be casted.
 *  \return  Casted constant reference to the object.
 *  \throw   BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates core::Object
 */
template<class CastConstRefType>
inline CastConstRefType castObjectT(const core::Object& inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<CastConstRefType>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		try {
			return dynamic_cast<CastConstRefType> (inObject);
		} catch (...) {
			std::string lMessage("Unable to cast object reference into desired type! ");
			lMessage += "Type of the object to cast: ";
			lMessage += typeid(inObject).name();
			lMessage += ", \t";
			lMessage += "Desired type cast: ";
			lMessage += typeid(CastConstRefType).name();
			lMessage += ".";
			throw core_BadCastExceptionM(lMessage);
		}
		return static_cast<CastConstRefType> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<CastConstRefType>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("CastConstRefType castObjectT<CastConstRefType>(const Object& inObject)");
}

/*!
 *  \brief   Cast an Object smart pointer into a CastType handle type.
 *  \param   inObject Object smart pointer to be casted.
 *  \return  Casted smart pointer to the object.
 *  \throw   BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates core::Object
 */
template<class CastType>
inline typename CastType::Handle& castHandleT(core::Object::Handle& inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<typename CastType::Handle&>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		if (inObject.getPointer()) {
			if (dynamic_cast<CastType*> (inObject.getPointer()) == NULL) {
				std::string lMessage("Unable to cast object handle into desired type! ");
				lMessage += "Type of the object to cast: ";
				lMessage += typeid(*inObject).name();
				lMessage += ", \t";
				lMessage += "Desired type cast: ";
				lMessage += typeid(CastType).name();
				lMessage += ".";
				throw core_BadCastExceptionM(lMessage);
			}
		}
		return static_cast<typename CastType::Handle&> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<typename CastType::Handle&>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("CastType::Handle& castHandleT<CastType>(Object::Handle& inObject)");
}

/*!
 *  \brief   Cast a constant Object smart pointer into a constant CastType handle type.
 *  \param   inObject Constant Object smart pointer to be casted.
 *  \return  Casted constant smart pointer to the object.
 *  \throw   BadCastException If RTTI is enabled and the typecast is invalid.
 *  \relates core::Object
 */
template<class CastType>
inline const typename CastType::Handle& castHandleT(const core::Object::Handle& inObject) {
	schnaps_StackTraceBeginM();
#ifdef SCHNAPS_NDEBUG
		return static_cast<const typename CastType::Handle&>(inObject);
#else // SCHNAPS_NDEBUG
#ifdef SCHNAPS_HAVE_DYNAMIC_CAST
		if (inObject.getPointer()) {
			if (dynamic_cast<const CastType*> (inObject.getPointer()) == NULL) {
				std::string lMessage("Unable to cast object handle into desired type! ");
				lMessage += "Type of the object to cast: ";
				lMessage += typeid(*inObject).name();
				lMessage += ", \t";
				lMessage += "Desired type cast: ";
				lMessage += typeid(CastType).name();
				lMessage += ".";
				throw core_BadCastExceptionM(lMessage);
			}
		}
		return static_cast<const typename CastType::Handle&> (inObject);
#else // SCHNAPS_HAVE_DYNAMIC_CAST
		return static_cast<const typename CastType::Handle&>(inObject);
#endif // SCHNAPS_HAVE_DYNAMIC_CAST
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("const CastType::Handle& castHandleT<CastType>(const Object::Handle& inObject)");
}
} // end of core namespace

#endif // core_castObjectT_hpp
