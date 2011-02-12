/*
 * And.hpp
 *
 *  Created on: 2009-02-19
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

#ifndef basic_And_hpp
#define basic_And_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class And SCHNAPS-plugins/Basic/And.hpp "SCHNAPS-plugins/Basic/And.hpp"
 *  \brief Logical AND primitive class.
 */
class And: public core::Primitive {
public:
	//! And allocator type.
	typedef core::AllocatorT<And, core::Primitive::Alloc> Alloc;
	//! And handle type.
	typedef core::PointerT<And, core::Primitive::Handle> Handle;
	//! And bag type.
	typedef core::ContainerT<And, core::Primitive::Bag> Bag;

	And();
	And(const And& inOriginal);
	virtual ~And() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_And");
			return lName;
		schnaps_StackTraceEndM("const std::string& And::getName() const");
	}

	And& operator=(const And& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_And_hpp */
