/*
 * ObjectException.hpp
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

#ifndef SCHNAPS_Core_ObjectException_hpp
#define SCHNAPS_Core_ObjectException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 *  \def   schnaps_ObjectExceptionM
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
 * \class ObjectException SCHNAPS/Core/ObjectException.hpp "SCHNAPS/Core/ObjectException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Exception relative to a erroneous state of an Object.
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

	//! Explain the exception throwed in the stream given.
	virtual void explain(std::ostream& ioES = std::cerr) throw ();
	
	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
	
	//! Set values of the ObjectException using given object reference.
	void setObjectState(const Object& inObject);

	/*!
	 * \brief Return the name of the object related to the exception.
	 * \return A const reference to the name of the object related to the exception.
	 */
	inline const std::string& getObjectName() const throw () {
		return mObjectName;
	}

	/*!
	 * \brief Return the value of the reference counter of the object related to the exception.
	 * \return Reference counter value of the object related to the exception.
	 */
	inline unsigned int getObjectRefCounter() const throw () {
		return mObjectRefCounter;
	}

	/*!
	 * \brief Return the RTTI type name of the object related to the exception.
	 * \return RTTI type name of the object related to the exception.
	 * \warning The RTTI name of the Object is determined only when the preprocessing flag
	 *   SCHNAPS_HAVE_RTTI is defined.
	 */
	inline const std::string& getObjectTypeName() const throw () {
		return mObjectTypeName;
	}

protected:
	ObjectException() {
	} //!< Prohibited constructor!

private:
	std::string mObjectName;		//!< Name of the erroneous Object.
	std::string mObjectTypeName;	//!< RTTI name of the erroneous Object.
	unsigned int mObjectRefCounter;	//!< Value of the reference counter of the Object.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ObjectException_hpp
