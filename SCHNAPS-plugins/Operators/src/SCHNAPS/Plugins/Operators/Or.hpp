/*
 * Or.hpp
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

#ifndef SCHNAPS_Plugins_Operators_Or_hpp
#define SCHNAPS_Plugins_Operators_Or_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Or SCHNAPS/Plugins/Operators/Or.hpp "SCHNAPS/Plugins/Operators/Or.hpp"
 *  \brief Logical OR primitive class.
 */
class Or: public Core::Primitive {
public:
	//! Or allocator type.
	typedef Core::AllocatorT<Or, Core::Primitive::Alloc> Alloc;
	//! Or handle type.
	typedef Core::PointerT<Or, Core::Primitive::Handle> Handle;
	//! Or bag type.
	typedef Core::ContainerT<Or, Core::Primitive::Bag> Bag;

	Or();
	Or(const Or& inOriginal);
	virtual ~Or() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Or");
			return lName;
		schnaps_StackTraceEndM("const std::string& Or::getName() const");
	}

	Or& operator=(const Or& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Or_hpp */
