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

#ifndef simulator_IndividualModel_hpp
#define simulator_IndividualModel_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/HashString.hpp"

#include <set>

namespace simulator {

/*!
 *  \class IndividualModel Simulator/include/IndividualModel.hpp "Simulator/include/IndividualModel.hpp"
 *  \brief IndividualModel class.
 */
class IndividualModel: public core::Object, public std::set<std::string>
{
public:
	//! IndividualModel allocator type.
	typedef core::AllocatorT<IndividualModel, core::Object::Alloc> Alloc;
	//! IndividualModel handle type.
	typedef core::PointerT<IndividualModel, core::Object::Handle> Handle;
	//! IndividualModel bag type.
	typedef core::ContainerT<IndividualModel, core::Object::Bag> Bag;

	IndividualModel() {}
	IndividualModel(const IndividualModel& inOriginal);
	virtual ~IndividualModel() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("IndividualModel");
			return lName;
		schnaps_StackTraceEndM("const std::string& IndividualModel::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
};
} // end of simulator namespace

#endif /* simulator_IndividualModel_hpp */
