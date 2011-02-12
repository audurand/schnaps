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

#ifndef simulator_Population_hpp
#define simulator_Population_hpp

#include "SCHNAPS/Simulator/Individual.hpp"

#define SUB_POPULATION_SIZE 100000

namespace simulator {
/*!
 *  \class Population Simulator/include/Population.hpp "Simulator/include/Population.hpp"
 *  \brief Population class.
 */
class Population: public Individual::Bag {
public:
	//! Population allocator type.
	typedef core::AllocatorT<Population, Individual::Bag::Alloc> Alloc;
	//! Population handle type.
	typedef core::PointerT<Population, Individual::Bag::Handle> Handle;
	//! Population bag type.
	typedef core::ContainerT<Population, Individual::Bag::Bag> Bag;

	Population() {}
	Population(const Population& inOriginal);
	virtual ~Population() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Population");
			return lName;
		schnaps_StackTraceEndM("const std::string& Population::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	bool addIndividuals(Individual::Bag::Handle lNewIndividuals);

private:
	void readFromIndexWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	void writeSubPopulation(PACC::XML::Streamer& ioStreamer, unsigned int inFrom, unsigned int inTo, bool inIndent = true) const;
};
} // end of simulator namespace

#endif /* simulator_Population_hpp */
