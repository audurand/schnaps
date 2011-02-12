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

#ifndef core_InternalException_hpp
#define core_InternalException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 *  \def   schnaps_UndefinedMethodInternalExceptionM
 *  \brief Construct a InternalException using the message given.
 *  \param METHOD Name of the method undefined.
 *  \param OBJECT Name of the object where the method is declared but undefined.
 *  \param NAME   Name of the instance that generated this exception.
 */
#define schnaps_UndefinedMethodInternalExceptionM(METHOD,OBJECT,NAME) \
  core::InternalException( \
    std::string("Method '").append(METHOD).append("' of class '").append(OBJECT).append( \
      "' is called but is not properly overdefined in subclass with name '").append(NAME).append("'."), \
    __FILE__,__LINE__)

/*!
 *  \def   schnaps_InternalExceptionM
 *  \brief Construct a InternalException using the message given.
 *  \param MESS Exception message.
 */
#define schnaps_InternalExceptionM(MESS) \
  core::InternalException(MESS,__FILE__,__LINE__)

namespace core {

// Forward declarations.
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class InternalException SCHNAPS/Core/InternalException.hpp "SCHNAPS/Core/InternalException.hpp"
 *  \brief Indication of an internal Beagle exception.
 */
class InternalException: public TargetedException {
public:
	//! InternalException allocator type.
	typedef AllocatorT<InternalException, TargetedException::Alloc> Alloc;
	//! InternalException handle type.
	typedef PointerT<InternalException, TargetedException::Handle> Handle;
	//! InternalException bag type.
	typedef ContainerT<InternalException, TargetedException::Bag> Bag;

	explicit InternalException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	virtual ~InternalException() throw () {}

	virtual const char* getExceptionName() const throw ();
};
}

#endif // core_InternalException_hpp
