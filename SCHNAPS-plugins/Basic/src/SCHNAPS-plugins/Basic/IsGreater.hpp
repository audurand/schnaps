/*
 * IsGreater.hpp
 *
 *  Created on: 2010-04-20
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

#ifndef basic_IsGreater_hpp
#define basic_IsGreater_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class IsGreater SCHNAPS-plugins/Basic/IsGreater.hpp "SCHNAPS-plugins/Basic/IsGreater.hpp"
 *  \brief Relational IsGreater operator primitive class.
 */
class IsGreater: public core::Primitive {
public:
	//! IsIsGreater allocator type.
	typedef core::AllocatorT<IsGreater, core::Primitive::Alloc> Alloc;
	//! IsIsGreater handle type.
	typedef core::PointerT<IsGreater, core::Primitive::Handle> Handle;
	//! IsIsGreater IsIsGreater type.
	typedef core::ContainerT<IsGreater, core::Primitive::Bag> Bag;

	IsGreater();
	IsGreater(const IsGreater& inOriginal);
	virtual ~IsGreater() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_IsGreater");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsGreater::getName() const");
	}

	IsGreater& operator=(const IsGreater& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_IsGreater_hpp */
