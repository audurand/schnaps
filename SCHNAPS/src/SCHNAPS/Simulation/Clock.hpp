/*
 * Clock.hpp
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

#ifndef SCHNAPS_Simulation_Clock_hpp
#define SCHNAPS_Simulation_Clock_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Bool.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Clock SCHNAPS/Simulation/Clock.hpp "SCHNAPS/Simulation/Clock.hpp"
 *  \brief The clock that drives the simulation.
 */
class Clock: public Core::Object {
public:
	//! Clock allocator type.
	typedef Core::AllocatorT<Clock, Core::Object::Alloc> Alloc;
	//! Clock handle type.
	typedef Core::PointerT<Clock, Core::Object::Handle> Handle;
	//! Clock bag type.
	typedef Core::ContainerT<Clock, Core::Object::Bag> Bag;

	Clock();
	Clock(const Clock& inOriginal);
	virtual ~Clock() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Clock");
		return lName;
		schnaps_StackTraceEndM("const std::string& Clock::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	/*!
	 * \brief Reset the value of clock to zero.
	 */
	void reset() {
		schnaps_StackTraceBeginM();
		mValue = 0;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::Clock::reset()");
	}

	/*!
	 * \brief  Have the clock stepping of one time unit.
	 * \param  ioContext A reference to the execution context.
	 * \return True if the simulation continues, false if it the stop condition was encountered.
	 */
	bool step(Core::ExecutionContext& ioContext) {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mStop);
		mValue++;
		return !Core::castHandleT<Core::Bool>(mStop->interpret(ioContext))->getValue();
		schnaps_StackTraceEndM("bool SCHNAPS::Simulation::Clock::step(SCHNAPS::Core::ExecutionContext&)");
	}

	/*!
	 * \brief  Return a const reference to the current value.
	 * \return A const reference to the current value.
	 */
	const unsigned long& getValue() const {
		return mValue;
	}

	/*!
	 * \brief  Return a const handle to the stop condition tree.
	 * \return A const handle to the stop condition tree.
	 */
	const Core::PrimitiveTree::Handle getStop() const {
		return mStop;
	}

private:
	unsigned long mValue; 				//!< The clock value.
	Core::PrimitiveTree::Handle mStop;	//!< A handle to the stop condition tree.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Clock_hpp */
