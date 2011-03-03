/*
 * Eventv2.hpp
 *
 *  Created on: 2010-12-02
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

#ifndef SCHNAPS_Plugins_Meds_Eventv2_hpp
#define SCHNAPS_Plugins_Meds_Eventv2_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Eventv2 SCHNAPS/Plugins/Meds/Eventv2.hpp "SCHNAPS/Plugins/Meds/Eventv2.hpp"
 *  \brief Primitive that implements an Eventv2 that occurs on individuals.
 */
class Eventv2: public Core::Primitive {
public:
	//! Eventv2 allocator type
	typedef Core::AllocatorT<Eventv2, Core::Primitive::Alloc> Alloc;
	//! Eventv2 handle type
	typedef Core::PointerT<Eventv2, Core::Primitive::Handle> Handle;
	//! Eventv2 bag type
	typedef Core::ContainerT<Eventv2, Core::Primitive::Bag> Bag;

	Eventv2();
	Eventv2(const Eventv2& inOriginal);
	virtual ~Eventv2() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Eventv2");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Eventv2::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;						//!< Name of event.
	std::string mProbabilityVariableLabel;	//!< Individual variable reresenting the probability of this individual of having that event.
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Eventv2_hpp */
