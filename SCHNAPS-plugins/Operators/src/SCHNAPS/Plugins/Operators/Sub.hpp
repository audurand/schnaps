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

#ifndef SCHNAPS_Plugins_Operators_Sub_hpp
#define SCHNAPS_Plugins_Operators_Sub_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Sub SCHNAPS/Plugins/Operators/Sub.hpp "SCHNAPS/Plugins/Operators/Sub.hpp"
 *  \brief Subtraction primitive class.
 */
class Sub: public Core::Primitive {
public:
	//! Sub allocator type.
	typedef Core::AllocatorT<Sub, Core::Primitive::Alloc> Alloc;
	//! Sub handle type.
	typedef Core::PointerT<Sub, Core::Primitive::Handle> Handle;
	//! Sub bag type.
	typedef Core::ContainerT<Sub, Core::Primitive::Bag> Bag;

	Sub();
	Sub(const Sub& inOriginal);
	virtual ~Sub() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Sub");
			return lName;
		schnaps_StackTraceEndM("const std::string& Sub::getName() const");
	}

	Sub& operator=(const Sub& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Sub_hpp */
