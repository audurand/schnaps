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

#ifndef simulator_GenProfile_hpp
#define simulator_GenProfile_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"
#include "SCHNAPS/Simulator/Demography.hpp"
#include "SCHNAPS/Simulator/IndividualModel.hpp"
#include "SCHNAPS/Simulator/SimulationVariables.hpp"

namespace simulator {

/*!
 *  \class GenProfile Simulator/include/GenProfile.hpp "Simulator/include/GenProfile.hpp"
 *  \brief GenProfile class.
 */
class GenProfile: public core::Object {
public:
	//! GenProfile allocator type.
	typedef core::AllocatorT<GenProfile, core::Object::Alloc> Alloc;
	//! GenProfile handle type.
	typedef core::PointerT<GenProfile, core::Object::Handle> Handle;
	//! GenProfile bag type.
	typedef core::ContainerT<GenProfile, core::Object::Bag> Bag;

	GenProfile();
	GenProfile(const GenProfile& inOriginal);
	explicit GenProfile(Demography::Handle inDemography,
						core::PrimitiveTree::Handle inAcceptFunction,
						IndividualModel::Handle inIndividualModel,
						SimulationVariables::Handle inSimulationVariables);
	virtual ~GenProfile() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("GenProfile");
			return lName;
		schnaps_StackTraceEndM("const std::string& GenProfile::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	GenProfile::Handle deepCopy(const core::System& ioSystem) const;

	const Demography& getDemography() const {
		schnaps_NonNullPointerAssertM(mDemography);
		return *mDemography;
	}

	const core::PrimitiveTree& getAcceptFunction() const {
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
	core::PrimitiveTree::Handle mAcceptFunction;
	IndividualModel::Handle mIndividualModel;
	SimulationVariables::Handle mSimulationVariables;
};
} // end of simulator namespace

#endif /* simulator_GenProfile_hpp */
