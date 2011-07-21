/*
 * GenProfile.hpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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
 *  \brief Profile from which individuals are created.
 */
class GenProfile: public Core::Object {
public:
	//! GenProfile allocator type.
	typedef Core::AllocatorT<GenProfile, Core::Object::Alloc> Alloc;
	//! GenProfile handle type.
	typedef Core::PointerT<GenProfile, Core::Object::Handle> Handle;
	//! GenProfile bag type.
	typedef Core::ContainerT<GenProfile, Core::Object::Bag> Bag;

	GenProfile();
	GenProfile(const GenProfile& inOriginal);
	explicit GenProfile(Demography::Handle inDemography,
						Core::PrimitiveTree::Handle inAcceptFunction,
						IndividualModel::Handle inIndividualModel,
						SimulationVariables::Handle inSimulationVariables);
	virtual ~GenProfile() {}

	//! Return a handle to a deep copy of the object.
	virtual Core::Object::Handle deepCopy(const Core::System& inSystem) const;

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("GenProfile");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::GenProfile::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	/*!
	 * \brief  Return a const reference to the demography.
	 * \return A const reference to the demography.
	 * \throw SCHNAPS::Core::AssertException if the current demography is NULL.
	 */
	const Demography& getDemography() const {
		schnaps_NonNullPointerAssertM(mDemography);
		return *mDemography;
	}

	/*!
	 * \brief  Return a const reference to the accept function tree.
	 * \return A const reference to the accept function tree.
	 * \throw SCHNAPS::Core::AssertException if the current accept function tree is NULL.
	 */
	const Core::PrimitiveTree& getAcceptFunction() const {
		schnaps_NonNullPointerAssertM(mAcceptFunction);
		return *mAcceptFunction;
	}

	/*!
	 * \brief  Return a const reference to the individual model.
	 * \return A const reference to the individual model.
	 * \throw SCHNAPS::Core::AssertException if the current individual model is NULL.
	 */
	const IndividualModel& getIndividualModel() const {
		schnaps_NonNullPointerAssertM(mIndividualModel);
		return *mIndividualModel;
	}

	/*!
	 * \brief  Return a const reference to the simulation variables.
	 * \return A const reference to the simulation variables.
	 * \throw SCHNAPS::Core::AssertException if the current simulation variables is NULL.
	 */
	const SimulationVariables& getSimulationVariables() const {
		schnaps_NonNullPointerAssertM(mSimulationVariables);
		return *mSimulationVariables;
	}

private:
	std::string mLabel;									//!< The label of the generation profile.
	Demography::Handle mDemography;						//!< A handle to the demography.
	Core::PrimitiveTree::Handle mAcceptFunction;		//!< A handle to the accept function tree.
	IndividualModel::Handle mIndividualModel;			//!< A handle to the individual model.
	SimulationVariables::Handle mSimulationVariables;	//!< A handle to the simulation variables.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenProfile_hpp */
