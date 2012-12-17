/*
 * InternalException.hpp
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

#ifndef SCHNAPS_Core_InternalException_hpp
#define SCHNAPS_Core_InternalException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 * \def   schnaps_UndefinedMethodInternalExceptionM
 * \brief Construct a InternalException using the message given.
 * \param METHOD Name of the method undefined.
 * \param OBJECT Name of the object where the method is declared but undefined.
 * \param NAME   Name of the instance that generated this exception.
 */
#define schnaps_UndefinedMethodInternalExceptionM(METHOD,OBJECT,NAME) \
  SCHNAPS::Core::InternalException( \
    std::string("Method '").append(METHOD).append("' of class '").append(OBJECT).append( \
      "' is called but is not properly overdefined in subclass with name '").append(NAME).append("'."), \
    __FILE__,__LINE__)

/*!
 * \def   schnaps_InternalExceptionM
 * \brief Construct a InternalException using the message given.
 * \param MESS Exception message.
 */
#define schnaps_InternalExceptionM(MESS) \
  SCHNAPS::Core::InternalException(MESS,__FILE__,__LINE__)

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class InternalException SCHNAPS/Core/InternalException.hpp "SCHNAPS/Core/InternalException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Indication of an internal exception.
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

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_InternalException_hpp
