/*
 * SimulationContext.hpp
 *
 *  Created on: 2009-02-19
 *  Updated on: 2010-10-18
 *      Author: Audrey Durand
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

#ifndef simulator_SimulationContext_hpp
#define simulator_SimulationContext_hpp

#include <list>

#include "SCHNAPS/Simulator/ExecutionContext.hpp"
#include "SCHNAPS/Simulator/Clock.hpp"
#include "SCHNAPS/Simulator/Environment.hpp"
#include "SCHNAPS/Simulator/Individual.hpp"
#include "SCHNAPS/Simulator/Process.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#else
#include <map>
#endif

namespace simulator {

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
 *  \class SimulationContext Simulator/include/SimulationContext.hpp "Simulator/include/SimulationContext.hpp"
 *  \brief SimulationContext class, the context for the simulation.
 */
class SimulationContext: public ExecutionContext {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Process::Handle, core::HashString> ProcessMap;
	typedef std::hash_map<std::string, Process::Bag::Handle, core::HashString> ObserverMap;
	typedef std::hash_map<std::string, Scenario, core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Process::Handle, core::HashString> ProcessMap;
	typedef __gnu_cxx::hash_map<std::string, Process::Bag::Handle, core::HashString> ObserverMap;
	typedef __gnu_cxx::hash_map<std::string, Scenario, core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Process::Handle, core::HashString> ProcessMap;
	typedef stdext::hash_map<std::string, Process::Bag::Handle, core::HashString> ObserverMap;
	typedef stdext::hash_map<std::string, Scenario, core::HashString> ScenarioMap;
#else // No hash_map found
	typedef std::map<std::string, Process::Handle> ProcessMap;
	typedef std::map<std::string, Process::Bag::Handle> ObserverMap;
	typedef std::map<std::string, Scenario> ScenarioMap;
#endif

public:
	//! SimulationContext allocator type.
	typedef core::AllocatorT<SimulationContext, ExecutionContext::Alloc> Alloc;
	//! SimulationContext handle type.
	typedef core::PointerT<SimulationContext, ExecutionContext::Handle> Handle;
	//! SimulationContext bag type.
	typedef core::ContainerT<SimulationContext, ExecutionContext::Bag> Bag;

	SimulationContext();
	SimulationContext(const SimulationContext& inOriginal);
	explicit SimulationContext(const core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
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

	const std::list<Push>& getPushList() const {
		return mPushList;
	}

	std::list<Push>& getPushList() {
		return mPushList;
	}

private:
	// Data structures
	ProcessMap mProcesses;						//!< Stored processes.
	ClockObservers mClockObservers;				//!< Clock observers.
	ScenarioMap mScenarios;						//!< Scenario processes (label to scenario).

	// Current structures
	std::list<Push> mPushList;					//!< List of pushes made while executing current individual.
};
} // end of simulator namespace

#endif /* simulator_SimulationContext_hpp */
