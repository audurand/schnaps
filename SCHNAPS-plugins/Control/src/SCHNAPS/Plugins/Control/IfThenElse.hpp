/*
 * IfThenElse.hpp
 *
 *  Created on: 2009-03-10
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

#ifndef SCHNAPS_Plugins_Control_IfThenElse_hpp
#define SCHNAPS_Plugins_Control_IfThenElse_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class IfThenElse SCHNAPS/Basic/IfThenElse.hpp "SCHNAPS/Basic/IfThenElse.hpp"
 *  \brief Branching IfThenElse primitive class.
 */
class IfThenElse: public SCHNAPS::Core::Primitive {
public:
	//! IfThenElse allocator type.
	typedef SCHNAPS::Core::AllocatorT<IfThenElse, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! IfThenElse handle type.
	typedef SCHNAPS::Core::PointerT<IfThenElse, SCHNAPS::Core::Primitive::Handle> Handle;
	//! IfThenElse bag type.
	typedef SCHNAPS::Core::ContainerT<IfThenElse, SCHNAPS::Core::Primitive::Bag> Bag;

	IfThenElse();
	IfThenElse(const IfThenElse& inOriginal);
	virtual ~IfThenElse() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_IfThenElse");
			return lName;
		schnaps_StackTraceEndM("const std::string& IfThenElse::getName() const");
	}

	IfThenElse& operator=(const IfThenElse& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_IfThenElse_hpp */
