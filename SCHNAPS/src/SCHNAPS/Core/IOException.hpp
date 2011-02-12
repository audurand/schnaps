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

#ifndef core_IOException_hpp
#define core_IOException_hpp

#include <string>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 *  \def   core_IOExceptionMessageM
 *  \brief Construct a IOException using the message given.
 *  \param MESS Exception message.
 */
#define schnaps_IOExceptionMessageM(MESS) \
  core::IOException((MESS),__FILE__,__LINE__)

/*!
 *  \def   schnaps_IOExceptionNodeM
 *  \brief Construct a IOException using the message given.
 *  \param XMLNODE XML node for which the exception occurred.
 *  \param MESS Exception message.
 */
#define schnaps_IOExceptionNodeM(XMLNODE,MESS) \
  core::IOException((XMLNODE),(MESS),__FILE__,__LINE__)

namespace core {

// Forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class IOException SCHNAPS/Core/IOException.hpp "SCHNAPS/Core/IOException.hpp"
 *  \brief SCHNAPS exception indicating that a parameter, or such, is invalid.
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

	virtual const char* getExceptionName() const throw ();
};
}

#endif // core_IOException_hpp
