/*
 * Serialize.hpp
 *
 *  Created on: 2010-02-01
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

#ifndef basic_Serialize_hpp
#define basic_Serialize_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Serialize SCHNAPS-plugins/Basic/Serialize.hpp "SCHNAPS-plugins/Basic/Serialize.hpp"
 *  \brief Primitive class that serializes an atom to string.
 */
class Serialize: public core::Primitive {
public:
	//! Serialize allocator type.
	typedef core::AllocatorT<Serialize, core::Primitive::Alloc> Alloc;
	//! Serialize handle type.
	typedef core::PointerT<Serialize, core::Primitive::Handle> Handle;
	//! Serialize bag type.
	typedef core::ContainerT<Serialize, core::Primitive::Bag> Bag;

	Serialize();
	Serialize(const Serialize& inOriginal);
	virtual ~Serialize() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Serialize");
			return lName;
		schnaps_StackTraceEndM("const std::string& Serialize::getName() const");
	}

	Serialize& operator=(const Serialize& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Serialize_hpp */
