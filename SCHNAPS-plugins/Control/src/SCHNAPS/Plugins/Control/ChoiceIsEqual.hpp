/*
 * ChoiceIsEqual.hpp
 *
 *  Created on: 2010-11-20
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

#ifndef SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp
#define SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp

#include <map>

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class ChoiceIsEqual Simulator/Core/include/ChoiceIsEqual.hpp "Simulator/Core/include/ChoiceIsEqual.hpp"
 *  \brief Primitive class that choses over some values.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_ChoiceIsEqual");
			return lName;
		schnaps_StackTraceEndM("const std::string& ChoiceIsEqual::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	ChoiceIsEqual& operator=(const ChoiceIsEqual& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mChoiceVariableLabel;	//!< current individual variable label to switch on
	std::string mChoices_Ref;			//!< reference to choice values
	ChoiceMap mChoices;					//!< map of values to children index associated
	bool mCatchError;					//!< catch error if variable not in choices
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_ChoiceIsEqual_hpp */
