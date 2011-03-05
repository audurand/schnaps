/*
 * Process.hpp
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

#ifndef SCHNAPS_Simulation_Process_hpp
#define SCHNAPS_Simulation_Process_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Process SCHNAPS/Simulation/Process.hpp "SCHNAPS/Simulation/Process.hpp"
 *  \brief Class that describes a process applied to individuals.
 */
class Process: public SCHNAPS::Core::Object {
public:
	//! Specify a process target.
	enum Target {
		eCurrent, eEnvironment, eIndividuals,
	};

	//! Process allocator type.
	typedef SCHNAPS::Core::AllocatorT<Process, SCHNAPS::Core::Object::Alloc> Alloc;
	//! Process handle type.
	typedef SCHNAPS::Core::PointerT<Process, SCHNAPS::Core::Object::Handle> Handle;
	//! Process bag type.
	typedef SCHNAPS::Core::ContainerT<Process, SCHNAPS::Core::Object::Bag> Bag;

	Process();
	Process(const Process& inOriginal);
	explicit Process(const std::string inLabel, SCHNAPS::Core::PrimitiveTree::Handle inPrimitiveTree);
	virtual ~Process() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Process");
		return lName;
		schnaps_StackTraceEndM("const std::string& Process::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual SCHNAPS::Core::AnyType::Handle execute(SCHNAPS::Core::ExecutionContext& ioContext) const;

	const std::string& getReturnType(SCHNAPS::Core::ExecutionContext& ioContext) const;

	void validate(SCHNAPS::Core::ExecutionContext& ioContext) const;

	Process::Handle deepCopy(const SCHNAPS::Core::System& ioSystem) const;

	const std::string& getLabel() const {
		return mLabel;
	}

protected:
	std::string mLabel;

private:
	SCHNAPS::Core::PrimitiveTree::Handle mPrimitiveTree;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Process_hpp */
