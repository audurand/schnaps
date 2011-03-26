/*
 * ChoiceIsBetween.hpp
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

#ifndef SCHNAPS_Plugins_Control_ChoiceIsBetween_hpp
#define SCHNAPS_Plugins_Control_ChoiceIsBetween_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class ChoiceIsBetween SCHNAPS/Plugins/Control/ChoiceIsBetween.hpp "SCHNAPS/Plugins/Control/ChoiceIsBetween.hpp"
 *  \brief Branch choice according to some value and choice bounds.
 *  	   Lower boundaries are given and branches are executed according to LowerBound <= Value < HigherBound.
 */
class ChoiceIsBetween: public Core::Primitive {
public:
	//! ChoiceIsBetween allocator type.
	typedef Core::AllocatorT<ChoiceIsBetween, Core::Primitive::Alloc> Alloc;
	//! ChoiceIsBetween handle type.
	typedef Core::PointerT<ChoiceIsBetween, Core::Primitive::Handle> Handle;
	//! ChoiceIsBetween bag type.
	typedef Core::ContainerT<ChoiceIsBetween, Core::Primitive::Bag> Bag;

	ChoiceIsBetween();
	ChoiceIsBetween(const ChoiceIsBetween& inOriginal);
	virtual ~ChoiceIsBetween() {}

	//! Copy operator.
	ChoiceIsBetween& operator=(const ChoiceIsBetween& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_ChoiceIsBetween");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ChoiceIsBetween::getName() const");
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
	std::string mValue_Ref;			//!< Reference to the value to switch on.
	Core::Number::Handle mValue;	//!< A handle to the value to switch on.
	std::string mBounds_Ref;		//!< Reference to choice values.
	Core::Vector::Handle mBounds;	//!< A Handle to the vector of choice lower bounds.
};
} // end of Control namespace  
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_ChoiceIsBetween_hpp */
