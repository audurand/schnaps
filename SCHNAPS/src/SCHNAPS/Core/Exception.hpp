/*
 * Exception.hpp
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

/*!
 * \brief SCHNAPS custom exceptions, part of the Object Oriented Foundations.
 *
 * SCHNAPS integrates the C++ exceptions mechanism. Keeping it
 * object oriented (OO), an hierarchy of exceptions is defined. At
 * the top is the abstract exception superclass, Exception,
 * which is inherited by every other SCHNAPS exception
 * class. This top class inherits is derived from the standard
 * exception class (\c std::exception), which allows SCHNAPS
 * exceptions to be caught in a simple \c catch(std::exception&)
 * expression.
 *
 * A Java-like trace mechanism exists for exceptions that are derived
 * from Exception.  When such an exception is caught, the
 * Exception::explain() method displays the call-stack at the
 * point the exception was thrown.  In order to acheive this it is
 * necessary to call to macros from every function or method to be
 * included in the trace.  The first macro \c
 * schnaps_StackTraceBeginM() should appear at the top of your
 * function or method.  The second macro \c schnaps_StackTraceEndM()
 * should appear at the end or your function or method with the
 * function or method's specification.  Here is an example: \code
 *
 * void myFunction() {
 *   schnaps_StackTraceBeginM();
 *   std::cout << "This is my function." << std::endl;
 *   schnaps_StackTraceEndM("void myFunction()");
 * } \endcode
 *
 * If your function or method has an all-inclusive try...catch block,
 * the macros should appear within that block.
 */

#ifndef SCHNAPS_Core_Exception_hpp
#define SCHNAPS_Core_Exception_hpp

#include "SCHNAPS/Core/Object.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

/*!
 * \def   schnaps_StackTraceBeginM
 * \brief Used to declare the start of a section of code that will
 *   appear in an exception's stack trace
 */
#ifndef SCHNAPS_NDEBUG
#define schnaps_StackTraceBeginM() \
  try {
#else // SCHNAPS_NDEBUG
#define schnaps_StackTraceBeginM()
#endif // SCHNAPS_NDEBUG

/*!
 * \def   schnaps_StackTraceEndM
 * \brief Used to declare the end of a section of code that will
 *   appear in an exception's stack trace
 * \param NAME Function/method name that is traced.
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
 * \class Exception SCHNAPS/Core/Exception.hpp "SCHNAPS/Core/Exception.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Basic SCHNAPS exception class.
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

	//! Explain the exception throwed in the stream given.
	virtual void explain(std::ostream& ioES = std::cerr) throw ();
	//! Terminate execution of actual program by displaying exception message and aborting.
	void terminate(std::ostream& ioES = std::cerr) throw ();
	//! Return a copy of the error message.
	virtual const char* what() const throw ();

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw () =0;

	/*!
	 *  \brief  Return error message of the actual exception.
	 *  \return Error message.
	 */
	inline std::string getMessage() const throw () {
		return mMessage;
	}

	/*!
	 *  \brief Add an item to the exception's stack trace.
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
		std::string mName;			//!< Name of the method/function traced.
		std::string mFilename;		//!< Source filename where stack tracing is done.
		unsigned int mLineNumber;	//!< Line number in source where stack tracing is done.

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

	std::vector<StackTraceItem> mStackTrace;	//!< Stack trace of the calls
	std::string mMessage;						//!< Exception message describing the abnormal situation.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Exception_hpp
