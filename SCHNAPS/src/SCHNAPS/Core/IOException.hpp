/*
 * IOException.hpp
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

#ifndef SCHNAPS_Core_IOException_hpp
#define SCHNAPS_Core_IOException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

#include <string>

/*!
 * \def   schnaps_IOExceptionMessageM
 * \brief Construct a IOException using the message given.
 * \param MESS Exception message.
 */
#define schnaps_IOExceptionMessageM(MESS) \
  SCHNAPS::Core::IOException((MESS),__FILE__,__LINE__)

/*!
 *  \def   schnaps_IOExceptionNodeM
 *  \brief Construct a IOException using the message given.
 *  \param XMLNODE XML node for which the exception occurred.
 *  \param MESS Exception message.
 */
#define schnaps_IOExceptionNodeM(XMLNODE,MESS) \
  SCHNAPS::Core::IOException((XMLNODE),(MESS),__FILE__,__LINE__)

namespace SCHNAPS {
namespace Core {

// forward declaration
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class IOException SCHNAPS/Core/IOException.hpp "SCHNAPS/Core/IOException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Exception indicating that a parameter, or such, is invalid.
 */
class IOException: public TargetedException {
public:
	//! IOException allocator type.
	typedef AllocatorT<IOException, TargetedException::Alloc> Alloc;
	//! IOException handle type.
	typedef PointerT<IOException, TargetedException::Handle> Handle;
	//! IOException bag type.
	typedef ContainerT<IOException, TargetedException::Bag> Bag;

	explicit IOException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	explicit IOException(const PACC::XML::Node& inIter, std::string inMessage = "", std::string inFileName = "",
			unsigned int inLineNumber = 0);
	virtual ~IOException() throw () {}

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_IOException_hpp
