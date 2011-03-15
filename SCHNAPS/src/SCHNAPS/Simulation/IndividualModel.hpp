/*
 * IndividualModel.hpp
 *
 *  Created on: 2010-04-14
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

#ifndef SCHNAPS_Simulation_IndividualModel_hpp
#define SCHNAPS_Simulation_IndividualModel_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#include <set>

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class IndividualModel SCHNAPS/Simulation/IndividualModel.hpp "SCHNAPS/Simulation/IndividualModel.hpp"
 *  \brief IndividualModel class.
 */
class IndividualModel: public SCHNAPS::Core::Object, public std::set<std::string>
{
public:
	//! IndividualModel allocator type.
	typedef SCHNAPS::Core::AllocatorT<IndividualModel, SCHNAPS::Core::Object::Alloc> Alloc;
	//! IndividualModel handle type.
	typedef SCHNAPS::Core::PointerT<IndividualModel, SCHNAPS::Core::Object::Handle> Handle;
	//! IndividualModel bag type.
	typedef SCHNAPS::Core::ContainerT<IndividualModel, SCHNAPS::Core::Object::Bag> Bag;

	IndividualModel() {}
	IndividualModel(const IndividualModel& inOriginal);
	virtual ~IndividualModel() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("IndividualModel");
			return lName;
		schnaps_StackTraceEndM("const std::string& IndividualModel::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_IndividualModel_hpp */
