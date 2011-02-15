/*
 * Div.hpp
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

#ifndef SCHNAPS_Plugins_Operators_Div_hpp
#define SCHNAPS_Plugins_Operators_Div_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Div SCHNAPS/Plugins/Operators/Div.hpp "SCHNAPS/Plugins/Operators/Div.hpp"
 *  \brief Mathematical division operation primitive class.
 */
class Div: public Core::Primitive {
public:
	//! Div allocator type.
	typedef Core::AllocatorT<Div, Core::Primitive::Alloc> Alloc;
	//! Div handle type.
	typedef Core::PointerT<Div, Core::Primitive::Handle> Handle;
	//! Div bag type.
	typedef Core::ContainerT<Div, Core::Primitive::Bag> Bag;

	Div();
	Div(const Div& inOriginal);
	virtual ~Div() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Div");
			return lName;
		schnaps_StackTraceEndM("const std::string& Div::getName() const");
	}

	Div& operator=(const Div& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Div_hpp */
