/*
 * ExecutionContext.hpp
 *
 *  Created on: 2010-06-26
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

#ifndef SCHNAPS_Simulation_ExecutionContext_hpp
#define SCHNAPS_Simulation_ExecutionContext_hpp

#include "SCHNAPS/Core/ExecutionContext.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/Individual.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class ExecutionContext SCHNAPS/Simulation/ExecutionContext.hpp "SCHNAPS/Simulation/ExecutionContext.hpp"
 *  \brief ExecutionContext class, execution context for simulator.
 */
class ExecutionContext: public SCHNAPS::Core::ExecutionContext {
public:
	//! ExecutionContext allocator type.
	typedef SCHNAPS::Core::AllocatorT<ExecutionContext, SCHNAPS::Core::ExecutionContext::Alloc> Alloc;
	//! ExecutionContext handle type.
	typedef SCHNAPS::Core::PointerT<ExecutionContext, SCHNAPS::Core::ExecutionContext::Handle> Handle;
	//! ExecutionContext bag type.
	typedef SCHNAPS::Core::ContainerT<ExecutionContext, SCHNAPS::Core::ExecutionContext::Bag> Bag;

	ExecutionContext();
	ExecutionContext(const SCHNAPS::Simulation::ExecutionContext& inOriginal);
	explicit ExecutionContext(const SCHNAPS::Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
	virtual ~ExecutionContext() {}

	virtual const std::string& getName() const {
		return mName;
	}

	void setClock(Clock::Handle inClock) {
		mClock = inClock;
	}

	void setEnvironment(Environment::Handle inEnvironment) {
		mEnvironment = inEnvironment;
	}

	void setIndividualByIndex(unsigned int inIndex) {
		mIndividual = mEnvironment->getPopulation()[inIndex];
	}

	void setIndividual(Individual::Handle inIndividual) {
		mIndividual = inIndividual;
	}

	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

	const Environment& getEnvironment() const {
		schnaps_NonNullPointerAssertM(mEnvironment);
		return *mEnvironment;
	}

	const Environment::Handle getEnvironmentHandle() const {
		return mEnvironment;
	}

	const Individual& getIndividual() const {
		schnaps_NonNullPointerAssertM(mIndividual);
		return *mIndividual;
	}

	Individual& getIndividual() {
		schnaps_NonNullPointerAssertM(mIndividual);
		return *mIndividual;
	}

	const Individual::Handle getIndividualHandle() const {
		return mIndividual;
	}

protected:
	std::string mName;

	// Reference structures
	Clock::Handle mClock;					//!< Handle to the clock used for simulation.
	Environment::Handle mEnvironment;		//!< Handle to the environment.

	// Current structures
	Individual::Handle mIndividual;			//!< Handle to the current individual (or environment) processing.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_ExecutionContext_hpp */
