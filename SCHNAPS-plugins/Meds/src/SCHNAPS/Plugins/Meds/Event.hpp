/*
 * Event.hpp
 *
 *  Created on: 2010-11-02
 *  Updated on: 2010-11-02
 *      Author: Audrey Durand
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

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Event SCHNAPS/Plugins/Meds/Event.hpp "SCHNAPS/Plugins/Meds/Event.hpp"
 *  \brief Primitive that implements a prevention campain on the population.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_Event");
			return lName;
		schnaps_StackTraceEndM("const std::string& Event::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	std::string mProbabilityVariableLabel;
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Event_hpp */
