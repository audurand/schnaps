/*
 * State.hpp
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

#ifndef SCHNAPS_Simulation_State_hpp
#define SCHNAPS_Simulation_State_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#include <map>
#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#endif // defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#ifdef SCHNAPS_HAVE_GNUCXX_HASHMAP
#include <ext/hash_map>
#endif // SCHNAPS_HAVE_GNUCXX_HASHMAP

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class State SCHNAPS/Simulation/State.hpp "SCHNAPS/Simulation/State.hpp"
 *  \brief Individual state made of variables.
 */
class State: public Core::Object {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Core::Atom::Handle, Core::HashString> VariablesMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Core::Atom::Handle, Core::HashString> VariablesMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Core::Atom::Handle, Core::HashString> VariablesMap;
#else // No hash_map found
	typedef std::map<std::string, Core::Atom::Handle> VariablesMap;
#endif

public:
	//! State allocator type.
	typedef Core::AllocatorT<State, Core::Object::Alloc> Alloc;
	//! State handle type.
	typedef Core::PointerT<State, Core::Object::Handle> Handle;
	//! State bag type.
	typedef Core::ContainerT<State, Core::Object::Bag> Bag;

	State() {}
	State(const State& inOriginal);
	virtual ~State() {}

	//! Copy operator.
	State& operator=(const State& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("State");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::State::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Print state to file stream.
	void print(std::ostream& ioStream, const std::vector<std::string>& inVariables) const;
	
	//! Erase all variables.
	void clear();

	//! Insert a variable.
	void insertVariable(const std::string& inLabel, Core::Atom::Handle inValue);
	//! Remove a variable.
	void removeVariable(const std::string& inLabel);
	//! Check if the state contains a variable.
	bool hasVariable(const std::string& inLabel) const;

	/*!
	 * \brief  Return a const reference to the value of a variable.
	 * \param  inLabel A const reference to the label of the variable.
	 * \return A const reference to the value of the variable.
	 * \throw  SCHNAPS::Core::RunTimeException if the variable does not exist.
	 * \throw  SCHNAPS::Core::AssertException if the variable is NULL.
	 */
	const Core::Atom& getVariable(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		VariablesMap::const_iterator lIterVariables = mVariablesMap.find(inLabel);
		if (lIterVariables == mVariablesMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The variable '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		schnaps_NonNullPointerAssertM(lIterVariables->second);
		return *lIterVariables->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::Atom& SCHNAPS::Simulation::State::getVariable(const std::string&) const");
	}

	/*!
	 * \brief  Return a const reference to the value of a variable.
	 * \param  inLabel A const reference to the label of the variable.
	 * \return A const reference to the value of the variable.
	 * \throw  SCHNAPS::Core::RunTimeException if the variable does not exist.
	 */
	const Core::Atom::Handle getVariableHandle(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		VariablesMap::const_iterator lIterVariables = mVariablesMap.find(inLabel);
		if (lIterVariables == mVariablesMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The variable '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		return lIterVariables->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::Atom::Handle SCHNAPS::Simulation::State::getVariableHandle(const std::string&) const");
	}

	/*!
	 * \brief Set a variable to a specific value.
	 * \param inLabel A const reference to the label of the variable.
	 * \param inValue A handle to the new value of the variable.
	 * \throw SCHNAPS::Core::RunTimeException if the variable does not exist.
	 */
	void setVariable(const std::string& inLabel, Core::Atom::Handle inValue) {
		schnaps_StackTraceBeginM();
		VariablesMap::iterator lIterVariables = mVariablesMap.find(inLabel);
		if (lIterVariables == mVariablesMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The variable '" << inLabel << "' does not exist; ";
			lOSS << "could not set it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		lIterVariables->second = inValue;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::State::setVariable(const std::string&, SCHNAPS::Core::Atom::Handle) const");
	}

private:
	VariablesMap mVariablesMap; //!< The map of variable labels to values.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_State_hpp */
