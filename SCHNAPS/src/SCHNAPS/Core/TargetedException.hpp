/*
 * TargetedException.hpp
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

#ifndef SCHNAPS_Core_TargetedException_hpp
#define SCHNAPS_Core_TargetedException_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"

#include <string>

namespace SCHNAPS {
namespace Core {

// forward declarations
template<class T, class BaseType> class AbstractAllocT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 * \class TargetedException SCHNAPS/Core/TargetedException.hpp "SCHNAPS/Core/TargetedException.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Targeted exception that indicate a precise line number in a source file.
 */
class TargetedException: public Exception {
public:
	//! TargetedException allocator type.
	typedef AbstractAllocT<TargetedException, Exception::Alloc> Alloc;
	//! TargetedException handle type.
	typedef PointerT<TargetedException, Exception::Handle> Handle;
	//! TargetedException bag type.
	typedef ContainerT<TargetedException, Exception::Bag> Bag;

	explicit TargetedException(std::string inMessage = "", std::string inFileName = "", unsigned int inLineNumber = 0);
	virtual ~TargetedException() throw () {}

	//! Explain the exception throwed in the stream given.
	virtual void explain(std::ostream& ioES = std::cerr) throw ();

	//! Return a const pointer to the actual name (char) of the exception.
	virtual const char* getExceptionName() const throw () =0;

	/*!
	 * \brief  Return the file name where the exception is detected (throwed).
	 * \return File name where the exception is detected (throwed).
	 */
	inline const std::string& getFileName() const throw () {
		return mFileName;
	}

	/*!
	 * \brief  Return the line number in the file where the exception is detected (throwed).
	 * \return Line number in the file where the exception is detected (throwed).
	 */
	inline unsigned int getLineNumber() const throw () {
		return mLineNumber;
	}

	/*!
	 *  \brief Set the file name where the exception is detected (throwed).
	 *  \param inFileName A const reference to the File name where the exception is detected.
	 */
	inline void setFileName(const std::string& inFileName) {
		schnaps_StackTraceBeginM();
		mFileName = inFileName;
		schnaps_StackTraceEndM("void TargetedException::setFileName(const std::string& inFileName)");
	}

	/*!
	 *  \brief Set the line number in the file where the exception is detected (throwed).
	 *  \param inLineNumber Line number in the file where the exception is detected.
	 */
	inline void setLineNumber(unsigned int inLineNumber) {
		schnaps_StackTraceBeginM();
		mLineNumber = inLineNumber;
		schnaps_StackTraceEndM("void TargetedException::setLineNumber(unsigned int inLineNumber)");
	}

private:
	std::string mFileName;		//!< Name of the file where the error is targeted.
	unsigned int mLineNumber;	//!< Line number in the file where the error is targeted.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ObjectException_hpp
