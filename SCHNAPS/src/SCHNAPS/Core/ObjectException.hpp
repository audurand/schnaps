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

#ifndef SCHNAPS_Core_ObjectException_hpp
#define SCHNAPS_Core_ObjectException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 *  \def   simulator_ObjectExceptionM
 *  \brief Construct a ObjectException using the message given.
 *  \param MESS Exception message.
 */
#define schnaps_ObjectExceptionM(MESS) \
  SCHNAPS::Core::ObjectException((*this),MESS,__FILE__,__LINE__)

namespace SCHNAPS {

namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class ObjectException SCHNAPS/Core/ObjectException.hpp "SCHNAPS/Core/ObjectException.hpp"
 *  \brief SCHNAPS exception relative to a erroneous state of an Object.
 */
class ObjectException: public TargetedException {

public:
	//! ObjectException allocator type.
	typedef AllocatorT<ObjectException, TargetedException::Alloc> Alloc;
	//! ObjectException handle type.
	typedef PointerT<ObjectException, TargetedException::Handle> Handle;
	//! ObjectException bag type.
	typedef ContainerT<ObjectException, TargetedException::Bag> Bag;

	explicit ObjectException(const Object& inObject, std::string inMessage = "", std::string inFileName = "",
			unsigned int inLineNumber = 0);
	virtual ~ObjectException() throw () {}

	virtual void explain(std::ostream& ioES = std::cerr) throw ();
	virtual const char* getExceptionName() const throw ();
	void setObjectState(const Object& inObject);

	/*!
	 *  \brief Getting the name of the object related to the exception.
	 *  \return Name of the object related to the exception.
	 */
	inline const std::string& getObjectName() const throw () {
		return mObjectName;
	}

	/*!
	 *  \brief Getting the value of the reference counter of the object related to the exception.
	 *  \return Reference counter value of the object related to the exception.
	 */
	inline unsigned int getObjectRefCounter() const throw () {
		return mObjectRefCounter;
	}

	/*!
	 *  \brief Getting the RTTI type name of the object related to the exception.
	 *  \return RTTI type name of the object related to the exception.
	 *  \warning The RTTI name of the Object is determined only when the preprocessing flag
	 *    SCHNAPS_HAVE_RTTI is defined.
	 */
	inline const std::string& getObjectTypeName() const throw () {
		return mObjectTypeName;
	}

protected:
	ObjectException() {
	} //!< Prohibited constructor!

private:

	std::string mObjectName; //!< Name of the erroneous Object.
	std::string mObjectTypeName; //!< RTTI name of the erroneous Object.
	unsigned int mObjectRefCounter; //!< Value of the reference counter of the Object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ObjectException_hpp
