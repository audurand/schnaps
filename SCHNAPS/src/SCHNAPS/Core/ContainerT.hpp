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

#ifndef SCHNAPS_Core_ContainerT_hpp
#define SCHNAPS_Core_ContainerT_hpp

#include "SCHNAPS/Core/Allocator.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/Container.hpp"
#include "SCHNAPS/Core/Pointer.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/RunTimeException.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class ContainerT SCHNAPS/Core/ContainerT.hpp "SCHNAPS/Core/ContainerT.hpp"
 *  \brief Templated class defining a T-type container, inheriting from BaseType container.
 *  \param T Type of the values contained.
 *  \param BaseType Base type from which the actual uniform bag type is derived.
 */
template<class T, class BaseType>
class ContainerT: public BaseType {
public:
	//! ContainerT allocator type.
	typedef AllocatorT<ContainerT<T, BaseType> , typename BaseType::Alloc> Alloc;
	//! ContainerT handle type.
	typedef PointerT<ContainerT<T, BaseType> , typename BaseType::Handle> Handle;
	//! ContainerT bag type.
	typedef ContainerT<ContainerT<T, BaseType> , typename BaseType::Bag> Bag;

	explicit ContainerT(unsigned int inN = 0, typename T::Handle inModel = NULL);
	virtual ~ContainerT() {}

	/*!
	 *  \brief  Access to a constant handle to the Nth element of the ContainerT.
	 *  \param  inN Indice of the constant handle (T::Handle) to get.
	 *  \return Constant handle (T::Handle) at the indice N.
	 */
	const typename T::Handle& operator[](unsigned int inN) const {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,BaseType::size()-1);
			return castHandleT<T> (std::vector<Pointer>::operator[](inN));
		schnaps_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::operator[](unsigned int inN) const");
	}

	/*!
	 *  \brief  Acces to an handle the Nth element of the ContainerT.
	 *  \param  inN Indice of the handle (T::Handle) to get.
	 *  \return Pointer (T::Handle) at the indice N.
	 */
	typename T::Handle& operator[](unsigned int inN) {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,BaseType::size()-1);
			return castHandleT<T> (std::vector<Pointer>::operator[](inN));
		schnaps_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::operator[](unsigned int inN)");
	}

	/*!
	 *  \brief  Access to a handle to the Nth element of the ContainerT.
	 *  \param  inN Index of the handle (T::Handle) to get.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	typename T::Handle& at(unsigned int inN) {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,BaseType::size()-1);
			return castHandleT<T> (std::vector<Pointer>::operator[](inN));
		schnaps_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::at(unsigned int inN)");
	}

	/*!
	 *  \brief  Access to a handle to the Nth element of the ContainerT.
	 *  \param  inN Index of the handle (T::Handle) to get.
	 *  \return const Pointer (T::Handle) at the index N.
	 */
	const typename T::Handle& at(unsigned int inN) const {
		schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN,BaseType::size()-1);
			return castHandleT<T> (std::vector<Pointer>::operator[](inN));
		schnaps_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::at(unsigned int inN) const");
	}

	/*!
	 *  \brief  Access to a handle to the last element of the ContainerT.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	typename T::Handle& back() {
		schnaps_StackTraceBeginM();
			if (BaseType::empty()) {
				std::ostringstream lOSS;
				lOSS << "Container named '" << BaseType::getName();
				lOSS << "' is empty; you've called the back() method but there is nothing to return.";
				throw schnaps_RunTimeExceptionM(lOSS.str());
			}
			return castHandleT<T> (std::vector<Pointer>::back());
		schnaps_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::back()");
	}

	/*!
	 *  \brief  Access to a handle to the last element of the ContainerT.
	 *  \return Pointer (T::Handle) at the index N.
	 */
	const typename T::Handle& back() const {
		schnaps_StackTraceBeginM();
			if (BaseType::empty()) {
				std::ostringstream lOSS;
				lOSS << "Container named '" << BaseType::getName();
				lOSS << "' is empty; you've called the back() method but there is nothing to return.";
				throw schnaps_RunTimeExceptionM(lOSS.str());
			}
			return castHandleT<T> (std::vector<Pointer>::back());
		schnaps_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::back() const");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

/*!
 *  \brief Construct a container.
 *  \param inN Initial size of the container.
 *  \param inModel Initial value of the composing elements.
 */
template<class T, class BaseType>
SCHNAPS::Core::ContainerT<T, BaseType>::ContainerT(unsigned int inN, typename T::Handle inModel) :
	BaseType(inN, inModel) {
}

#endif // SCHNAPS_Core_ContainerT_hpp
