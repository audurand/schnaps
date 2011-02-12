/*
 * Add.hpp
 *
 *  Created on: 2009-02-26
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

#ifndef basic_Add_hpp
#define basic_Add_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {

/*!
 *  \class Add SCHNAPS-plugins/Basic/Add.hpp "SCHNAPS-plugins/Basic/Add.hpp"
 *  \brief Mathematical addition operation primitive class.
 */
class Add: public core::Primitive {
public:
	//! Add allocator type.
	typedef core::AllocatorT<Add, core::Primitive::Alloc> Alloc;
	//! Add handle type.
	typedef core::PointerT<Add, core::Primitive::Handle> Handle;
	//! Add bag type.
	typedef core::ContainerT<Add, core::Primitive::Bag> Bag;

	Add();
	Add(const Add& inOriginal);
	virtual ~Add() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Add");
			return lName;
		schnaps_StackTraceEndM("const std::string& Add::getName() const");
	}

	Add& operator=(const Add& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Add_hpp */
