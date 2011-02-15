/*
 * Exp.h
 *
 *  Created on: 2009-03-10
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

#ifndef SCHNAPS_Plugins_Operators_Exp_hpp
#define SCHNAPS_Plugins_Operators_Exp_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Exp SCHNAPS/Plugins/Operators/Exp.hpp "SCHNAPS/Plugins/Operators/Exp.hpp"
 *  \brief Mathematical exponent operation primitive class.
 */
class Exp: public Core::Primitive {
public:
	//! Exp allocator type.
	typedef Core::AllocatorT<Exp, Core::Primitive::Alloc> Alloc;
	//! Exp handle type.
	typedef Core::PointerT<Exp, Core::Primitive::Handle> Handle;
	//! Exp bag type.
	typedef Core::ContainerT<Exp, Core::Primitive::Bag> Bag;

	Exp();
	Exp(const Exp& inOriginal);
	virtual ~Exp() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Exp");
			return lName;
		schnaps_StackTraceEndM("const std::string& Exp::getName() const");
	}

	Exp& operator=(const Exp& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Exp_hpp */
