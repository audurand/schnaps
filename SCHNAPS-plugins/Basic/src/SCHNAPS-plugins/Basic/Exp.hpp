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

#ifndef basic_Exp_hpp
#define basic_Exp_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {
/*!
 *  \class Exp SCHNAPS-plugins/Basic/Exp.hpp "SCHNAPS-plugins/Basic/Exp.hpp"
 *  \brief Mathematical exponent operation primitive class.
 */
class Exp: public core::Primitive {
public:
	//! Exp allocator type.
	typedef core::AllocatorT<Exp, core::Primitive::Alloc> Alloc;
	//! Exp handle type.
	typedef core::PointerT<Exp, core::Primitive::Handle> Handle;
	//! Exp bag type.
	typedef core::ContainerT<Exp, core::Primitive::Bag> Bag;

	Exp();
	Exp(const Exp& inOriginal);
	virtual ~Exp() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Exp");
			return lName;
		schnaps_StackTraceEndM("const std::string& Exp::getName() const");
	}

	Exp& operator=(const Exp& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Exp_hpp */
