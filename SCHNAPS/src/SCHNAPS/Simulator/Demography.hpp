/*
 * Demography.hpp
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

#ifndef simulator_Demography_hpp
#define simulator_Demography_hpp

#include "SCHNAPS/Core/HashString.hpp"
#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

#include <vector>

namespace simulator {

/*!
 *  \class Demography Simulator/include/Demography.hpp "Simulator/include/Demography.hpp"
 *  \brief Demography class.
 */
class Demography: public core::Object, public std::vector<std::pair<std::string, core::PrimitiveTree::Handle> > {
public:
	//! Demography allocator type.
	typedef core::AllocatorT<Demography, core::Object::Alloc> Alloc;
	//! Demography handle type.
	typedef core::PointerT<Demography, core::Object::Handle> Handle;
	//! Demography bag type.
	typedef core::ContainerT<Demography, core::Object::Bag> Bag;

	Demography() {}
	Demography(const Demography& inOriginal);
	virtual ~Demography() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Demography");
			return lName;
		schnaps_StackTraceEndM("const std::string& Demography::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
};
} // end of simulator namespace

#endif /* simulator_Demography_hpp */
