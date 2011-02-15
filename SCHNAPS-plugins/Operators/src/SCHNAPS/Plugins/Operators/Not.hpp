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

#ifndef SCHNAPS_Plugins_Operators_Not_hpp
#define SCHNAPS_Plugins_Operators_Not_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Not SCHNAPS/Plugins/Operators/Not.hpp "SCHNAPS/Plugins/Operators/Not.hpp"
 *  \brief Logical NOT primitive class.
 */
class Not: public Core::Primitive {
public:
	//! Not allocator type.
	typedef Core::AllocatorT<Not, Core::Primitive::Alloc> Alloc;
	//! Not handle type.
	typedef Core::PointerT<Not, Core::Primitive::Handle> Handle;
	//! Not bag type.
	typedef Core::ContainerT<Not, Core::Primitive::Bag> Bag;

	Not();
	Not(const Not& inOriginal);
	virtual ~Not() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Not");
			return lName;
		schnaps_StackTraceEndM("const std::string& Not::getName() const");
	}

	Not& operator=(const Not& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Not_hpp */
