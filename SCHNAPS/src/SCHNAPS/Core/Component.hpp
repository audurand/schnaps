/*
 * Component.hpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#ifndef SCHNAPS_Core_Component_hpp
#define SCHNAPS_Core_Component_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"

namespace SCHNAPS {
namespace Core {

// forward declaration
class System;

/*!
 * \class Component SCHNAPS/Core/Component.hpp "SCHNAPS/Core/Component.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief System component base class.
 */
class Component: public Object {
public:

	//! Component allocator type.
	typedef AllocatorT<Component, Object::Alloc> Alloc;
	//! Component handle type.
	typedef PointerT<Component, Object::Handle> Handle;
	//! Component bag type.
	typedef ContainerT<Component, Object::Bag> Bag;

	explicit Component(std::string inName = "Unnamed component");
	virtual ~Component() {}

	//! Read the object from XML using System.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);

	//! Initialize the component.
	virtual void init(System& ioSystem);

	/*!
	 * \brief Return a const reference to the name of the component.
	 * \return A const reference to the name of the component.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		return mName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Component::getName() const");
	}

	/*!
	 * \brief Set the name of the component.
	 * \param inName A const reference to the new name.
	 */
	virtual void setName(const std::string& inName) {
		schnaps_StackTraceBeginM();
		mName = inName;
		schnaps_StackTraceEndM("void SCHNAPS::Core::Component::setName(const std::string&)");
	}


protected:
	std::string mName; //!< Name of the component.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Component_hpp
