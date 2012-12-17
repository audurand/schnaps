/*
 * ChoiceIsEqual.hpp
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

#ifndef SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp
#define SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

#include <map>

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class ChoiceIsEqual SCHNAPS/Plugins/Control/ChoiceIsEqual.hpp "SCHNAPS/Plugins/Control/ChoiceIsEqual.hpp"
 *  \brief Execute node branch according to some value over a set of possibilities (choices).
 * 		   WARNING: There is no default choice so the value MUST BE in choices. 
 */
class ChoiceIsEqual: public Core::Primitive {
protected:
	typedef std::map<Core::Atom::Handle, unsigned int, Core::IsLessPointerPredicate> ChoiceMap;

public:
	//! ChoiceIsEqual allocator type.
	typedef Core::AllocatorT<ChoiceIsEqual, Core::Primitive::Alloc> Alloc;
	//! ChoiceIsEqual handle type.
	typedef Core::PointerT<ChoiceIsEqual, Core::Primitive::Handle> Handle;
	//! ChoiceIsEqual bag type.
	typedef Core::ContainerT<ChoiceIsEqual, Core::Primitive::Bag> Bag;

	ChoiceIsEqual();
	ChoiceIsEqual(const ChoiceIsEqual& inOriginal);
	virtual ~ChoiceIsEqual() {}

	//! Copy operator.
	ChoiceIsEqual& operator=(const ChoiceIsEqual& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_ChoiceIsEqual");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsEqual::getName() const");
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
	std::string mValue_Ref;		//!< Reference to the value to switch on.
	Core::Atom::Handle mValue;	//!< A handle to the value to switch on.
	std::string mChoices_Ref;	//!< Reference to choice values.
	ChoiceMap mChoiceMap;		//!< Map of values to children index associated.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp */
