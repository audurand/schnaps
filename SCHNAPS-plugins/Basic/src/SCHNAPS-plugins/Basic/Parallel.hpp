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

#ifndef basic_Parallel_hpp
#define basic_Parallel_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Parallel SCHNAPS-plugins/Basic/Parallel.hpp "SCHNAPS-plugins/Basic/Parallel.hpp"
 *  \brief Primitive class that executes sequentially its children but returns a vector containing the result of each one (as if executed in parallel).
 */
class Parallel: public core::Primitive {
public:
	//! Parallel allocator type.
	typedef core::AllocatorT<Parallel, core::Primitive::Alloc> Alloc;
	//! Parallel handle type.
	typedef core::PointerT<Parallel, core::Primitive::Handle> Handle;
	//! Parallel bag type.
	typedef core::ContainerT<Parallel, core::Primitive::Bag> Bag;

	Parallel();
	Parallel(const Parallel& inOriginal);
	virtual ~Parallel() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Parallel");
			return lName;
		schnaps_StackTraceEndM("const std::string& Parallel::getName() const");
	}

	Parallel& operator=(const Parallel& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Parallel_hpp */
