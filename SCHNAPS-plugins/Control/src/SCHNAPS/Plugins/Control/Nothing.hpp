/*
 * Nothing.hpp
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

#ifndef SCHNAPS_Plugins_Control_Nothing_hpp
#define SCHNAPS_Plugins_Control_Nothing_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Nothing SCHNAPS/Basic/Nothing.hpp "SCHNAPS/Basic/Nothing.hpp"
 *  \brief Empty token terminal primitive class.
 */
class Nothing: public SCHNAPS::Core::Primitive {
public:
	//! Nothing allocator type.
	typedef SCHNAPS::Core::AllocatorT<Nothing, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! Nothing handle type.
	typedef SCHNAPS::Core::PointerT<Nothing, SCHNAPS::Core::Primitive::Handle> Handle;
	//! Nothing bag type.
	typedef SCHNAPS::Core::ContainerT<Nothing, SCHNAPS::Core::Primitive::Bag> Bag;

	Nothing();
	Nothing(const Nothing& inOriginal);
	virtual ~Nothing() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_Nothing");
			return lName;
		schnaps_StackTraceEndM("const std::string& Nothing::getName() const");
	}

	Nothing& operator=(const Nothing& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Nothing_hpp */
