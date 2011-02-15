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

/*!
 *  \brief SCHNAPS custom exceptions, part of the Object Oriented Foundations.
 *
 *  SCHNAPS integrates the C++ exceptions mechanism. Keeping it
 *  object oriented (OO), an hierarchy of exceptions is defined. At
 *  the top is the abstract exception superclass, Exception,
 *  which is inherited by every other SCHNAPS exception
 *  class. This top class inherits is derived from the standard
 *  exception class (\c std::exception), which allows SCHNAPS
 *  exceptions to be caught in a simple \c catch(std::exception&)
 *  expression.
 *
 *  A Java-like trace mechanism exists for exceptions that are derived
 *  from Exception.  When such an exception is caught, the
 *  Exception::explain() method displays the call-stack at the
 *  point the exception was thrown.  In order to acheive this it is
 *  necessary to call to macros from every function or method to be
 *  included in the trace.  The first macro \c
 *  schnaps_StackTraceBeginM() should appear at the top of your
 *  function or method.  The second macro \c schnaps_StackTraceEndM()
 *  should appear at the end or your function or method with the
 *  function or method's specification.  Here is an example: \code
 *
 *  void myFunction() {
 *    schnaps_StackTraceBeginM();
 *    std::cout << "This is my function." << std::endl;
 *    schnaps_StackTraceEndM("void myFunction()");
 *  } \endcode
 *
 *  If your function or method has an all-inclusive try...catch block,
 *  the macros should appear within that block.
 *
 */

#ifndef SCHNAPS_Core_Exception_hpp
#define SCHNAPS_Core_Exception_hpp

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "SCHNAPS/Core/Object.hpp"

/*!
 *  \def   schnaps_StackTraceBeginM
 *  \brief Used to declare the start of a section of code that will
 *    appear in an exception's stack trace
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_StackTraceBeginM() \
  try {
#else // SCHNAPS_NDEBUG
#define schnaps_StackTraceBeginM()
#endif // SCHNAPS_NDEBUG
/*!
 *  \def   schnaps_StackTraceEndM
 *  \brief Used to declare the end of a section of code that will
 *    appear in an exception's stack trace
 *  \param NAME Function/method name that is traced.
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_StackTraceEndM(NAME) \
  } \
  catch (SCHNAPS::Core::Exception& inException) { \
    inException.pushStackTrace(NAME,__FILE__,__LINE__); \
    throw; \
  }
#else // SCHNAPS_NDEBUG
#define schnaps_StackTraceEndM(NAME)
#endif // SCHNAPS_NDEBUG

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AbstractAllocT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class Exception SCHNAPS/Core/Exception.hpp "SCHNAPS/Core/Exception.hpp"
 *  \brief Basic SCHNAPS exception class.
 */
class Exception: public Object, public std::exception {
public:
	//! Exception allocator type.
	typedef AbstractAllocT<Exception, Object::Alloc> Alloc;
	//! Exception handle type.
	typedef PointerT<Exception, Object::Handle> Handle;
	//! Exception bag type.
	typedef ContainerT<Exception, Object::Bag> Bag;

	explicit Exception(std::string inMessage = "");
	virtual ~Exception() throw () {}

	virtual void explain(std::ostream& ioES = std::cerr) throw ();
	void terminate(std::ostream& ioES = std::cerr) throw ();
	virtual const char* what() const throw ();

	/*!
	 *  \brief  Getting the actual name of a Beagle exception.
	 *  \return Exception name.
	 */
	virtual const char* getExceptionName() const throw () =0;

	/*!
	 *  \brief  Return error message of the actual exception.
	 *  \return Error message.
	 */
	inline std::string getMessage() const throw () {
		return mMessage;
	}

	/*!
	 *  \brief  Add an item to the exception's stack trace.
	 *  \param inName Name of the method/function traced.
	 *  \param inFilename Source filename where stack tracing is done.
	 *  \param inLineNumber Line number in source where stack tracing is done.
	 */
	inline void pushStackTrace(const char* inName, const char* inFilename, unsigned int inLineNumber) {
		mStackTrace.push_back(StackTraceItem(inName, inFilename, inLineNumber));
	}

	/*!
	 *  \brief Set error message of actual exception.
	 *  \param inMessage Value of error message.
	 */
	inline void setMessage(std::string inMessage) {
		mMessage = inMessage;
	}

protected:
	/*!
	 *  \brief Internal struct to store stack trace elements.
	 */
	struct StackTraceItem {
		std::string mName; //!< Name of the method/function traced.
		std::string mFilename; //!< Source filename where stack tracing is done.
		unsigned int mLineNumber; //!< Line number in source where stack tracing is done.

		/*!
		 *  \brief Construct a stack trace item.
		 *  \param inName Name of the method/function traced.
		 *  \param inFilename Source filename where stack tracing is done.
		 *  \param inLineNumber Line number in source where stack tracing is done.
		 */
		explicit StackTraceItem(const char* inName = "", const char* inFilename = "", unsigned int inLineNumber = 0) :
			mName(inName), mFilename(inFilename), mLineNumber(inLineNumber) {
		}
	};

	std::vector<StackTraceItem> mStackTrace; //!< Stack trace of the calls
	std::string mMessage; //!< Exception message describing the abnormal situation.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Exception_hpp
