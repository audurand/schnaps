/*
 * GenerationContext.hpp
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

#ifndef SCHNAPS_Simulation_GenerationContext_hpp
#define SCHNAPS_Simulation_GenerationContext_hpp

#include "SCHNAPS/Core/Atom.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"
#include "SCHNAPS/Simulation/ExecutionContext.hpp"
#include "SCHNAPS/Simulation/GenProfile.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class GenerationContext SCHNAPS/Simulation/GenerationContext.hpp "SCHNAPS/Simulation/GenerationContext.hpp"
 *  \brief The context for the execution of a primitive tree by generator.
 */
class GenerationContext: public ExecutionContext {
public:
	//! GenerationContext allocator type.
	typedef Core::AllocatorT<GenerationContext, ExecutionContext::Alloc> Alloc;
	//! GenerationContext handle type.
	typedef Core::PointerT<GenerationContext, ExecutionContext::Handle> Handle;
	//! GenerationContext bag type.
	typedef Core::ContainerT<GenerationContext, ExecutionContext::Bag> Bag;

	GenerationContext();
	GenerationContext(const GenerationContext& inOriginal);
	explicit GenerationContext(Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~GenerationContext() {}

	//! Copy operator.
	GenerationContext& operator=(const GenerationContext& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("GenrationContext");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::GenerationContext::getName() const");
	}

	//! Return a handle to a deep copy of the generation context.
	GenerationContext::Handle deepCopy() const;

	/*!
	 * \brief Set the profile from which individuals are created.
	 * \param inGenProfile A handle to the profile from which individuals are created.
	 */
	void setGenProfile(GenProfile::Handle inGenProfile) {
		mGenProfile = inGenProfile;
	}

	/*!
	 * \brief  Return a const reference to the profile from which individuals are created.
	 * \return A const reference to the profile from which individuals are created.
	 * \throw  SCHNAPS::Core::AssertException if thr current profile is NULL.
	 */
	const GenProfile& getGenProfile() const {
		schnaps_NonNullPointerAssertM(mGenProfile);
		return *mGenProfile;
	}

protected:
	// data structures
	GenProfile::Handle mGenProfile;	//!< A handle to the profile from which individuals are created.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenerationContext_hpp */
