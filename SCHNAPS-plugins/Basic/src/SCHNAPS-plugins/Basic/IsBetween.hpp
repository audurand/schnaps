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

#ifndef basic_IsBetween_hpp
#define basic_IsBetween_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IsBetween SCHNAPS-plugins/Basic/IsBetween.hpp "SCHNAPS-plugins/Basic/IsBetween.hpp"
 *  \brief Relational IsBetween operator primitive class.
 */
class IsBetween: public core::Primitive {
public:
	//! IsIsBetween allocator type.
	typedef core::AllocatorT<IsBetween, core::Primitive::Alloc> Alloc;
	//! IsIsBetween handle type.
	typedef core::PointerT<IsBetween, core::Primitive::Handle> Handle;
	//! IsIsBetween IsIsBetween type.
	typedef core::ContainerT<IsBetween, core::Primitive::Bag> Bag;

	IsBetween();
	IsBetween(const IsBetween& inOriginal);
	virtual ~IsBetween() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IsBetween");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsBetween::getName() const");
	}

	IsBetween& operator=(const IsBetween& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IsBetween_hpp */
