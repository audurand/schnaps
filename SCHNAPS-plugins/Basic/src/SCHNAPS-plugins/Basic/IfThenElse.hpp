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

#ifndef basic_IfThenElse_hpp
#define basic_IfThenElse_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IfThenElse SCHNAPS-plugins/Basic/IfThenElse.hpp "SCHNAPS-plugins/Basic/IfThenElse.hpp"
 *  \brief Branching IfThenElse primitive class.
 */
class IfThenElse: public core::Primitive {
public:
	//! IfThenElse allocator type.
	typedef core::AllocatorT<IfThenElse, core::Primitive::Alloc> Alloc;
	//! IfThenElse handle type.
	typedef core::PointerT<IfThenElse, core::Primitive::Handle> Handle;
	//! IfThenElse bag type.
	typedef core::ContainerT<IfThenElse, core::Primitive::Bag> Bag;

	IfThenElse();
	IfThenElse(const IfThenElse& inOriginal);
	virtual ~IfThenElse() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IfThenElse");
			return lName;
		schnaps_StackTraceEndM("const std::string& IfThenElse::getName() const");
	}

	IfThenElse& operator=(const IfThenElse& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IfThenElse_hpp */
