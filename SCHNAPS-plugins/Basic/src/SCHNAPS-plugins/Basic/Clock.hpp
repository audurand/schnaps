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

#ifndef basic_Clock_hpp
#define basic_Clock_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Clock SCHNAPS-plugins/Basic/Clock.hpp "SCHNAPS-plugins/Basic/Clock.hpp"
 *  \brief Token terminal primitive class that refers to a variable.
 */
class Clock: public core::Primitive {
public:
	//! Clock allocator type.
	typedef core::AllocatorT<Clock, core::Primitive::Alloc> Alloc;
	//! Clock handle type.
	typedef core::PointerT<Clock, core::Primitive::Handle> Handle;
	//! Clock bag type.
	typedef core::ContainerT<Clock, core::Primitive::Bag> Bag;

	Clock();
	virtual ~Clock() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Clock");
			return lName;
		schnaps_StackTraceEndM("const std::string& Clock::getName() const");
	}

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Clock_hpp */
