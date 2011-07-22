/*
 * Individual.hpp
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

#ifndef SCHNAPS_Simulation_Individual_hpp
#define SCHNAPS_Simulation_Individual_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Simulation/State.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Individual Simulator/Simulation/include/Individual.hpp "Simulator/Simulation/include/Individual.hpp"
 *  \brief Population individual.
 */
class Individual: public Core::Object {
public:
	//! Individual allocator type.
	typedef Core::AllocatorT<Individual, Core::Object::Alloc> Alloc;
	//! Individual handle type.
	typedef Core::PointerT<Individual, Core::Object::Handle> Handle;
	//! Individual bag type.
	typedef Core::ContainerT<Individual, Core::Object::Bag> Bag;

	explicit Individual(std::string inID = "");
	Individual(const Individual& inOriginal);
	virtual ~Individual() {}
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Individual");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Individual::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Print individual to file stream.
	void print(std::ostream& ioStream, const std::vector<std::string> inVariables) const;

	/*!
	 * \brief  Return a const reference to the ID.
	 * \return A const reference to the ID.
	 */
	const std::string& getID() const {
		return mID;
	}

	/*!
	 * \brief  Return the ID prefix.
	 * \return The ID prefix.
	 */
	const std::string getPrefix() const {
		std::stringstream lID(mID);
		PACC::Tokenizer lTokenizer(lID);
		lTokenizer.setDelimiters("/", "");
		return lTokenizer.getNextToken();
	}

	/*!
	 * \brief  Return the ID index.
	 * \return The ID index.
	 */
	const unsigned int getIndex() const {
		std::stringstream lID(mID);
		PACC::Tokenizer lTokenizer(lID);
		lTokenizer.setDelimiters("/", "");
		lTokenizer.getNextToken();
		return SCHNAPS::str2uint(lTokenizer.getNextToken());
	}

	/*!
	 * \brief  Return a const reference to the state.
	 * \return A const reference to the state.
	 */
	const State& getState() const {
		return mState;
	}

	/*!
	 * \brief  Return a reference to the state.
	 * \return A reference to the state.
	 */
	State& getState() {
		return mState;
	}

	/*!
	 * \brief  Return a const reference to the activity status.
	 * \return A const reference to the activity status.
	 */
	const bool& isActive() const {
		return mActive;
	}

	/*!
	 * \brief Set the ID to a specific value.
	 * \param inID A reference to the new ID.
	 */
	void setID(std::string& inID) {
		mID = inID;
	}

	/*!
	 * \brief Set the state to a specific value.
	 * \param inState A reference to the new state.
	 */
	void setState(State& inState) {
		mState = inState;
	}

	/*!
	 * \brief Set the activity status to idle.
	 */
	void setIdle() {
		mActive = false;
	}

protected:
	std::string mID;	//!< Individual ID tag;
	State mState;		//!< The state that describes the individual.
	bool mActive;		//!< Indicates if the individual is active or idle, thus considered by the simulation or not.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Individual_hpp */
