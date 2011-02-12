/*
 * Pow.h
 *
 *  Created on: 2010-08-26
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

#ifndef basic_Pow_hpp
#define basic_Pow_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {
/*!
 *  \class Pow SCHNAPS-plugins/Basic/Pow.hpp "SCHNAPS-plugins/Basic/Pow.hpp"
 *  \brief Mathematical exponent operation primitive class.
 */
class Pow: public core::Primitive {
public:
	//! Pow allocator type.
	typedef core::AllocatorT<Pow, core::Primitive::Alloc> Alloc;
	//! Pow handle type.
	typedef core::PointerT<Pow, core::Primitive::Handle> Handle;
	//! Pow bag type.
	typedef core::ContainerT<Pow, core::Primitive::Bag> Bag;

	Pow();
	Pow(const Pow& inOriginal);
	virtual ~Pow() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Pow");
			return lName;
		schnaps_StackTraceEndM("const std::string& Pow::getName() const");
	}

	Pow& operator=(const Pow& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Pow_hpp */
