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

#ifndef simulator_Clock_hpp
#define simulator_Clock_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Bool.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace simulator {

/*!
 *  \class Clock Simulator/include/Clock.hpp "Simulator/include/Clock.hpp"
 *  \brief Switch primitive class that refers to a variable (to make the switch).
 */
class Clock: public core::Object {
public:
	//! Clock allocator type.
	typedef core::AllocatorT<Clock, core::Object::Alloc> Alloc;
	//! Clock handle type.
	typedef core::PointerT<Clock, core::Object::Handle> Handle;
	//! Clock bag type.
	typedef core::ContainerT<Clock, core::Object::Bag> Bag;

	Clock();
	Clock(const Clock& inOriginal);
	virtual ~Clock() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Clock");
			return lName;
		schnaps_StackTraceEndM("const std::string& Clock::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void reset() {
		schnaps_StackTraceBeginM();
			mValue = 0;
		schnaps_StackTraceEndM("void simulator::Clock::reset()");
	}

	bool step(core::ExecutionContext& ioContext) {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mStop);
			mValue++;
			return !core::castHandleT<core::Bool>(mStop->interpret(ioContext))->getValue();
		schnaps_StackTraceEndM("bool simulator::Clock::step(core::ExecutionContext& ioContext)");
	}

	const unsigned long getValue() const {
		return mValue;
	}

	const core::PrimitiveTree::Handle getStop() const {
		return mStop;
	}

private:
	unsigned long mValue; 				//!< The clock value.
	core::PrimitiveTree::Handle mStop;	//!< Stop condition tree.
};
} // end of simulator namespace

#endif /* simulator_Clock_hpp */
