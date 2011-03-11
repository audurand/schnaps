/*
 * RunTimeException.hpp
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

#ifndef SCHNAPS_Core_RunTimeException_hpp
#define SCHNAPS_Core_RunTimeException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

#include <string>

/*!
 * \def   schnaps_RunTimeExceptionM
 * \brief Construct a RunTimeException using the message given.
 * \param MESS Exception message.
 */
#define schnaps_RunTimeExceptionM(MESS) \
  SCHNAPS::Core::RunTimeException((MESS),__FILE__,__LINE__)

namespace SCHNAPS {
namespace Core {
	
// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class RunTimeException SCHNAPS/Core/RunTimeException.hpp "SCHNAPS/Core/RunTimeException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Run time checking exception.
 */
class RunTimeException: public TargetedException {
public:
	//! RunTimeException allocator type.
	typedef AllocatorT<RunTimeException, TargetedException::Alloc> Alloc;
	//! RunTimeException handle type.
	typedef PointerT<RunTimeException, TargetedException::Handle> Handle;
	//! RunTimeException bag type.
	typedef ContainerT<RunTimeException, TargetedException::Bag> Bag;

	explicit RunTimeException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	virtual ~RunTimeException() throw () {}

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_RunTimeException_hpp
