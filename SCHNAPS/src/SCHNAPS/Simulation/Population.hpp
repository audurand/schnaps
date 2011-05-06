/*
 * Population.hpp
 *
 *  Created on: 2009-03-17
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

#ifndef SCHNAPS_Simulation_Population_hpp
#define SCHNAPS_Simulation_Population_hpp

#include "SCHNAPS/Simulation/Individual.hpp"

#define SUB_POPULATION_SIZE 100000

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Population SCHNAPS/Simulation/Population.hpp "SCHNAPS/Simulation/Population.hpp"
 *  \brief Population class.
 */
class Population: public Individual::Bag {
public:
	//! Population allocator type.
	typedef SCHNAPS::Core::AllocatorT<Population, Individual::Bag::Alloc> Alloc;
	//! Population handle type.
	typedef SCHNAPS::Core::PointerT<Population, Individual::Bag::Handle> Handle;
	//! Population bag type.
	typedef SCHNAPS::Core::ContainerT<Population, Individual::Bag::Bag> Bag;

	Population() {}
	Population(const Population& inOriginal);
	virtual ~Population() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Population");
			return lName;
		schnaps_StackTraceEndM("const std::string& Population::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	bool addIndividuals(Individual::Bag::Handle lNewIndividuals);

private:
	void readFromIndexWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	void writeSubPopulation(PACC::XML::Streamer& ioStreamer, unsigned int inFrom, unsigned int inTo, bool inIndent = true) const;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Population_hpp */
