/*
 * NotComplex.hpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#ifndef SCHNAPS_Plugins_Operators_NotComplex_hpp
#define SCHNAPS_Plugins_Operators_NotComplex_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class NotComplex SCHNAPS/Plugins/Operators/NotComplex.hpp "SCHNAPS/Plugins/Operators/NotComplex.hpp"
 *  \brief Logical NOT operator.
 */
class NotComplex: public Core::Primitive {
public:
	//! NotComplex allocator type.
	typedef Core::AllocatorT<NotComplex, Core::Primitive::Alloc> Alloc;
	//! NotComplex handle type.
	typedef Core::PointerT<NotComplex, Core::Primitive::Handle> Handle;
	//! NotComplex bag type.
	typedef Core::ContainerT<NotComplex, Core::Primitive::Bag> Bag;

	NotComplex();
	NotComplex(const NotComplex& inNotComplexComplexiginal);
	virtual ~NotComplex() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Operators_NotComplex");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NotComplex::getName() const");
	}

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_NotComplex_hpp */
