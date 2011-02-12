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

#ifndef simulator_SimulationVariables_hpp
#define simulator_SimulationVariables_hpp

#include "SCHNAPS/Simulator/Demography.hpp"

namespace simulator {

/*!
 *  \class SimulationVariables Simulator/include/SimulationVariables.hpp "Simulator/include/SimulationVariables.hpp"
 *  \brief SimulationVariables class.
 */
	class SimulationVariables: public Demography
{
public:
	//! SimulationVariables allocator type.
	typedef core::AllocatorT<SimulationVariables, core::Object::Alloc> Alloc;
	//! SimulationVariables handle type.
	typedef core::PointerT<SimulationVariables, core::Object::Handle> Handle;
	//! SimulationVariables bag type.
	typedef core::ContainerT<SimulationVariables, core::Object::Bag> Bag;

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
} // end of simulator namespace

#endif /* simulator_SimulationVariables_hpp */
