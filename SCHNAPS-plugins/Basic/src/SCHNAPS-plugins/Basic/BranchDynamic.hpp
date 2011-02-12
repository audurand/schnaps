/*
 * BranchDynamic.hpp
 *
 *  Created on: 2010-01-12
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

#ifndef basic_BranchDynamic_hpp
#define basic_BranchDynamic_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class BranchDynamic SCHNAPS-plugins/Basic/BranchDynamic.hpp "SCHNAPS-plugins/Basic/BranchDynamic.hpp"
 *  \brief Primitive class that returns the result of a branching with dynamic weights.
 */
class BranchDynamic: public core::Primitive {
public:
	//! BranchDynamic allocator type.
	typedef core::AllocatorT<BranchDynamic, core::Primitive::Alloc> Alloc;
	//! BranchDynamic handle type.
	typedef core::PointerT<BranchDynamic, core::Primitive::Handle> Handle;
	//! BranchDynamic bag type.
	typedef core::ContainerT<BranchDynamic, core::Primitive::Bag> Bag;

	BranchDynamic();
	BranchDynamic(const BranchDynamic& inOriginal);
	virtual ~BranchDynamic() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_BranchDynamic");
			return lName;
		schnaps_StackTraceEndM("const std::string& BranchDynamic::getName() const");
	}

	BranchDynamic& operator=(const BranchDynamic& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_BranchDynamic_hpp */
