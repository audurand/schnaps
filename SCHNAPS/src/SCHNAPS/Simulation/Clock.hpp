/*
 * Clock.hpp
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

#ifndef SCHNAPS_Simulation_Clock_hpp
#define SCHNAPS_Simulation_Clock_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Bool.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Clock SCHNAPS/Simulation/Clock.hpp "SCHNAPS/Simulation/Clock.hpp"
 *  \brief Switch primitive class that refers to a variable (to make the switch).
 */
class Clock: public SCHNAPS::Core::Object {
public:
	//! Clock allocator type.
	typedef SCHNAPS::Core::AllocatorT<Clock, SCHNAPS::Core::Object::Alloc> Alloc;
	//! Clock handle type.
	typedef SCHNAPS::Core::PointerT<Clock, SCHNAPS::Core::Object::Handle> Handle;
	//! Clock bag type.
	typedef SCHNAPS::Core::ContainerT<Clock, SCHNAPS::Core::Object::Bag> Bag;

	Clock();
	Clock(const Clock& inOriginal);
	virtual ~Clock() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Clock");
			return lName;
		schnaps_StackTraceEndM("const std::string& Clock::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void reset() {
		schnaps_StackTraceBeginM();
			mValue = 0;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::Clock::reset()");
	}

	bool step(SCHNAPS::Core::ExecutionContext& ioContext) {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mStop);
			mValue++;
			return !SCHNAPS::Core::castHandleT<SCHNAPS::Core::Bool>(mStop->interpret(ioContext))->getValue();
		schnaps_StackTraceEndM("bool SCHNAPS::Simulation::Clock::step(SCHNAPS::Core::ExecutionContext& ioContext)");
	}

	const unsigned long getValue() const {
		return mValue;
	}

	const SCHNAPS::Core::PrimitiveTree::Handle getStop() const {
		return mStop;
	}

private:
	unsigned long mValue; 				//!< The clock value.
	SCHNAPS::Core::PrimitiveTree::Handle mStop;	//!< Stop condition tree.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Clock_hpp */
