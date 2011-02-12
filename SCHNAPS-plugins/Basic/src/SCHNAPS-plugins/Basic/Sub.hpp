/*
 * Sub.hpp
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

#ifndef basic_Sub_hpp
#define basic_Sub_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Sub SCHNAPS-plugins/Basic/Sub.hpp "SCHNAPS-plugins/Basic/Sub.hpp"
 *  \brief Subtraction primitive class.
 */
class Sub: public core::Primitive {
public:
	//! Sub allocator type.
	typedef core::AllocatorT<Sub, core::Primitive::Alloc> Alloc;
	//! Sub handle type.
	typedef core::PointerT<Sub, core::Primitive::Handle> Handle;
	//! Sub bag type.
	typedef core::ContainerT<Sub, core::Primitive::Bag> Bag;

	Sub();
	Sub(const Sub& inOriginal);
	virtual ~Sub() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Sub");
			return lName;
		schnaps_StackTraceEndM("const std::string& Sub::getName() const");
	}

	Sub& operator=(const Sub& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Sub_hpp */
