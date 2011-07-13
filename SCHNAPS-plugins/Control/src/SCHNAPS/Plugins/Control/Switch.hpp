/*
 * Switch.hpp
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

#ifndef SCHNAPS_Plugins_Control_Switch_hpp
#define SCHNAPS_Plugins_Control_Switch_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

#include <map>

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Switch SCHNAPS/Plugins/Control/Switch.hpp "SCHNAPS/Plugins/Control/Switch.hpp"
 *  \brief Switch case over a value with default case available.
 */
class Switch: public Core::Primitive {
protected:
	typedef std::map<Core::Atom::Handle, unsigned int, Core::IsLessPointerPredicate> SwitchMap;

public:
	//! Switch allocator type.
	typedef Core::AllocatorT<Switch, Core::Primitive::Alloc> Alloc;
	//! Switch handle type.
	typedef Core::PointerT<Switch, Core::Primitive::Handle> Handle;
	//! Switch bag type.
	typedef Core::ContainerT<Switch, Core::Primitive::Bag> Bag;

	Switch();
	Switch(const Switch& inOriginal);
	virtual ~Switch() {}

	//! Copy operator.
	Switch& operator=(const Switch& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_Switch");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Switch::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mKeys_Ref;	//!< Reference to switch keys.
	SwitchMap mSwitchMap;	//!< Map of keys to associated argument index.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Switch_hpp */
