/*
 * Demography.hpp
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

#ifndef SCHNAPS_Simulation_Demography_hpp
#define SCHNAPS_Simulation_Demography_hpp

#include "SCHNAPS/Core/HashString.hpp"
#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

#include <vector>

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Demography SCHNAPS/Simulation/Demography.hpp "SCHNAPS/Simulation/Demography.hpp"
 *  \brief Demography variables.
 */
class Demography: public Core::Object {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Core::PrimitiveTree::Handle, HashString> VariablesMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Core::PrimitiveTree::Handle, HashString> VariablesMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Core::PrimitiveTree::Handle, HashString> VariablesMap;
#else // no hash_map found
	typedef std::map<std::string, Core::PrimitiveTree::Handle> VariablesMap;
#endif

public:
	//! Demography allocator type.
	typedef Core::AllocatorT<Demography, Core::Object::Alloc> Alloc;
	//! Demography handle type.
	typedef Core::PointerT<Demography, Core::Object::Handle> Handle;
	//! Demography bag type.
	typedef Core::ContainerT<Demography, Core::Object::Bag> Bag;

	Demography() {}
	Demography(const Demography& inOriginal);
	virtual ~Demography() {}
	
	//! Return a handle to a deep copy of the object.
	virtual Core::Object::Handle deepCopy(const Core::System& inSystem) const;

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Demography");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Demography::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	
	//! Return the list of variables contained in demography.
	std::vector<std::string> getVariableList() const;
	//! Return a const reference to a variable initialization tree function.
	const Core::PrimitiveTree& getVariableInitTree(const std::string& inLabel) const;
	//! Check if a variable is contained in demography.
	bool hasVariable(const std::string& inLabel) const;

protected:
VariablesMap mVariablesMap; //!< The map of all demography variable labels to initialization tree.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Demography_hpp */
