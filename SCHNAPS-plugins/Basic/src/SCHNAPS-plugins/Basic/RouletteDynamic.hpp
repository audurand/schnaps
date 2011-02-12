/*
 * RouletteDynamic.hpp
 *
 *  Created on: 2010-01-11
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

#ifndef basic_RouletteDynamic_hpp
#define basic_RouletteDynamic_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class RouletteDynamic SCHNAPS-plugins/Basic/RouletteDynamic.hpp "SCHNAPS-plugins/Basic/RouletteDynamic.hpp"
 *  \brief Primitive class that returns the result of a roulette with dynamic weights.
 */
class RouletteDynamic: public core::Primitive {
public:
	//! RouletteDynamic allocator type.
	typedef core::AllocatorT<RouletteDynamic, core::Primitive::Alloc> Alloc;
	//! RouletteDynamic handle type.
	typedef core::PointerT<RouletteDynamic, core::Primitive::Handle> Handle;
	//! RouletteDynamic bag type.
	typedef core::ContainerT<RouletteDynamic, core::Primitive::Bag> Bag;

	RouletteDynamic();
	RouletteDynamic(const RouletteDynamic& inOriginal);
	virtual ~RouletteDynamic() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_RouletteDynamic");
			return lName;
		schnaps_StackTraceEndM("const std::string& RouletteDynamic::getName() const");
	}

	RouletteDynamic& operator=(const RouletteDynamic& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_RouletteDynamic_hpp */
