/*
 * Parameters.hpp
 *
 *  Created on: 2010-04-10
 *  Author: Audrey Durand
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
 *  \brief System component to handle simulator parameters.
 */
class Parameters: public Component {
protected:
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

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void readStr(const std::string& inStr) {
		std::stringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters(",", "");

		std::string lOption;
		std::string::size_type lPos;

		while (lTokenizer.getNextToken(lOption)) {
			lPos = lOption.find("=");

			if (lPos == std::string::npos) {
				printf("Expected value of option %s!\n", lOption.c_str());
			} else {
				(*this)[lOption.substr(0, lPos)]->readStr(lOption.substr(lPos+1));
			}
		}
	}

	std::string writeStr() const {
		std::stringstream lSS;
		std::string lParameters;

		for (ParametersMap::const_iterator lIt = mParametersMap.begin(); lIt != mParametersMap.end(); lIt++) {
			lSS << lIt->first.c_str() << "=" << lIt->second->writeStr().c_str() << ",";
		}
		lParameters = lSS.str();
		lParameters.erase(lParameters.end()-1);
		return lParameters;
	}

	void addParameter(std::string inLabel, AnyType::Handle inValue) {
#ifndef SCHNAPS_NDEBUG
		if (mParametersMap.find(inLabel) != mParametersMap.end()) {
			throw schnaps_InternalExceptionM("Parameter " + inLabel + " is already in parameters database!");
		}
#else
		schnaps_AssertM(mParametersMap.find(inLabel) == mParametersMap.end());
#endif
		mParametersMap.insert(std::pair<std::string, AnyType::Handle>(inLabel.c_str(), inValue));
	}

	AnyType::Handle operator[](std::string inLabel) {
#ifndef SCHNAPS_NDEBUG
		if (mParametersMap.find(inLabel) == mParametersMap.end()) {
			throw schnaps_InternalExceptionM("Could not find parameter " + inLabel + " in parameters database!");
		}
#else
		schnaps_AssertM(mParametersMap.find(inLabel) != mParametersMap.end());
#endif
		return mParametersMap.find(inLabel)->second;
	}

	const AnyType::Handle operator[](std::string inLabel) const {
#ifndef SCHNAPS_NDEBUG
		if (mParametersMap.find(inLabel) == mParametersMap.end()) {
			throw schnaps_InternalExceptionM("Could not find parameter " + inLabel + " in parameters database!");
		}
#else
		schnaps_AssertM(mParametersMap.find(inLabel) != mParametersMap.end());
#endif
		return mParametersMap.find(inLabel)->second->clone();
	}

private:
	ParametersMap mParametersMap;
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Parameters_hpp
