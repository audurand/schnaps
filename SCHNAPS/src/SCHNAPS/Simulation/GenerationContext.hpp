/*
 * GenerationContext.hpp
 *
 *  Created on: 2010-01-16
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
 *  \brief GenerationContext class, the context for the execution of a primitive tree by generator.
 */
class GenerationContext: public ExecutionContext {
public:
	//! GenerationContext allocator type.
	typedef SCHNAPS::Core::AllocatorT<GenerationContext, ExecutionContext::Alloc> Alloc;
	//! GenerationContext handle type.
	typedef SCHNAPS::Core::PointerT<GenerationContext, ExecutionContext::Handle> Handle;
	//! GenerationContext bag type.
	typedef SCHNAPS::Core::ContainerT<GenerationContext, ExecutionContext::Bag> Bag;

	GenerationContext();
	GenerationContext(const GenerationContext& inOriginal);
	explicit GenerationContext(SCHNAPS::Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~GenerationContext() {}

	virtual const std::string& getName() const {
		return mName;
	}

	GenerationContext& operator=(const GenerationContext& inOriginal);

	GenerationContext::Handle deepCopy() const;

	void setGenProfile(GenProfile::Handle inGenProfile) {
		mGenProfile = inGenProfile;
	}

	const GenProfile& getGenProfile() const {
		schnaps_NonNullPointerAssertM(mGenProfile);
		return *mGenProfile;
	}

protected:
	// Data structures
	GenProfile::Handle mGenProfile;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenerationContext_hpp */
