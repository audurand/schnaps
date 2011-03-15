/*
 * Environment.hpp
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

#ifndef SCHNAPS_Simulation_Environment_hpp
#define SCHNAPS_Simulation_Environment_hpp

#include "SCHNAPS/Simulation/Individual.hpp"
#include "SCHNAPS/Simulation/Population.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Environment SCHNAPS/Simulation/Environment.hpp "SCHNAPS/Simulation/Environment.hpp"
 *  \brief Environment where all individuals evolve.
 */
class Environment: public Individual {
public:
	//! Environment allocator type.
	typedef Core::AllocatorT<Environment, Individual::Alloc> Alloc;
	//! Environment handle type.
	typedef Core::PointerT<Environment, Individual::Handle> Handle;
	//! Environment bag type.
	typedef Core::ContainerT<Environment, Individual::Bag> Bag;

	Environment();
	Environment(const Environment& inOriginal);
	virtual ~Environment() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Environment");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Environment::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	/*!
	 * \brief Reset the environment to its initial state and clear the population.
	 */
	void reset() {
		mState = mInitState;
		mPopulation.clear();
	}

	/*!
	 * \brief  Return a const reference to the population.
	 * \return A const reference to the population.
	 */
	const Population& getPopulation() const {
		return mPopulation;
	}

	/*!
	 * \brief  Return a reference to the population.
	 * \return A reference to the population.
	 */
	Population& getPopulation() {
		return mPopulation;
	}

private:
	State mInitState;		//!< Initial state of the environment
	Population mPopulation; //!< The population
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Environment_hpp */
