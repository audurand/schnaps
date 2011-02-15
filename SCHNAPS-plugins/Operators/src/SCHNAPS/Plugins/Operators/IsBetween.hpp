/*
 * IsBetween.hpp
 *
 *  Created on: 2010-08-19
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

#ifndef SCHNAPS_Plugins_Operators_IsBetween_hpp
#define SCHNAPS_Plugins_Operators_IsBetween_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsBetween SCHNAPS/Plugins/Operators/IsBetween.hpp "SCHNAPS/Plugins/Operators/IsBetween.hpp"
 *  \brief Relational IsBetween operator primitive class.
 */
class IsBetween: public Core::Primitive {
public:
	//! IsIsBetween allocator type.
	typedef Core::AllocatorT<IsBetween, Core::Primitive::Alloc> Alloc;
	//! IsIsBetween handle type.
	typedef Core::PointerT<IsBetween, Core::Primitive::Handle> Handle;
	//! IsIsBetween IsIsBetween type.
	typedef Core::ContainerT<IsBetween, Core::Primitive::Bag> Bag;

	IsBetween();
	IsBetween(const IsBetween& inOriginal);
	virtual ~IsBetween() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_IsBetween");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsBetween::getName() const");
	}

	IsBetween& operator=(const IsBetween& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsBetween_hpp */
