/*
 * Parameters.hpp
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

#ifndef SCHNAPS_Core_Parameters_hpp
#define SCHNAPS_Core_Parameters_hpp

#include "SCHNAPS/Core/AnyType.hpp"
#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

namespace SCHNAPS {
namespace Core {

/*!
 *  \class Parameters SCHNAPS/Core/Parameters.hpp "SCHNAPS/Core/Parameters.hpp"
 *  \brief Parameters class to handle simulator parameters.
 */
class Parameters: public Component {
private:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, AnyType::Handle, HashString> ParametersMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, AnyType::Handle, HashString> ParametersMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, AnyType::Handle, HashString> ParametersMap;
#else // no hash_map found
	typedef std::map<std::string, AnyType::Handle> ParametersMap;
#endif

public:
	//! Parameters allocator type.
	typedef AllocatorT<Parameters, Component::Alloc> Alloc;
	//! Parameters handle type.
	typedef PointerT<Parameters, Component::Handle> Handle;
	//! Parameters bag type.
	typedef ContainerT<Parameters, Component::Bag> Bag;

	Parameters();
	virtual ~Parameters() {}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Read data from string.
	void readStr(const std::string& inStr);
	//! Write data to string.
	std::string writeStr() const;

	//! Insert a new parameter with specific value.
	void insertParameter(const std::string& inLabel, AnyType::Handle inValue);
	
	//! Set parameter value.
	void setParameter(const std::string& inLabel, const AnyType::Handle inValue);
	
	/*!
	 * \brief  Return a const reference to the parameter with specific label.
	 * \param  inLabel A const reference to the label of the parameter.
	 * \return A const reference to the parameter.
	 * \throw  SCHNAPS::Core::RunTimeException if the parameter does not exist.
	 * \throw  SCHNAPS::Core::AssertException if the parameter is NULL.
	 */
	const AnyType& getParameter(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		ParametersMap::const_iterator lIterParameters = mParametersMap.find(inLabel);
		if(lIterParameters == mParametersMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The parameter '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		schnaps_NonNullPointerAssertM(lIterParameters->second);
		return *lIterParameters->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::AnyType& SCHNAPS::Core::Parameters::getParameter(const std::string&) const");
	}
	
	/*!
	 * \brief  Return a const handle to the parameter with specific label.
	 * \param  inLabel A const reference to the label of the parameter.
	 * \return A const handle to the parameter.
	 * \throw  SCHNAPS::Core::RunTimeException if the parameter does not exist.
	 * \throw  SCHNAPS::Core::AssertException if the parameter is NULL.
	 */
	const AnyType::Handle getParameterHandle(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		ParametersMap::const_iterator lIterParameters = mParametersMap.find(inLabel);
		if(lIterParameters == mParametersMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The parameter '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		schnaps_NonNullPointerAssertM(lIterParameters->second);
		return lIterParameters->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Parameters::getParameter(const std::string&) const");
	}

private:
	ParametersMap mParametersMap;	//!< The map of parameter labels to values.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Parameters_hpp
