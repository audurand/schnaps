/*
 * IsLess.hpp
 *
 *  Created on: 2009-10-29
 *   Author: Audrey Durand
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

#ifndef basic_IsLess_hpp
#define basic_IsLess_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IsLess SCHNAPS-plugins/Basic/IsLess.hpp "SCHNAPS-plugins/Basic/IsLess.hpp"
 *  \brief Relational IsLess operator primitive class.
 */
class IsLess: public core::Primitive {
public:
	//! IsIsLess allocator type.
	typedef core::AllocatorT<IsLess, core::Primitive::Alloc> Alloc;
	//! IsIsLess handle type.
	typedef core::PointerT<IsLess, core::Primitive::Handle> Handle;
	//! IsIsLess IsIsLess type.
	typedef core::ContainerT<IsLess, core::Primitive::Bag> Bag;

	IsLess();
	IsLess(const IsLess& inOriginal);
	virtual ~IsLess() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IsLess");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsLess::getName() const");
	}

	IsLess& operator=(const IsLess& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IsLess_hpp */
