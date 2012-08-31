/*
 * Base.hpp
 *
 * SCHNAPS
 * Copyright (C) 2012 by Xavier Douville
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

#ifndef SCHNAPS_Plugins_Contacts_Base_hpp
#define SCHNAPS_Plugins_Contacts_Base_hpp

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/XML.hpp"

#define AGE_GROUP_VARIABLE "trancheAge"

namespace SCHNAPS {
namespace Plugins {
namespace Contacts {

/*!
 *  \class Base SCHNAPS-plugins/Contacts/Base.hpp "SCHNAPS-plugins/Contacts/Base.hpp"
 *  \brief Basic algorithm to generate reciprocal contacts. Each individual is a member of an age group and the number of contacts depend on the group.
 */
class Base: public Core::ContactsGen {
public:
	//! Base allocator type.
	typedef Core::AllocatorT<Base, Core::Primitive::Alloc> Alloc;
	//! Base handle type.
	typedef Core::PointerT<Base, Core::Primitive::Handle> Handle;
	//! Base bag type.
	typedef Core::ContainerT<Base, Core::Primitive::Bag> Bag;


	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Contacts_Base");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Contacts::Base::getName() const");
	}

	virtual void generate(Simulation::Individual::Bag::Handle inPop, Core::System::Handle inSystem, std::vector<Core::Vector::Handle>& ioList) const;

};
} // end of Contacts namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Contacts_Base_hpp */
