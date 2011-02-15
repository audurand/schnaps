/*
 * IsLessOrEqual.hpp
 *
 *  Created on: 2010-08-19
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

#ifndef SCHNAPS_Plugins_Operators_IsLessOrEqual_hpp
#define SCHNAPS_Plugins_Operators_IsLessOrEqual_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsLessOrEqual SCHNAPS/Plugins/Operators/IsLessOrEqual.hpp "SCHNAPS/Plugins/Operators/IsLessOrEqual.hpp"
 *  \brief Relational IsLessOrEqual operator primitive class.
 */
class IsLessOrEqual: public Core::Primitive {
public:
	//! IsIsLessOrEqual allocator type.
	typedef Core::AllocatorT<IsLessOrEqual, Core::Primitive::Alloc> Alloc;
	//! IsIsLessOrEqual handle type.
	typedef Core::PointerT<IsLessOrEqual, Core::Primitive::Handle> Handle;
	//! IsIsLessOrEqual IsIsLessOrEqual type.
	typedef Core::ContainerT<IsLessOrEqual, Core::Primitive::Bag> Bag;

	IsLessOrEqual();
	IsLessOrEqual(const IsLessOrEqual& inOriginal);
	virtual ~IsLessOrEqual() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_IsLessOrEqual");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsLessOrEqual::getName() const");
	}

	IsLessOrEqual& operator=(const IsLessOrEqual& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsLessOrEqual_hpp */
