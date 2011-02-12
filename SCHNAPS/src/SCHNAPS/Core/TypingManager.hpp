/*
 * TypingManager.hpp
 *
 *  Created on: 2009-10-08
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

#ifndef core_TypingManager_hpp
#define core_TypingManager_hpp

#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/String.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

namespace core {

/*!
 *  \class TypingManager SCHNAPS/Core/TypingManager.hpp "SCHNAPS/Core/TypingManager.hpp"
 *  \brief Object TypingManager allowing dynamic types allocation.
 *
 *  A TypingManager is used to manage the type hierarchy in order to bypass the dynamic casting
 *  to verify that a type is a specialisation of another.
 */
class TypingManager: public Component {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, StringArray, HashString> TypeMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, StringArray, HashString> TypeMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, StringArray, HashString> TypeMap;
#else // No hash_map found
	typedef std::map<std::string, StringArray> TypeMap;
#endif

public:
	//! TypingManager allocator type.
	typedef AllocatorT<TypingManager, Component::Alloc> Alloc;
	//! TypingManager handle type.
	typedef PointerT<TypingManager, Component::Handle> Handle;
	//! TypingManager bag type.
	typedef ContainerT<TypingManager, Component::Bag> Bag;

	TypingManager();
	TypingManager(const TypingManager& inOriginal);
	virtual ~TypingManager() {}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	TypingManager& operator=(const TypingManager& inOriginal);

	/*!
	 *  \brief Asks for a conversion of a data type into another.
	 *  \param inTypeNameFrom Name of the type to convert.
	 *  \param inTypeNameTo Name of the type to convert into.
	 *  \return True if the conversion can be done, false if not.
	 */
	inline bool typeCast(const std::string& inTypeNameFrom, const std::string& inTypeNameTo) const {
		schnaps_StackTraceBeginM();
			if (inTypeNameFrom == inTypeNameTo) {
				return true;
			}

			TypeMap::const_iterator lIterTypeMap = mTypeMap.find(inTypeNameFrom);
#ifndef SCHNAPS_NDEBUG
			if (lIterTypeMap == mTypeMap.end()) {
				throw schnaps_InternalExceptionM("Unknown type " + inTypeNameFrom + "!");
			}
#else
			schnaps_AssertM(lIterTypeMap != mTypeMap.end());
#endif

			StringArray lParents = lIterTypeMap->second;
			for (unsigned int i = 0; i < lParents.size(); i++) {
				if (typeCast(lParents[i], inTypeNameTo)) {
					return true;
				}
			}

			return false;
		schnaps_StackTraceEndM("bool typeCast(const std::string&, const std::string&) const");
	}

	/*!
	 *  \brief Asks for the nearest common type between two types.
	 *  \param inFirstType One type.
	 *  \param inSecondType Another type.
	 *  \return The nearest common type between the two given types.
	 */
	inline std::string commonType(const std::string& inFirstType, const std::string& inSecondType) const {
		schnaps_StackTraceBeginM();
			if (typeCast(inFirstType, inSecondType)) {
				return inFirstType.c_str();
			} else if (typeCast(inSecondType, inFirstType)) {
				return inSecondType.c_str();
			}

			std::string lCommonType;
			StringArray lSecondTypeParents = mTypeMap.find(inFirstType)->second;
			for (unsigned int i = 0; i < lSecondTypeParents.size(); i++) {
				lCommonType = commonType(inFirstType, lSecondTypeParents[i]);
				if (lCommonType.empty() == false) {
					return lCommonType;
				}
			}

			return "";
		schnaps_StackTraceEndM("std::string commonType(const std::string&, const std::string&) const");
	}

protected:
	TypeMap mTypeMap; //!< Map of stored types to their parents.
};
}

#endif // core_TypingManager_hpp
