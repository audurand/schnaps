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

#ifndef basic_Or_hpp
#define basic_Or_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Or SCHNAPS-plugins/Basic/Or.hpp "SCHNAPS-plugins/Basic/Or.hpp"
 *  \brief Logical OR primitive class.
 */
class Or: public core::Primitive {
public:
	//! Or allocator type.
	typedef core::AllocatorT<Or, core::Primitive::Alloc> Alloc;
	//! Or handle type.
	typedef core::PointerT<Or, core::Primitive::Handle> Handle;
	//! Or bag type.
	typedef core::ContainerT<Or, core::Primitive::Bag> Bag;

	Or();
	Or(const Or& inOriginal);
	virtual ~Or() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Or");
			return lName;
		schnaps_StackTraceEndM("const std::string& Or::getName() const");
	}

	Or& operator=(const Or& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Or_hpp */
