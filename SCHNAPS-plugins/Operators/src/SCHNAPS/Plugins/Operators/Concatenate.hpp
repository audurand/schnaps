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

#ifndef SCHNAPS_Plugins_Operators_Concatenate_hpp
#define SCHNAPS_Plugins_Operators_Concatenate_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class Concatenate SCHNAPS/Plugins/Operators/Concatenate.hpp "SCHNAPS/Plugins/Operators/Concatenate.hpp"
 *  \brief Logical Concatenate primitive class.
 */
class Concatenate: public Core::Primitive {
public:
	//! Concatenate allocator type.
	typedef Core::AllocatorT<Concatenate, Core::Primitive::Alloc> Alloc;
	//! Concatenate hConcatenatele type.
	typedef Core::PointerT<Concatenate, Core::Primitive::Handle> Handle;
	//! Concatenate bag type.
	typedef Core::ContainerT<Concatenate, Core::Primitive::Bag> Bag;

	Concatenate();
	Concatenate(const Concatenate& inOriginal);
	virtual ~Concatenate() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_Concatenate");
			return lName;
		schnaps_StackTraceEndM("const std::string& Concatenate::getName() const");
	}

	Concatenate& operator=(const Concatenate& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_Concatenate_hpp */
