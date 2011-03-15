/*
 * ProcessPushed.hpp
 *
 *  Created on: 2009-06-10
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

#ifndef SCHNAPS_Simulation_ProcessPushed_hpp
#define SCHNAPS_Simulation_ProcessPushed_hpp

#include "SCHNAPS/Simulation/Process.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class ProcessPushed SCHNAPS/Simulation/ProcessPushed.hpp "SCHNAPS/Simulation/ProcessPushed.hpp"
 *  \brief Class that describes a pushed process applied to individuals.
 */
class ProcessPushed: public Process {
public:
	//! ProcessPushed allocator type.
	typedef SCHNAPS::Core::AllocatorT<ProcessPushed, Process::Alloc> Alloc;
	//! ProcessPushed handle type.
	typedef SCHNAPS::Core::PointerT<ProcessPushed, Process::Handle> Handle;
	//! ProcessPushed bag type.
	typedef SCHNAPS::Core::ContainerT<ProcessPushed, Process::Bag> Bag;

	ProcessPushed();
	ProcessPushed(const ProcessPushed& inOriginal);
	explicit ProcessPushed(const std::string inLabel);
	virtual ~ProcessPushed() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("ProcessPushed");
			return lName;
		schnaps_StackTraceEndM("const std::string& ProcessPushed::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual SCHNAPS::Core::AnyType::Handle execute(SCHNAPS::Core::ExecutionContext& ioContext) const;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_ProcessPushed_hpp */
