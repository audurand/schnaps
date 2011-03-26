/*
 * ModComplex.hpp
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

#ifndef SCHNAPS_Plugins_Operators_ModComplex_hpp
#define SCHNAPS_Plugins_Operators_ModComplex_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class ModComplex SCHNAPS/Plugins/Operators/ModComplex.hpp "SCHNAPS/Plugins/Operators/ModComplex.hpp"
 *  \brief Mathematical modulo operator.
 */
class ModComplex: public Core::Primitive {
public:
	//! ModComplex allocator type.
	typedef Core::AllocatorT<ModComplex, Core::Primitive::Alloc> Alloc;
	//! ModComplex handle type.
	typedef Core::PointerT<ModComplex, Core::Primitive::Handle> Handle;
	//! ModComplex bag type.
	typedef Core::ContainerT<ModComplex, Core::Primitive::Bag> Bag;

	ModComplex();
	ModComplex(const ModComplex& inOriginal);
	virtual ~ModComplex() {}

	//! Copy operator.
	ModComplex& operator=(const ModComplex& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Operators_ModComplex");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::ModComplex::getName() const");
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

#endif /* SCHNAPS_Plugins_Operators_ModComplex_hpp */
