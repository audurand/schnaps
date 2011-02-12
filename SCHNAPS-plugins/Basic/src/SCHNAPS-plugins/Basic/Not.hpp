/*
 * Not.hpp
 *
 *  Created on: 2010-02-20
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

#ifndef basic_Not_hpp
#define basic_Not_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Not SCHNAPS-plugins/Basic/Not.hpp "SCHNAPS-plugins/Basic/Not.hpp"
 *  \brief Logical NOT primitive class.
 */
class Not: public core::Primitive {
public:
	//! Not allocator type.
	typedef core::AllocatorT<Not, core::Primitive::Alloc> Alloc;
	//! Not handle type.
	typedef core::PointerT<Not, core::Primitive::Handle> Handle;
	//! Not bag type.
	typedef core::ContainerT<Not, core::Primitive::Bag> Bag;

	Not();
	Not(const Not& inOriginal);
	virtual ~Not() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Not");
			return lName;
		schnaps_StackTraceEndM("const std::string& Not::getName() const");
	}

	Not& operator=(const Not& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Not_hpp */
