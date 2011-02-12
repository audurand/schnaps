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

#ifndef simulator_ProcessPushed_hpp
#define simulator_ProcessPushed_hpp

#include "SCHNAPS/Simulator/Process.hpp"

namespace simulator {

/*!
 *  \class ProcessPushed Simulator/include/ProcessPushed.hpp "Simulator/include/ProcessPushed.hpp"
 *  \brief Class that describes a pushed process applied to individuals.
 */
class ProcessPushed: public Process {
public:
	//! ProcessPushed allocator type.
	typedef core::AllocatorT<ProcessPushed, Process::Alloc> Alloc;
	//! ProcessPushed handle type.
	typedef core::PointerT<ProcessPushed, Process::Handle> Handle;
	//! ProcessPushed bag type.
	typedef core::ContainerT<ProcessPushed, Process::Bag> Bag;

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

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(core::ExecutionContext& ioContext) const;
};
} // end of simulator namespace

#endif /* simulator_ProcessPushed_hpp */
