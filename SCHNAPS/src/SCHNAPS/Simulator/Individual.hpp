/*
 * Individual.hpp
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

#ifndef simulator_Individual_hpp
#define simulator_Individual_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Simulator/State.hpp"

namespace simulator {

/*!
 *  \class Individual Simulator/Simulation/include/Individual.hpp "Simulator/Simulation/include/Individual.hpp"
 *  \brief Individual class.
 */
class Individual: public core::Object {
public:
	//! Individual allocator type.
	typedef core::AllocatorT<Individual, core::Object::Alloc> Alloc;
	//! Individual handle type.
	typedef core::PointerT<Individual, core::Object::Handle> Handle;
	//! Individual bag type.
	typedef core::ContainerT<Individual, core::Object::Bag> Bag;

	explicit Individual(std::string inID = "");
	Individual(const Individual& inOriginal);
	virtual ~Individual() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Individual");
			return lName;
		schnaps_StackTraceEndM("const std::string& Individual::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void print(std::ostream& ioStream, const std::vector<std::string> inVariables) const;

	const std::string& getID() const {
		return mID;
	}

	const std::string getPrefix() const {
		std::stringstream lID(mID);
		PACC::Tokenizer lTokenizer(lID);
		lTokenizer.setDelimiters("/", "");
		return lTokenizer.getNextToken();
	}

	const unsigned int getIndex() const {
		std::stringstream lID(mID);
		PACC::Tokenizer lTokenizer(lID);
		lTokenizer.setDelimiters("/", "");
		lTokenizer.getNextToken();
		return core::str2uint(lTokenizer.getNextToken());
	}

	const State& getState() const {
		return mState;
	}

	State& getState() {
		return mState;
	}

	void setID(std::string& inID) {
		mID = inID;
	}

	void setState(State& inState) {
		mState = inState;
	}

protected:
	std::string mID;	//!< Individual ID tag;
	State mState;		//!< The state that describes the individual.
};
} // end of simulator namespace

#endif /* simulator_Individual_hpp */
