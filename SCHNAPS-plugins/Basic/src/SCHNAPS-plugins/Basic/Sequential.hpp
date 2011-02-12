/*
 * Sequential.hpp
 *
 *  Created on: 2009-11-20
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

#ifndef basic_Sequential_hpp
#define basic_Sequential_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Sequential SCHNAPS-plugins/Basic/Sequential.hpp "SCHNAPS-plugins/Basic/Sequential.hpp"
 *  \brief Primitive class that executes sequentially its children and returns the result of the last one.
 */
class Sequential: public core::Primitive {
public:
	//! Sequential allocator type.
	typedef core::AllocatorT<Sequential, core::Primitive::Alloc> Alloc;
	//! Sequential handle type.
	typedef core::PointerT<Sequential, core::Primitive::Handle> Handle;
	//! Sequential bag type.
	typedef core::ContainerT<Sequential, core::Primitive::Bag> Bag;

	Sequential();
	Sequential(const Sequential& inOriginal);
	virtual ~Sequential() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Sequential");
			return lName;
		schnaps_StackTraceEndM("const std::string& Sequential::getName() const");
	}

	Sequential& operator=(const Sequential& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Sequential_hpp */
