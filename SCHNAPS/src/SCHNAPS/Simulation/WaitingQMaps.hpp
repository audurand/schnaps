/*
 * WaitingQMaps.hpp
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

#ifndef SCHNAPS_Simulation_WaitingQMaps_hpp
#define SCHNAPS_Simulation_WaitingQMaps_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulation/Process.hpp"

#include <map>
#include <queue>
#include <vector>

namespace SCHNAPS {
namespace Simulation {

class WaitingQMaps: public Core::Object {
public:
	//! WaitingQMap allocator type.
	typedef Core::AllocatorT<WaitingQMaps, Core::Object::Alloc> Alloc;
	//! WaitingQMap handle type.
	typedef Core::PointerT<WaitingQMaps, Core::Object::Handle> Handle;
	//! WaitingQMap bag type.
	typedef Core::ContainerT<WaitingQMaps, Core::Object::Bag> Bag;

	WaitingQMaps() {}
	WaitingQMaps(const WaitingQMaps& inOriginal);
	virtual ~WaitingQMaps() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("WaitingQMaps");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::WaitingQMaps::getName() const");
	}

	/*!
	 * \brief  Return a const reference to the environment FIFOs.
	 * \return A const reference to the environment FIFOs.
	 */
	std::map<unsigned int, std::queue<Process::Handle> >& getEnvironmentWaitingQMap() {
		return mEnvironment;
	}

	/*!
	 * \brief  Return a const reference to all individuals FIFOs.
	 * \return A const reference to all individuals FIFOs FIFOs.
	 */
	std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > >& getIndividualsWaitingQMaps() {
		return mIndividuals;
	}

private:
	std::map<unsigned int, std::queue<Process::Handle> > mEnvironment;							//!< FIFOs for environment.
	std::map<unsigned int, std::map<unsigned int, std::queue<Process::Handle> > > mIndividuals;	//!< FIFOs for each individual.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_WaitingQMaps_hpp */
