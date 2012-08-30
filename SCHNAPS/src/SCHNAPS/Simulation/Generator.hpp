/*
 * Generator.hpp
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

#ifndef generator_Generator_hpp
#define generator_Generator_hpp

#include "SCHNAPS/Core/HashString.hpp"
#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/System.hpp"
#include "SCHNAPS/Core/ULong.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/GenProfile.hpp"
#include "SCHNAPS/Simulation/Population.hpp"
#include "SCHNAPS/Simulation/GenerationThread.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

#define NBCONTACTS_VARIABLE "ref.nbContacts_"
#define CONTACTS_FLAG "ref.Contacts"
#define CONTACTS_LIST_VARIABLE "liste_contacts"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Generator SCHNAPS/Simulation/Generator.hpp "SCHNAPS/Simulation/Generator.hpp"
 *  \brief Individual generator.
 */
class Generator: public Core::Object {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, GenProfile, Core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, GenProfile::Handle, Core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, GenProfile::Handle, Core::HashString> ProfileMap;
#else // no hash_map found
	typedef std::map<std::string, GenProfile::Handle> ProfileMap;
#endif

public:
	//! Generator allocator type.
	typedef Core::AllocatorT<Generator, Core::Object::Alloc> Alloc;
	//! Generator handle type.
	typedef Core::PointerT<Generator, Core::Object::Handle> Handle;
	//! Generator bag type.
	typedef Core::ContainerT<Generator, Core::Object::Bag> Bag;

	Generator();
	Generator(const Generator& inOriginal);
	explicit Generator(Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~Generator();

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Generator");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Generator::getName() const");
	}

	//! Read object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Generate a quantity of individuals from a specific profile using specific prefix and start index.
	Individual::Bag::Handle generate(const std::string& inProfile, unsigned int inSize, const std::string& inPrefix, unsigned int inStartingIndex);
	
	//! Build individuals using specific thread.
	static void buildIndividuals(GenerationThread::Handle inThread);
	
	//! Optionally generate contact lists for all individuals.
	void generateContacts(Individual::Bag::Handle inPop);

	//! Refresh generator structure with up-to-date parameters.
	void refresh();
	
	//! Clear randomizer (seed to zero, empty state).
	void clearRandomizer();
	//! Reset randomizer to intial seed and state.
	void resetRandomizer();

	/*!
	 * \brief  Return a const reference to the system.
	 * \return A const reference to the system.
	 * \throw  SCHNAPS::Core::AssertException if system is NULL.
	 */
	const Core::System& getSystem() const {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	/*!
	 * \brief  Return a const reference to the clock.
	 * \return A const reference to the clock.
	 * \throw  SCHNAPS::Core::AssertException if clock is NULL.
	 */
	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

	/*!
	 * \brief  Return a const reference to the environment.
	 * \return A const reference to the environment.
	 * \throw  SCHNAPS::Core::AssertException if environment is NULL.
	 */
	const Environment& getEnvironment() const {
		schnaps_NonNullPointerAssertM(mEnvironment);
		return *mEnvironment;
	}

private:
	// sub reads
	void readRandomizerInfo(PACC::XML::ConstIterator inIter);
	void readProfiles(PACC::XML::ConstIterator inIter);

	// sub writes
	void writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeProfiles(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	

private:
	// system structures
	Core::System::Handle mSystem;				//!< Handle to the system.

	// reference structures
	Clock::Handle mClock;						//!< Handle to simulation clock.
	Environment::Handle mEnvironment;			//!< Handle to simulation environment.

	// randomizers info
	Core::ULongArray mRandomizerInitSeed;		//!< Init registered seed of random number generators (one per thread).
	Core::StringArray mRandomizerInitState;		//!< Init state of random number generators (one per thread).
	Core::ULongArray mRandomizerCurrentSeed;	//!< Current registered seed of random number generators (one per thread).
	Core::StringArray mRandomizerCurrentState;	//!< Current state of random number generators (one per thread).

	GenerationContext::Bag mContext;

	// multi-threads management structures
	GenerationThread::Bag mSubThreads;
	PACC::Threading::Condition* mParallel;
	PACC::Threading::Semaphore* mSequential;

	// profiles
	ProfileMap mProfiles;						//!< Population profiles (maps profile name to generator profile).
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Generator_hpp */
