/*
 * Generator.hpp
 *
 *  Created on: 2009-03-12
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

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Simulator SCHNAPS/Simulation/Generator.hpp "SCHNAPS/Simulation/Generator.hpp"
 *  \brief Generator class.
 */
class Generator: public SCHNAPS::Core::Object {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, GenProfile, SCHNAPS::Core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, GenProfile::Handle, SCHNAPS::Core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, GenProfile::Handle, SCHNAPS::Core::HashString> ProfileMap;
#else // no hash_map found
	typedef std::map<std::string, GenProfile::Handle> ProfileMap;
#endif

public:
	//! Generator allocator type.
	typedef SCHNAPS::Core::AllocatorT<Generator, SCHNAPS::Core::Object::Alloc> Alloc;
	//! Generator handle type.
	typedef SCHNAPS::Core::PointerT<Generator, SCHNAPS::Core::Object::Handle> Handle;
	//! Generator bag type.
	typedef SCHNAPS::Core::ContainerT<Generator, SCHNAPS::Core::Object::Bag> Bag;

	Generator();
	Generator(const Generator& inOriginal);
	explicit Generator(SCHNAPS::Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~Generator();

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Generator");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Generator::getName() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Individual::Bag::Handle generate(std::string inProfile, unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex);
	static void buildIndividuals(GenerationThread::Handle inThread);

	//! Refresh generator structure with up-to-date parameters.
	void refresh();
	
	void clearRandomizer();
	void resetRandomizer();

	const Core::System& getSystem() const {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

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
