/*
 * Process.hpp
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

#ifndef SCHNAPS_Simulation_Process_hpp
#define SCHNAPS_Simulation_Process_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Process SCHNAPS/Simulation/Process.hpp "SCHNAPS/Simulation/Process.hpp"
 *  \brief Process part of a simulation.
 */
class Process: public Core::Object {
protected:
	typedef std::pair<std::string, Core::AnyType::Handle> LocalVariable;

public:
	//! Process target.
	enum Target {
		eCurrent, eEnvironment, eIndividuals,
	};

	//! Process allocator type.
	typedef Core::AllocatorT<Process, Core::Object::Alloc> Alloc;
	//! Process handle type.
	typedef Core::PointerT<Process, Core::Object::Handle> Handle;
	//! Process bag type.
	typedef Core::ContainerT<Process, Core::Object::Bag> Bag;

	Process();
	Process(const Process& inOriginal);
	Process(const std::string& inLabel, Core::PrimitiveTree::Handle inPrimitiveTree);
	virtual ~Process() {}

	//! Return a handle to a deep copy of the object.
	virtual Core::Object::Handle deepCopy(const Core::System& inSystem) const;

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Process");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Process::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Return a handle to the result of process execution.
	virtual Core::AnyType::Handle execute(Core::ExecutionContext& ioContext) const;

	//! Return a const reference to the type of process execution result.
	const std::string& getReturnType(Core::ExecutionContext& ioContext) const;

	//! Validate the process.
	void validate(Core::ExecutionContext& ioContext) const;

	//! Return a const reference to the process label.
	const std::string& getLabel() const {
		return mLabel;
	}
	
private:
	//! Read local variables from XML using system.
	void readLocalVariables(PACC::XML::ConstIterator inIter, Core::System& ioSystem);

protected:
	std::string mLabel;							//!< Label of process.

private:
	Core::PrimitiveTree::Handle mPrimitiveTree;	//!< Primive tree that represents the process execution.
	std::vector<LocalVariable> mLocalVariables;	//!< Variables local to the process.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Process_hpp */
