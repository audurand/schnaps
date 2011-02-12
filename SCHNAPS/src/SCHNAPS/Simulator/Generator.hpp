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
#include "SCHNAPS/Simulator/Clock.hpp"
#include "SCHNAPS/Simulator/Environment.hpp"
#include "SCHNAPS/Simulator/GenProfile.hpp"
#include "SCHNAPS/Simulator/Population.hpp"
#include "SCHNAPS/Simulator/GenerationThread.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

namespace simulator {

/*!
 *  \class Simulator Simulator/Generator/include/Generator.hpp "Simulator/Generator/include/Generator.hpp"
 *  \brief Generator class.
 */
class Generator: public core::Object {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, GenProfile, core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, GenProfile::Handle, core::HashString> ProfileMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, GenProfile::Handle, core::HashString> ProfileMap;
#else // No hash_map found
	typedef std::map<std::string, GenProfile::Handle> ProfileMap;
#endif

public:
	//! Generator allocator type.
	typedef core::AllocatorT<Generator, core::Object::Alloc> Alloc;
	//! Generator handle type.
	typedef core::PointerT<Generator, core::Object::Handle> Handle;
	//! Generator bag type.
	typedef core::ContainerT<Generator, core::Object::Bag> Bag;

	Generator();
	Generator(const Generator& inOriginal);
	explicit Generator(core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~Generator();

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Generator");
			return lName;
		schnaps_StackTraceEndM("const std::string& simulator::Generator::getName() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Individual::Bag::Handle generate(std::string inProfile, unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex);
	static void buildIndividuals(GenerationThread::Handle inThread);

	void clearRandomizer();
	void resetRandomizer();

	const core::System& getSystem() const {
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
	void init();

	// Sub reads
	void readRandomizerInfo(PACC::XML::ConstIterator inIter);
	void readProfiles(PACC::XML::ConstIterator inIter);

	// Sub writes
	void writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeProfiles(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

private:
	// System structures
	core::System::Handle mSystem;				//!< Handle to the system.

	// Reference structures
	Clock::Handle mClock;						//!< Handle to simulation clock.
	Environment::Handle mEnvironment;			//!< Handle to simulation environment.

	// Randomizers info
	core::ULongArray mRandomizerInitSeed;		//!< Init registered seed of random number generators (one per thread).
	core::StringArray mRandomizerInitState;		//!< Init state of random number generators (one per thread).
	core::ULongArray mRandomizerCurrentSeed;	//!< Current registered seed of random number generators (one per thread).
	core::StringArray mRandomizerCurrentState;	//!< Current state of random number generators (one per thread).

	GenerationContext::Bag mContext;

	// Multi-threads management structures
	GenerationThread::Bag mSubThreads;
	PACC::Threading::Condition* mParallel;
	PACC::Threading::Semaphore* mSequential;

	// Profiles
	ProfileMap mProfiles;						//!< Population profiles (maps profile name to generator profile).
};
} // end of simulator namespace

#endif /* simulator_Generator_hpp */
