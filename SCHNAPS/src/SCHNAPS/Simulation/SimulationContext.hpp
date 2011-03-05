/*
 * SimulationContext.hpp
 *
 *  Created on: 2009-02-19
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

#ifndef SCHNAPS_Simulation_SimulationContext_hpp
#define SCHNAPS_Simulation_SimulationContext_hpp

#include "SCHNAPS/Simulation/ExecutionContext.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/Individual.hpp"
#include "SCHNAPS/Simulation/Process.hpp"

#include <list>

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

namespace SCHNAPS {
namespace Simulation {

struct Scenario {
	Process::Handle mProcessEnvironment;
	Process::Handle mProcessIndividual;

	Scenario() :
		mProcessEnvironment(NULL),
		mProcessIndividual(NULL)
	{}
};

struct ClockObservers {
	Process::Bag mProcessEnvironment;
	Process::Bag mProcessIndividual;

	ClockObservers() {}
};

/*!
 *  \class SimulationContext SCHNAPS/Simulation/SimulationContext.hpp "SCHNAPS/Simulation/SimulationContext.hpp"
 *  \brief SimulationContext class, the context for the simulation.
 */
class SimulationContext: public ExecutionContext {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Process::Handle, SCHNAPS::Core::HashString> ProcessMap;
	typedef std::hash_map<std::string, Process::Bag::Handle, SCHNAPS::Core::HashString> ObserverMap;
	typedef std::hash_map<std::string, Scenario, SCHNAPS::Core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Process::Handle, SCHNAPS::Core::HashString> ProcessMap;
	typedef __gnu_cxx::hash_map<std::string, Process::Bag::Handle, SCHNAPS::Core::HashString> ObserverMap;
	typedef __gnu_cxx::hash_map<std::string, Scenario, SCHNAPS::Core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Process::Handle, SCHNAPS::Core::HashString> ProcessMap;
	typedef stdext::hash_map<std::string, Process::Bag::Handle, SCHNAPS::Core::HashString> ObserverMap;
	typedef stdext::hash_map<std::string, Scenario, SCHNAPS::Core::HashString> ScenarioMap;
#else // no hash_map found
	typedef std::map<std::string, Process::Handle> ProcessMap;
	typedef std::map<std::string, Process::Bag::Handle> ObserverMap;
	typedef std::map<std::string, Scenario> ScenarioMap;
#endif

public:
	//! SimulationContext allocator type.
	typedef SCHNAPS::Core::AllocatorT<SimulationContext, ExecutionContext::Alloc> Alloc;
	//! SimulationContext handle type.
	typedef SCHNAPS::Core::PointerT<SimulationContext, ExecutionContext::Handle> Handle;
	//! SimulationContext bag type.
	typedef SCHNAPS::Core::ContainerT<SimulationContext, ExecutionContext::Bag> Bag;

	SimulationContext();
	SimulationContext(const SimulationContext& inOriginal);
	explicit SimulationContext(const Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
	virtual ~SimulationContext() {}

	virtual const std::string& getName() const {
		return mName;
	}

	void readProcesses(PACC::XML::ConstIterator inIter);
	void readObservers(PACC::XML::ConstIterator inIter);
	void readScenarios(PACC::XML::ConstIterator inIter);
	void writeProcesses(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeObservers(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeScenarios(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	SimulationContext::Handle deepCopy() const;

	void reset() {
		mIndividual = NULL;
		mPushList.clear();
	}

	const ProcessMap& getProcesses() const {
		return mProcesses;
	}

	const ClockObservers& getClockObservers() const {
		return mClockObservers;
	}

	const ScenarioMap& getScenarios() const {
		return mScenarios;
	}
	
	const Scenario& getScenario(std::string inLabel) {
		schnaps_StackTraceBeginM();
		ScenarioMap::const_iterator lIt = mScenarios.find(inLabel);
#ifndef SCHNAPS_NDEBUG
		if (lIt == mScenarios.end()) {
			throw schnaps_InternalExceptionM("Could not find " + inLabel + " in scenario list of simulation context number" + uint2str(mThreadNb) + "!");
		}
#else
		schnaps_AssertM(lIt != mScenarios.end());
#endif
		return lIt->second;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::getScenario(std::string&)");
	}

	const std::list<Push>& getPushList() const {
		return mPushList;
	}

	std::list<Push>& getPushList() {
		return mPushList;
	}

private:
	// data structures
	ProcessMap mProcesses;			//!< Stored processes.
	ClockObservers mClockObservers;	//!< Clock observers.
	ScenarioMap mScenarios;			//!< Scenario processes (label to scenario).

	// current structures
	std::list<Push> mPushList;		//!< List of pushes made while executing current individual.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_SimulationContext_hpp */
