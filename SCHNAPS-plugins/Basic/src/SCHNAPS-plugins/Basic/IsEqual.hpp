/*
 * Equal.hpp
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

#ifndef basic_IsEqual_hpp
#define basic_IsEqual_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IsEqual SCHNAPS-plugins/Basic/IsEqual.hpp "SCHNAPS-plugins/Basic/IsEqual.hpp"
 *  \brief Relational IsEqual operator primitive class.
 */
class IsEqual: public core::Primitive {
public:
	//! IsEqual allocator type.
	typedef core::AllocatorT<IsEqual, core::Primitive::Alloc> Alloc;
	//! IsEqual handle type.
	typedef core::PointerT<IsEqual, core::Primitive::Handle> Handle;
	//! IsEqual IsEqual type.
	typedef core::ContainerT<IsEqual, core::Primitive::Bag> Bag;

	IsEqual();
	IsEqual(const IsEqual& inOriginal);
	virtual ~IsEqual() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IsEqual");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsEqual::getName() const");
	}

	IsEqual& operator=(const IsEqual& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IsEqual_hpp */
