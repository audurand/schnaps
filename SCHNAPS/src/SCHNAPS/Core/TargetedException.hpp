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

#ifndef SCHNAPS_Core_TargetedException_hpp
#define SCHNAPS_Core_TargetedException_hpp

#include <string>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"

namespace SCHNAPS {
	
namespace Core {

// forward declarations
template<class T, class BaseType> class AbstractAllocT;
template<class T, class BaseType> class PointerT;
template<class T, class BaseType> class ContainerT;

/*!
 *  \class TargetedException SCHNAPS/Core/TargetedException.hpp "SCHNAPS/Core/TargetedException.hpp"
 *  \brief Targeted SCHNAPS exception that indicate a precise line number in a source file.
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

	//! Getting the actual name of a SCHNAPS exception.
	virtual const char* getExceptionName() const throw () =0;

	virtual void explain(std::ostream& ioES = std::cerr) throw ();

	/*!
	 *  \brief  Getting the file name where the exception is detected (throwed).
	 *  \return File name where the exception is detected (throwed).
	 */
	inline const std::string& getFileName() const throw () {
		return mFileName;
	}

	/*!
	 *  \brief  Getting the line number in the file where the exception is detected (throwed).
	 *  \return Line number in the file where the exception is detected (throwed).
	 */
	inline unsigned int getLineNumber() const throw () {
		return mLineNumber;
	}

	/*!
	 *  \brief Set the file name where the exception is detected (throwed).
	 *  \param inFileName File name where the exception is detected.
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
	std::string mFileName; //!< Name of the file where the error is targeted.
	unsigned int mLineNumber; //!< Line number in the file where the error is targeted.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_ObjectException_hpp
