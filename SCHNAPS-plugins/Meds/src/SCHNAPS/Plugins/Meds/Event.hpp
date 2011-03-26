/*
 * Event.hpp
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

#ifndef SCHNAPS_Plugins_Meds_Event_hpp
#define SCHNAPS_Plugins_Meds_Event_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Event SCHNAPS/Plugins/Meds/Event.hpp "SCHNAPS/Plugins/Meds/Event.hpp"
 *  \brief Implementation of medical event.
 */
class Event: public Core::Primitive {
public:
	//! Event allocator type.
	typedef Core::AllocatorT<Event, Core::Primitive::Alloc> Alloc;
	//! Event handle type.
	typedef Core::PointerT<Event, Core::Primitive::Handle> Handle;
	//! Event bag type.
	typedef Core::ContainerT<Event, Core::Primitive::Bag> Bag;

	Event();
	Event(const Event& inOriginal);
	virtual ~Event() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Event");
		return lName;
		schnaps_StackTraceEndM("const std::string& Event::getName() const");
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
	std::string mLabel;					//!< Label of event.
	std::string mProbability_Ref;		//!< Reference to the probability of event.
	Core::Double::Handle mProbability;	//!< A handle to the probaility of event.
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Event_hpp */
