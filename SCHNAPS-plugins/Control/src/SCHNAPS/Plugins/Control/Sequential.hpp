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

#ifndef SCHNAPS_Plugins_Control_Sequential_hpp
#define SCHNAPS_Plugins_Control_Sequential_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Sequential SCHNAPS/Basic/Sequential.hpp "SCHNAPS/Basic/Sequential.hpp"
 *  \brief Primitive class that executes sequentially its children and returns the result of the last one.
 */
class Sequential: public SCHNAPS::Core::Primitive {
public:
	//! Sequential allocator type.
	typedef SCHNAPS::Core::AllocatorT<Sequential, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! Sequential handle type.
	typedef SCHNAPS::Core::PointerT<Sequential, SCHNAPS::Core::Primitive::Handle> Handle;
	//! Sequential bag type.
	typedef SCHNAPS::Core::ContainerT<Sequential, SCHNAPS::Core::Primitive::Bag> Bag;

	Sequential();
	Sequential(const Sequential& inOriginal);
	virtual ~Sequential() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_Sequential");
			return lName;
		schnaps_StackTraceEndM("const std::string& Sequential::getName() const");
	}

	Sequential& operator=(const Sequential& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Sequential_hpp */
