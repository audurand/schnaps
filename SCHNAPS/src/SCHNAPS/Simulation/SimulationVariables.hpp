/*
 * SimulationVariables.hpp
 *
 *  Created on: 2010-04-14
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

#ifndef SCHNAPS_Simulation_SimulationVariables_hpp
#define SCHNAPS_Simulation_SimulationVariables_hpp

#include "SCHNAPS/Simulation/Demography.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class SimulationVariables SCHNAPS/Simulation/SimulationVariables.hpp "SCHNAPS/Simulation/SimulationVariables.hpp"
 *  \brief SimulationVariables class.
 */
	class SimulationVariables: public Demography
{
public:
	//! SimulationVariables allocator type.
	typedef SCHNAPS::Core::AllocatorT<SimulationVariables, SCHNAPS::Core::Object::Alloc> Alloc;
	//! SimulationVariables handle type.
	typedef SCHNAPS::Core::PointerT<SimulationVariables, SCHNAPS::Core::Object::Handle> Handle;
	//! SimulationVariables bag type.
	typedef SCHNAPS::Core::ContainerT<SimulationVariables, SCHNAPS::Core::Object::Bag> Bag;

	SimulationVariables() {}
	SimulationVariables(const SimulationVariables& inOriginal);
	virtual ~SimulationVariables() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("SimulationVariables");
			return lName;
		schnaps_StackTraceEndM("const std::string& SimulationVariables::getName() const");
	}
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_SimulationVariables_hpp */
