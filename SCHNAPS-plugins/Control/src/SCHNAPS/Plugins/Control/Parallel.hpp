/*
 * Parallel.hpp
 *
 *  Created on: 2010-08-27
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

#ifndef SCHNAPS_Plugins_Control_Parallel_hpp
#define SCHNAPS_Plugins_Control_Parallel_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Parallel SCHNAPS/Basic/Parallel.hpp "SCHNAPS/Basic/Parallel.hpp"
 *  \brief Primitive class that executes sequentially its children but returns a vector containing the result of each one (as if executed in parallel).
 */
class Parallel: public SCHNAPS::Core::Primitive {
public:
	//! Parallel allocator type.
	typedef SCHNAPS::Core::AllocatorT<Parallel, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! Parallel handle type.
	typedef SCHNAPS::Core::PointerT<Parallel, SCHNAPS::Core::Primitive::Handle> Handle;
	//! Parallel bag type.
	typedef SCHNAPS::Core::ContainerT<Parallel, SCHNAPS::Core::Primitive::Bag> Bag;

	Parallel();
	Parallel(const Parallel& inOriginal);
	virtual ~Parallel() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_Parallel");
			return lName;
		schnaps_StackTraceEndM("const std::string& Parallel::getName() const");
	}

	Parallel& operator=(const Parallel& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Parallel_hpp */
