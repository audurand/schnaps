/*
 * Abs.hpp
 *
 *  Created on: 2009-02-24
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

#ifndef basic_Abs_hpp
#define basic_Abs_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {
/*!
 *  \class Abs SCHNAPS-plugins/Basic/Abs.hpp "SCHNAPS-plugins/Basic/Abs.hpp"
 *  \brief Mathematical absolute value operation primitive class.
 */
class Abs: public core::Primitive {
public:
	//! Abs allocator type.
	typedef core::AllocatorT<Abs, core::Primitive::Alloc> Alloc;
	//! Abs handle type.
	typedef core::PointerT<Abs, core::Primitive::Handle> Handle;
	//! Abs bag type.
	typedef core::ContainerT<Abs, core::Primitive::Bag> Bag;

	Abs();
	Abs(const Abs& inOriginal);
	virtual ~Abs() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Abs");
			return lName;
		schnaps_StackTraceEndM("const std::string& Abs::getName() const");
	}

	Abs& operator=(const Abs& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext);
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Abs_hpp */
