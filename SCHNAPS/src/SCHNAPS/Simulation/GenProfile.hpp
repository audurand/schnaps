/*
 * GenProfile.hpp
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

#ifndef SCHNAPS_Simulation_GenProfile_hpp
#define SCHNAPS_Simulation_GenProfile_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"
#include "SCHNAPS/Simulation/Demography.hpp"
#include "SCHNAPS/Simulation/IndividualModel.hpp"
#include "SCHNAPS/Simulation/SimulationVariables.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class GenProfile SCHNAPS/Simulation/GenProfile.hpp "SCHNAPS/Simulation/GenProfile.hpp"
 *  \brief GenProfile class.
 */
class GenProfile: public SCHNAPS::Core::Object {
public:
	//! GenProfile allocator type.
	typedef SCHNAPS::Core::AllocatorT<GenProfile, SCHNAPS::Core::Object::Alloc> Alloc;
	//! GenProfile handle type.
	typedef SCHNAPS::Core::PointerT<GenProfile, SCHNAPS::Core::Object::Handle> Handle;
	//! GenProfile bag type.
	typedef SCHNAPS::Core::ContainerT<GenProfile, SCHNAPS::Core::Object::Bag> Bag;

	GenProfile();
	GenProfile(const GenProfile& inOriginal);
	explicit GenProfile(Demography::Handle inDemography,
						SCHNAPS::Core::PrimitiveTree::Handle inAcceptFunction,
						IndividualModel::Handle inIndividualModel,
						SimulationVariables::Handle inSimulationVariables);
	virtual ~GenProfile() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("GenProfile");
			return lName;
		schnaps_StackTraceEndM("const std::string& GenProfile::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	GenProfile::Handle deepCopy(const SCHNAPS::Core::System& ioSystem) const;

	const Demography& getDemography() const {
		schnaps_NonNullPointerAssertM(mDemography);
		return *mDemography;
	}

	const SCHNAPS::Core::PrimitiveTree& getAcceptFunction() const {
		schnaps_NonNullPointerAssertM(mAcceptFunction);
		return *mAcceptFunction;
	}

	const IndividualModel& getIndividualModel() const {
		schnaps_NonNullPointerAssertM(mIndividualModel);
		return *mIndividualModel;
	}

	const SimulationVariables& getSimulationVariables() const {
		schnaps_NonNullPointerAssertM(mSimulationVariables);
		return *mSimulationVariables;
	}

private:
	std::string mLabel;
	Demography::Handle mDemography;
	SCHNAPS::Core::PrimitiveTree::Handle mAcceptFunction;
	IndividualModel::Handle mIndividualModel;
	SimulationVariables::Handle mSimulationVariables;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenProfile_hpp */
