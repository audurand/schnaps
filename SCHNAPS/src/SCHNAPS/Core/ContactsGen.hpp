/*
 * ContactsGen.hpp
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

#ifndef CONTACTSGEN_HPP
#define CONTACTSGEN_HPP

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AbstractAllocT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/System.hpp"
#include "SCHNAPS/Core/Vector.hpp"


#include "SCHNAPS/Simulation/Individual.hpp"


namespace SCHNAPS {
namespace Core {


/*!
 * \class ContactsGen SCHNAPS/Core/ContactsGen.hpp "SCHNAPS/Core/ContactsGen.hpp"
 * \brief Contacts generator base class.
 */
class ContactsGen: public Object {
public:
	
	//! ContactsGen allocator type.
	typedef AbstractAllocT<ContactsGen, Object::Alloc> Alloc;
	//! ContactsGen handle type.
	typedef PointerT<ContactsGen, Object::Handle> Handle;
	//! ContactsGen bag type.
	typedef ContainerT<ContactsGen, Object::Bag> Bag;


	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("ContactsGen");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::ContactsGen::getName() const");
	}

	//! Generate contacts
	virtual void generate(Simulation::Individual::Bag::Handle inPop, Core::System::Handle inSystem, std::vector<Core::Vector::Handle>& ioList) const;


};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif 
