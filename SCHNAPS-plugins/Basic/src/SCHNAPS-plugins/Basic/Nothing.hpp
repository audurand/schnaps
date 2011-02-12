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

#ifndef basic_Nothing_hpp
#define basic_Nothing_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Nothing SCHNAPS-plugins/Basic/Nothing.hpp "SCHNAPS-plugins/Basic/Nothing.hpp"
 *  \brief Empty token terminal primitive class.
 */
class Nothing: public core::Primitive {
public:
	//! Nothing allocator type.
	typedef core::AllocatorT<Nothing, core::Primitive::Alloc> Alloc;
	//! Nothing handle type.
	typedef core::PointerT<Nothing, core::Primitive::Handle> Handle;
	//! Nothing bag type.
	typedef core::ContainerT<Nothing, core::Primitive::Bag> Bag;

	Nothing();
	Nothing(const Nothing& inOriginal);
	virtual ~Nothing() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Nothing");
			return lName;
		schnaps_StackTraceEndM("const std::string& Nothing::getName() const");
	}

	Nothing& operator=(const Nothing& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Nothing_hpp */
