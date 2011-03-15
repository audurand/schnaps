/*
 * AssertException.hpp
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

#ifndef SCHNAPS_Core_AssertException_hpp
#define SCHNAPS_Core_AssertException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/TargetedException.hpp"

/*!
 * \def   schnaps_AssertM
 * \brief Assert that the condition is true. If not, throw a SCHNAPS::Core::AssertException.
 * \param COND Condition to test, to assert.
 * \throw AssertException The condition is false.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_AssertM(COND) \
  if ( !(COND) ) throw SCHNAPS::Core::AssertException("Assertion failed.",__FILE__,__LINE__)
#else // SCHNAPS_NDEBUG
#define schnaps_AssertM(COND)
#endif // SCHNAPS_NDEBUG

/*!
 * \def   schnaps_NonNullPointerAssertM
 * \brief Assert that the pointer is non null. If not, throw a SCHNAPS::Core::AssertException.
 * \param PTR Pointer to test.
 * \throw AssertException If the pointer is NULL.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_NonNullPointerAssertM(PTR) \
  if ( !(PTR) ) throw SCHNAPS::Core::AssertException("Pointer asserted is NULL.",__FILE__,__LINE__)
#else  // SCHNAPS_NDEBUG
#define schnaps_NonNullPointerAssertM(PTR)
#endif // SCHNAPS_NDEBUG

/*!
 * \def   schnaps_BoundCheckAssertM
 * \brief Assert the bounds of an index.
 * \param INDEX Index value to test.
 * \param LOWBOUND Lower bound.
 * \param UPBOUND Upper bound.
 * \throw AssertException If the value is out of bound.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_BoundCheckAssertM(INDEX,LOWBOUND,UPBOUND) \
  if ( (INDEX) < (LOWBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is less than the lower bound accepted (" << LOWBOUND << ")."; \
    throw SCHNAPS::Core::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  } \
  if ( (INDEX) > (UPBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is more than the upper bound accepted (" << UPBOUND << ")."; \
    throw SCHNAPS::Core::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  }
#else  // SCHNAPS_NDEBUG
#define schnaps_BoundCheckAssertM(INDEX,LOWBOUND,UPBOUND)
#endif // SCHNAPS_NDEBUG

/*!
 * \def   schnaps_UpperBoundCheckAssertM
 * \brief Assert the upper bound of an index value.
 * \param INDEX Index value to test.
 * \param UPBOUND Upper bound.
 * \throw AssertException If the value is out of bound.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_UpperBoundCheckAssertM(INDEX,UPBOUND) \
  if ( (INDEX) > (UPBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is more than the upper bound accepted (" << UPBOUND << ")."; \
    throw SCHNAPS::Core::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  }
#else  // SCHNAPS_NDEBUG
#define schnaps_UpperBoundCheckAssertM(INDEX,UPBOUND)
#endif // SCHNAPS_NDEBUG

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AllocatorT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class AssertException SCHNAPS/Core/AssertException.hpp "SCHNAPS/Core/AssertException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Exception indicating an badly asserted condition.
 */
class AssertException: public TargetedException {
public:
	//! AssertException allocator type.
	typedef AllocatorT<AssertException, TargetedException::Alloc> Alloc;
	//! AssertException handle type.
	typedef PointerT<AssertException, TargetedException::Handle> Handle;
	//! AssertException bag type.
	typedef ContainerT<AssertException, TargetedException::Bag> Bag;

	explicit AssertException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	virtual ~AssertException() throw () {}

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw ();
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_AssertException_hpp
