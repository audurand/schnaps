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

#ifndef osteoporosis_Event_hpp
#define osteoporosis_Event_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace osteoporosis {
/*!
 *  \class Event SCHNAPS-plugins/Osteoporosis/Event.hpp "SCHNAPS-plugins/Osteoporosis/Event.hpp"
 *  \brief Primitive that implements a prevention campain on the population.
 */
class Event: public core::Primitive {
public:
	//! Event allocator type.
	typedef core::AllocatorT<Event, core::Primitive::Alloc> Alloc;
	//! Event handle type.
	typedef core::PointerT<Event, core::Primitive::Handle> Handle;
	//! Event bag type.
	typedef core::ContainerT<Event, core::Primitive::Bag> Bag;

	Event();
	Event(const Event& inOriginal);
	virtual ~Event() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_Event");
			return lName;
		schnaps_StackTraceEndM("const std::string& Event::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	std::string mProbabilityVariableLabel;
};
}
// end of osteoporosis namespace

#endif /* osteoporosis_Event_hpp */
