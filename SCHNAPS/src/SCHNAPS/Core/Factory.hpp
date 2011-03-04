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

#ifndef SCHNAPS_Core_Factory_hpp
#define SCHNAPS_Core_Factory_hpp

#include "SCHNAPS/Core/Allocator.hpp"
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
 *  \class Factory SCHNAPS/Core/Factory.hpp "SCHNAPS/Core/Factory.hpp"
 *  \brief Object factory allowing dynamic types allocation.
 *
 *  A factory is used to generate and copy data types dynamically. It contains a database of
 *  object allocators associated to a name. String-based lookup are thus possible to obtain
 *  the allocators, to allocate, clone or copy given dynamically-binded data type. For example,
 *  asking for the type given in a string "FitnessSimple" would provide a instanciation
 *  of the allocator for that type exact type, FitnessSimple.
 */
class Factory: public Component {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Allocator::Handle, HashString> AllocatorMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Allocator::Handle, HashString> AllocatorMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Allocator::Handle, HashString> AllocatorMap;
#else // no hash_map found
	typedef std::map<std::string, Allocator::Handle> AllocatorMap;
#endif

public:
	//! Factory allocator type.
	typedef AllocatorT<Factory, Component::Alloc> Alloc;
	//! Factory handle type.
	typedef PointerT<Factory, Component::Handle> Handle;
	//! Factory bag type.
	typedef ContainerT<Factory, Component::Bag> Bag;

	Factory();
	virtual ~Factory() {}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Alias a type name to an already existing one in object factory.
	void aliasAllocator(const std::string& inTypeName, const std::string& inAlias);
	//! Insert new allocator in object factory.
	void insertAllocator(const std::string& inTypeName, Allocator::Handle inAllocator);
	//! Remove allocator from the factory.
	Allocator::Handle removeAllocator(const std::string& inTypeName);

	/*!
	 *  \brief Obtain allocator associated to the given type.
	 *  \param inTypeName Name of the type to obtain the allocator.
	 *  \return Allocator handle to the type allocator, NULL pointer if the type is unknown.
	 */
	inline Allocator::Handle getAllocator(const std::string& inTypeName) const {
		schnaps_StackTraceBeginM();
		Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
		if (lIterAllocMap == mAllocatorMap.end()) {
			return NULL;
		}
		return lIterAllocMap->second;
		schnaps_StackTraceEndM("SCHNAPS::Core::Allocator::Handle SCHNAPS::Core::Factory::getAllocator(const std::string&) const");
	}

protected:
	AllocatorMap mAllocatorMap; //!< Map of stored allocators.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Factory_hpp
