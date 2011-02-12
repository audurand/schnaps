/*
 * IsGreaterOrEqual.hpp
 *
 *  Created on: 2010-08-19
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

#ifndef basic_IsGreaterOrEqual_hpp
#define basic_IsGreaterOrEqual_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IsGreaterOrEqual SCHNAPS-plugins/Basic/IsGreaterOrEqual.hpp "SCHNAPS-plugins/Basic/IsGreaterOrEqual.hpp"
 *  \brief Relational IsGreaterOrEqual operator primitive class.
 */
class IsGreaterOrEqual: public core::Primitive {
public:
	//! IsIsGreaterOrEqual allocator type.
	typedef core::AllocatorT<IsGreaterOrEqual, core::Primitive::Alloc> Alloc;
	//! IsIsGreaterOrEqual handle type.
	typedef core::PointerT<IsGreaterOrEqual, core::Primitive::Handle> Handle;
	//! IsIsGreaterOrEqual IsIsGreaterOrEqual type.
	typedef core::ContainerT<IsGreaterOrEqual, core::Primitive::Bag> Bag;

	IsGreaterOrEqual();
	IsGreaterOrEqual(const IsGreaterOrEqual& inOriginal);
	virtual ~IsGreaterOrEqual() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IsGreaterOrEqual");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsGreaterOrEqual::getName() const");
	}

	IsGreaterOrEqual& operator=(const IsGreaterOrEqual& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IsGreaterOrEqual_hpp */
