/*
 * Switch.hpp
 *
 *  Created on: 2009-10-15
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

#ifndef SCHNAPS_Plugins_Control_Switch_hpp
#define SCHNAPS_Plugins_Control_Switch_hpp

#include <map>

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Switch SCHNAPS/Basic/Switch.hpp "SCHNAPS/Basic/Switch.hpp"
 *  \brief Primitive class that switches over some values.
 */
class Switch: public SCHNAPS::Core::Primitive {
protected:
	typedef std::map<SCHNAPS::Core::Atom::Handle, unsigned int, SCHNAPS::Core::IsLessPointerPredicate> SwitchMap;

public:
	//! Switch allocator type.
	typedef SCHNAPS::Core::AllocatorT<Switch, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! Switch handle type.
	typedef SCHNAPS::Core::PointerT<Switch, SCHNAPS::Core::Primitive::Handle> Handle;
	//! Switch bag type.
	typedef SCHNAPS::Core::ContainerT<Switch, SCHNAPS::Core::Primitive::Bag> Bag;

	Switch();
	Switch(const Switch& inOriginal);
	virtual ~Switch() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_Switch");
			return lName;
		schnaps_StackTraceEndM("const std::string& Switch::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Switch& operator=(const Switch& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;

private:
	std::string mKeys_Ref;	//!< Reference to switch keys.
	std::string mKeyType;	//!< Type of switch values.
	SwitchMap mSwitchMap;	//!< Map of values to children index associated.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Switch_hpp */
