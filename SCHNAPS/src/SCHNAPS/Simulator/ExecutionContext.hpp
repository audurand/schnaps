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

#ifndef simulator_ExecutionContext_hpp
#define simulator_ExecutionContext_hpp

#include "SCHNAPS/Core/ExecutionContext.hpp"
#include "SCHNAPS/Simulator/Clock.hpp"
#include "SCHNAPS/Simulator/Environment.hpp"
#include "SCHNAPS/Simulator/Individual.hpp"

namespace simulator {

/*!
 *  \class ExecutionContext Simulator/include/ExecutionContext.hpp "Simulator/include/ExecutionContext.hpp"
 *  \brief ExecutionContext class, execution context for simulator.
 */
class ExecutionContext: public core::ExecutionContext {

public:
	//! ExecutionContext allocator type.
	typedef core::AllocatorT<ExecutionContext, core::ExecutionContext::Alloc> Alloc;
	//! ExecutionContext handle type.
	typedef core::PointerT<ExecutionContext, core::ExecutionContext::Handle> Handle;
	//! ExecutionContext bag type.
	typedef core::ContainerT<ExecutionContext, core::ExecutionContext::Bag> Bag;

	ExecutionContext();
	ExecutionContext(const simulator::ExecutionContext& inOriginal);
	explicit ExecutionContext(const core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
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
} // end of simulator namespace

#endif /* simulator_ExecutionContext_hpp */
