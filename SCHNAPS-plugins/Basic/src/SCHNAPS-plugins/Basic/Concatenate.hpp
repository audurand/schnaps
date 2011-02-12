/*
 * Concatenate.hpp
 *
 *  Created on: 2010-06-24
 *  Author: Audrey DurConcatenate
 *
 * This program is free software: you can redistribute it Concatenate/or modify
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

#ifndef basic_Concatenate_hpp
#define basic_Concatenate_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Concatenate SCHNAPS-plugins/Basic/Concatenate.hpp "SCHNAPS-plugins/Basic/Concatenate.hpp"
 *  \brief Logical Concatenate primitive class.
 */
class Concatenate: public core::Primitive {
public:
	//! Concatenate allocator type.
	typedef core::AllocatorT<Concatenate, core::Primitive::Alloc> Alloc;
	//! Concatenate hConcatenatele type.
	typedef core::PointerT<Concatenate, core::Primitive::Handle> Handle;
	//! Concatenate bag type.
	typedef core::ContainerT<Concatenate, core::Primitive::Bag> Bag;

	Concatenate();
	Concatenate(const Concatenate& inOriginal);
	virtual ~Concatenate() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Concatenate");
			return lName;
		schnaps_StackTraceEndM("const std::string& Concatenate::getName() const");
	}

	Concatenate& operator=(const Concatenate& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Concatenate_hpp */
