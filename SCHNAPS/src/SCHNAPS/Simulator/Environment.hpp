/*
 * Environment.hpp
 *
 *  Created on: 2009-03-17
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

#ifndef simulator_Environment_hpp
#define simulator_Environment_hpp

#include "SCHNAPS/Simulator/Individual.hpp"
#include "SCHNAPS/Simulator/Population.hpp"

namespace simulator {
/*!
 *  \class Environment Simulator/include/Environment.hpp "Simulator/include/Environment.hpp"
 *  \brief Environment class.
 */
class Environment: public Individual {
public:
	//! Environment allocator type.
	typedef core::AllocatorT<Environment, Individual::Alloc> Alloc;
	//! Environment handle type.
	typedef core::PointerT<Environment, Individual::Handle> Handle;
	//! Environment bag type.
	typedef core::ContainerT<Environment, Individual::Bag> Bag;

	Environment();
	Environment(const Environment& inOriginal);
	virtual ~Environment() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Environment");
			return lName;
		schnaps_StackTraceEndM("const std::string& simulator::Environment::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void reset() {
		mState = mInitState;
		mPopulation.clear();
	}

	const Population& getPopulation() const {
		return mPopulation;
	}

	Population& getPopulation() {
		return mPopulation;
	}

private:
	State mInitState;
	Population mPopulation; //!< The population
};
} // end of simulator namespace

#endif /* simulator_Environment_hpp */
