/*
 * SimulationContext.hpp
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

/*!
 * \struct Scenario SCHNAPS/Simulation/SimulationContext.hpp "SCHNAPS/Simulation/SimulationContext.hpp"
 * \brief  Simulation scenario made of a process for the environment and a process for the individuals.
 */
struct Scenario {
	Process::Handle mProcessEnvironment;	//!< A handle to a process for the environment.
	Process::Handle mProcessIndividual;		//!< A handle to a process for the individuals.

	Scenario() :
		mProcessEnvironment(NULL),
		mProcessIndividual(NULL)
	{}
};

/*!
 * \struct ClockObservers SCHNAPS/Simulation/SimulationContext.hpp "SCHNAPS/Simulation/SimulationContext.hpp"
 * \brief  Processes automaticaly triggered on clock steps.
 */
struct ClockObservers {
	Process::Bag mProcessEnvironment;	//!< A bag of processes for the environment.
	Process::Bag mProcessIndividual;	//!< A bag of processes for the individuals.

	ClockObservers() {}
};

/*!
 *  \class SimulationContext SCHNAPS/Simulation/SimulationContext.hpp "SCHNAPS/Simulation/SimulationContext.hpp"
 *  \brief SimulationContext class, the context for the simulation.
 */
class SimulationContext: public ExecutionContext {
protected:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Process::Handle, Core::HashString> ProcessMap;
	typedef std::hash_map<std::string, Scenario, Core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Process::Handle, Core::HashString> ProcessMap;
	typedef __gnu_cxx::hash_map<std::string, Scenario, Core::HashString> ScenarioMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Process::Handle, Core::HashString> ProcessMap;
	typedef stdext::hash_map<std::string, Scenario, Core::HashString> ScenarioMap;
#else // no hash_map found
	typedef std::map<std::string, Process::Handle> ProcessMap;
	typedef std::map<std::string, Scenario> ScenarioMap;
#endif

public:

	//! SimulationContext allocator type.
	typedef Core::AllocatorT<SimulationContext, ExecutionContext::Alloc> Alloc;
	//! SimulationContext handle type.
	typedef Core::PointerT<SimulationContext, ExecutionContext::Handle> Handle;
	//! SimulationContext bag type.
	typedef Core::ContainerT<SimulationContext, ExecutionContext::Bag> Bag;

	SimulationContext();
	SimulationContext(const SimulationContext& inOriginal);
	explicit SimulationContext(const Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
	virtual ~SimulationContext() {}
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("SimulationContext");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::SimulationContext::getName() const");
	}

	//! Read processes from XML.
	void readProcesses(PACC::XML::ConstIterator inIter);
	void readObservers(PACC::XML::ConstIterator inIter);
	void readScenarios(PACC::XML::ConstIterator inIter);
	void writeProcesses(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeObservers(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeScenarios(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	SimulationContext::Handle deepCopy() const;

	/*!
	 * \brief Reset to a null individual and clears the list of push processes.
	 */
	void reset() {
		schnaps_StackTraceBeginM();
		mIndividual = NULL;
		mPushList.clear();
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::reset()");
	}

	/*!
	 * \brief  Return a const reference to all processes.
	 * \return A const reference to all processes.
	 */
	const ProcessMap& getProcesses() const {
		schnaps_StackTraceBeginM();
		return mProcesses;
		schnaps_StackTraceEndM("const SCHNAPS::Simulation::SimulationContext::ProcessMap& SCHNAPS::Simulation::SimulationContext::getProcesses() const");
	}
	
	/*!
	 * \brief  Return a const handle to a specific process.
	 * \param  inLabel A const reference to the process label.
	 * \return A const handle to a specific process.
	 * \throw  SCHNAPS::Core::RunTimeException if the process does not exist.
	 */
	const Process::Handle getProcessHandle(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		ProcessMap::const_iterator lIterProcess = mProcesses.find(inLabel);
		if (lIterProcess == mProcesses.end()) {
			std::ostringstream lOSS;
			lOSS << "The process '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		return lIterProcess->second;
		schnaps_StackTraceEndM("const SCHNAPS::Simulation::Process::Handle SCHNAPS::Simulation::SimulationContext::getProcessHandle(const std::string&) const");
	}

	/*!
	 * \brief  Return a const reference to all clock observers.
	 * \return A const reference to all clock observers.
	 */
	const ClockObservers& getClockObservers() const {
		schnaps_StackTraceBeginM();
		return mClockObservers;
		schnaps_StackTraceEndM("const SCHNAPS::Simulation::SimulationContext::ClockObservers& SCHNAPS::Simulation::SimulationContext::getClockObservers() const");
	}

	/*!
	 * \brief  Return a const reference to all scenarios.
	 * \return A const reference to all scenarios.
	 */
	const ScenarioMap& getScenarios() const {
		schnaps_StackTraceBeginM();
		return mScenarios;
		schnaps_StackTraceEndM("const SCHNAPS::Simulation::SimulationContext::ScenarioMap& SCHNAPS::Simulation::SimulationContext::getScenarios() const");
	}
	
	/*!
	 * \brief  Return a const reference to a specific scenario.
	 * \param  inLabel A const reference to the scenario label.
	 * \return A const reference to a specific scenario.
	 * \throw  SCHNAPS::Core::RunTimeException if the scenario does not exist.
	 */
	const Scenario& getScenario(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		ScenarioMap::const_iterator lIterScenario = mScenarios.find(inLabel);
		if (lIterScenario == mScenarios.end()) {
			std::ostringstream lOSS;
			lOSS << "The scenario '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		return lIterScenario->second;
		schnaps_StackTraceEndM("const SCHNAPS::Simulation::Scenario& SCHNAPS::Simulation::SimulationContext::getScenario(const std::string&) const");
	}

	/*!
	 * \brief  Return a const reference to the list of push processes.
	 * \return A const reference to the list of push processes.
	 */
	const std::list<Push>& getPushList() const {
		schnaps_StackTraceBeginM();
		return mPushList;
		schnaps_StackTraceEndM("const std::list<Push>& SCHNAPS::Simulation::SimulationContext::getPushList() const");
	}

	/*!
	 * \brief  Return a reference to the list of push processes.
	 * \return A reference to the list of push processes.
	 */
	std::list<Push>& getPushList() {
		schnaps_StackTraceBeginM();
		return mPushList;
		schnaps_StackTraceEndM("std::list<Push>& SCHNAPS::Simulation::SimulationContext::getPushList()");
	}

private:
	// data structures
	ProcessMap mProcesses;				//!< Stored processes.
	ClockObservers mClockObservers;		//!< Clock observers.
	ScenarioMap mScenarios;				//!< Scenario processes (label to scenario).

	// current structures
	std::list<Push> mPushList;			//!< List of pushes made while executing current individual.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_SimulationContext_hpp */
