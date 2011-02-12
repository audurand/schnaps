/*
 * Mod.hpp
 *
 *  Created on: 2010-02-20
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

#ifndef basic_Mod_hpp
#define basic_Mod_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Mod SCHNAPS-plugins/Basic/Mod.hpp "SCHNAPS-plugins/Basic/Mod.hpp"
 *  \brief Mathematical modulo operation primitive class.
 */
class Mod: public core::Primitive {
public:
	//! Mod allocator type.
	typedef core::AllocatorT<Mod, core::Primitive::Alloc> Alloc;
	//! Mod handle type.
	typedef core::PointerT<Mod, core::Primitive::Handle> Handle;
	//! Mod bag type.
	typedef core::ContainerT<Mod, core::Primitive::Bag> Bag;

	Mod();
	Mod(const Mod& inOriginal);
	virtual ~Mod() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Mod");
			return lName;
		schnaps_StackTraceEndM("const std::string& Mod::getName() const");
	}

	Mod& operator=(const Mod& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_Mod_hpp */
