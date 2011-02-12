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

#ifndef basic_Div_hpp
#define basic_Div_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Div SCHNAPS-plugins/Basic/Div.hpp "SCHNAPS-plugins/Basic/Div.hpp"
 *  \brief Mathematical division operation primitive class.
 */
class Div: public core::Primitive {
public:
	//! Div allocator type.
	typedef core::AllocatorT<Div, core::Primitive::Alloc> Alloc;
	//! Div handle type.
	typedef core::PointerT<Div, core::Primitive::Handle> Handle;
	//! Div bag type.
	typedef core::ContainerT<Div, core::Primitive::Bag> Bag;

	Div();
	Div(const Div& inOriginal);
	virtual ~Div() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Div");
			return lName;
		schnaps_StackTraceEndM("const std::string& Div::getName() const");
	}

	Div& operator=(const Div& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Div_hpp */
