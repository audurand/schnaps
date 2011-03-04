/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
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
 *  \class Component SCHNAPS/Core/Component.hpp "SCHNAPS/Core/Component.hpp"
 *  \brief System component base class.
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

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);

	//! Initialize the component.
	virtual void init(System& ioSystem);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		return mName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Component::getName() const");
	}

	virtual void setName(const std::string& inName) {
		schnaps_StackTraceBeginM();
		mName = inName;
		schnaps_StackTraceEndM("void SCHNAPS::Core::Component::setName(const std::string&)");
	}


protected:
	std::string mName; //!< Name of component.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Component_hpp
