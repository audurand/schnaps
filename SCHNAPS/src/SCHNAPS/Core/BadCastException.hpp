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

#ifndef SCHNAPS_Core_BadCastException_hpp
#define SCHNAPS_Core_BadCastException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

#ifdef SCHNAPS_HAVE_RTTI
#include <typeinfo>
#endif // SCHNAPS_HAVE_RTTI

/*!
 *  \def   SCHNAPS_Core_BadCastExceptionM
 *  \brief Construct a BadCastException.
 *  \param MESS Message explaining the bad cast problem.
 */
#define SCHNAPS_Core_BadCastExceptionM(MESS) \
  SCHNAPS::Core::BadCastException(MESS,__FILE__,__LINE__)

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class BadCastException SCHNAPS/Core/BadCastException.hpp "SCHNAPS/Core/BadCastException.hpp"
 *  \brief SCHNAPS exception indicating that a bad cast (by a call to castObjectT) happen.
 */
class BadCastException: public TargetedException {
public:

	//! BadCastException allocator type.
	typedef AllocatorT<BadCastException, TargetedException::Alloc> Alloc;
	//! BadCastException handle type.
	typedef PointerT<BadCastException, TargetedException::Handle> Handle;
	//! BadCastException bag type.
	typedef ContainerT<BadCastException, TargetedException::Bag> Bag;

	explicit BadCastException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	virtual ~BadCastException() throw () {}

	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_BadCastException_hpp
