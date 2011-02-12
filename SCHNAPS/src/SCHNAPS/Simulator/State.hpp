/*
 * State.hpp
 *
 *  Created on: 2009-03-12
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

#ifndef simulator_State_hpp
#define simulator_State_hpp

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

namespace simulator {

/*!
 *  \class State Simulator/include/State.hpp "Simulator/include/State.hpp"
 *  \brief State class.
 */
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	class State: public core::Object, public std::hash_map<std::string, core::Atom::Handle, core::HashString>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	class State: public core::Object, public __gnu_cxx::hash_map<std::string, core::Atom::Handle, core::HashString>
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	class State: public core::Object, public stdext::hash_map<std::string, core::Atom::Handle, core::HashString>
#else // No hash_map found
	class State: public core::Object, public std::map<std::string, core::Atom::Handle>
#endif
{
public:
	//! State allocator type.
	typedef core::AllocatorT<State, core::Object::Alloc> Alloc;
	//! State handle type.
	typedef core::PointerT<State, core::Object::Handle> Handle;
	//! State bag type.
	typedef core::ContainerT<State, core::Object::Bag> Bag;

	State() {}
	State(const State& inOriginal);
	virtual ~State() {}

	State& operator=(const State& inOriginal);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("State");
			return lName;
		schnaps_StackTraceEndM("const std::string& State::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void print(std::ostream& ioStream, const std::vector<std::string> inVariables) const;

	const core::Atom& getVariable(std::string inLabel) {
		State::iterator lIt =  find(inLabel);
		if (lIt == end()) {
			throw core_Exception_hpp("Cannot get the variable " + inLabel + ": it does not exists in the current individual state!");
		}
		return *lIt->second;
	}

	void setVariable(std::string inLabel, core::Atom::Handle inValue) {
		State::iterator lIt = find(inLabel);
		if (lIt == end()) {
			throw core_Exception_hpp("Cannot set the variable " + inLabel + ": it does not exists in the current individual state!");
		}
		lIt->second = inValue;
	}
};
} // end of simulator namespace

#endif /* simulator_State_hpp */
