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

#ifndef SCHNAPS_Plugins_Operators_Add_hpp
#define SCHNAPS_Plugins_Operators_Add_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Add SCHNAPS/Plugins/Operators/Add.hpp "SCHNAPS/Plugins/Operators/Add.hpp"
 *  \brief Mathematical addition operation primitive class.
 */
class Add: public Core::Primitive {
public:
	//! Add allocator type.
	typedef Core::AllocatorT<Add, Core::Primitive::Alloc> Alloc;
	//! Add handle type.
	typedef Core::PointerT<Add, Core::Primitive::Handle> Handle;
	//! Add bag type.
	typedef Core::ContainerT<Add, Core::Primitive::Bag> Bag;

	Add();
	Add(const Add& inOriginal);
	virtual ~Add() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Add");
			return lName;
		schnaps_StackTraceEndM("const std::string& Add::getName() const");
	}

	Add& operator=(const Add& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Add_hpp */
