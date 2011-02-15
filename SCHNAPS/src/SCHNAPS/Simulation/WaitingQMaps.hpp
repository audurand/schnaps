/*
 * WaitingQMaps.hpp
 *
 *  Created on: 2010-07-02
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

#ifndef SCHNAPS_Simulation_WaitingQMaps_hpp
#define SCHNAPS_Simulation_WaitingQMaps_hpp

#include <map>
#include <queue>
#include <vector>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulation/Process.hpp"

namespace SCHNAPS {
namespace Simulation {

class WaitingQMaps: public SCHNAPS::Core::Object {
public:
	//! WaitingQMap allocator type.
	typedef SCHNAPS::Core::AllocatorT<WaitingQMaps, SCHNAPS::Core::Object::Alloc> Alloc;
	//! WaitingQMap handle type.
	typedef SCHNAPS::Core::PointerT<WaitingQMaps, SCHNAPS::Core::Object::Handle> Handle;
	//! WaitingQMap bag type.
	typedef SCHNAPS::Core::ContainerT<WaitingQMaps, SCHNAPS::Core::Object::Bag> Bag;

	WaitingQMaps() {}
	WaitingQMaps(const WaitingQMaps& inOriginal);
	virtual ~WaitingQMaps() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("WaitingQMaps");
			return lName;
		schnaps_StackTraceEndM("const std::string& WaitingQMaps::getName() const");
	}

	std::map<unsigned int, std::queue<Process::Handle> >& getEnvironmentWaitingQMap() {
		return mEnvironment;
	}

	std::vector<std::map<unsigned int, std::queue<Process::Handle> > >& getIndividualsWaitingQMaps() {
		return mIndividuals;
	}

private:
	std::map<unsigned int, std::queue<Process::Handle> > mEnvironment;
	std::vector<std::map<unsigned int, std::queue<Process::Handle> > > mIndividuals;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_WaitingQMaps_hpp */
