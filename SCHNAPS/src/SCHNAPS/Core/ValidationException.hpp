/*
 * ValidationException.hpp
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

#ifndef SCHNAPS_Core_ValidationException_hpp
#define SCHNAPS_Core_ValidationException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"

#include <string>

/*!
 * \def   schnaps_ValidateParameterM
 * \brief Assert that a parameter is valid, following a given condition.
 * \param COND Condition to test.
 * \param PARAM String containing the name of the parameter.
 * \param MESS String explaining the condition.
 * \throw ValidationException When the condition is false.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_ValidateParameterM(COND,PARAM,MESS) \
  if( !(COND) ) throw SCHNAPS::Core::ValidationException( \
    std::string("The parameter ").append(PARAM).append(" is invalid (").append(MESS).append(")."))
#else  // SCHNAPS_NDEBUG
#define schnaps_ValidateParameterM(COND,PARAM,MESS)
#endif // SCHNAPS_NDEBUG

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class ValidationException SCHNAPS/Core/ValidationException.hpp "SCHNAPS/Core/ValidationException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Exception indicating that a parameter, or such, is invalid.
 */
class ValidationException: public Exception {
public:
	//! ValidationException allocator type.
	typedef AllocatorT<ValidationException, Exception::Alloc> Alloc;
	//! ValidationException handle type.
	typedef PointerT<ValidationException, Exception::Handle> Handle;
	//! ValidationException bag type.
	typedef ContainerT<ValidationException, Exception::Bag> Bag;

	explicit ValidationException(std::string inMessage = "");
	virtual ~ValidationException() throw () {}

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ValidationException_hpp
