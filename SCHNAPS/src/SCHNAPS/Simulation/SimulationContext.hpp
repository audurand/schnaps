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
#include <tr1/unordered_map>
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
 * \struct Observer SCHNAPS/Simulation/SimulationContext.hpp "SCHNAPS/Simulation/SimulationContext.hpp"
 * \brief  Process automaticaly triggered on specific clock ticks.
 */
struct Observer {
	Process::Handle mProcess;	//!< A handle to the observing process. 
	unsigned long mStart;		//!< The first occurence (in observer units).
	unsigned long mEnd;			//!< The last occurence (in observer untis).
	unsigned long mStep;		//!< The step between each occurence (in observer units).
	Clock::Units mUnits;		//!< Observer units.

	Observer(Process::Handle inProcess = NULL,
					unsigned long inStart = 0,
					unsigned long inEnd = 0,
					unsigned long inStep = 1,
					Clock::Units inUnits = Clock::eOther) :
					mProcess(inProcess),
					mStart(inStart),
					mEnd(inEnd),
					mStep(inStep),
					mUnits(inUnits)
	{}
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
	typedef std::hash_map<std::string, Observer, Core::HashString> ObserverMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef std::tr1::unordered_map<std::string, Process::Handle, Core::HashString> ProcessMap;
	typedef std::tr1::unordered_map<std::string, Scenario, Core::HashString> ScenarioMap;
	typedef std::tr1::unordered_map<std::string, Observer, Core::HashString> ObserverMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Process::Handle, Core::HashString> ProcessMap;
	typedef stdext::hash_map<std::string, Scenario, Core::HashString> ScenarioMap;
	typedef stdext::hash_map<std::string, Observer, Core::HashString> ObserverMap;
#else // no hash_map found
	typedef std::map<std::string, Process::Handle> ProcessMap; //!< Process label to process.
	typedef std::map<std::string, Scenario> ScenarioMap; //!< Scenario label to scenario.
	typedef std::map<std::string, Observer> ObserverMap; //!< Process label to observer that contains the process.
#endif
	typedef std::multimap<unsigned long, std::string> ExecutionMap;	//!< Clock tick of the next occurence to process label.

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
	//! Read clock observers from XML.
	void readObservers(PACC::XML::ConstIterator inIter);
	//! Read scenarios from XML.
	void readScenarios(PACC::XML::ConstIterator inIter);
	//! Write processes to XML.
	void writeProcesses(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write clock observers to XML.
	void writeObservers(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write scenarios to XML.
	void writeScenarios(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	SimulationContext::Handle deepCopy() const;

	/*!
	 * \brief Reset to a null individual and clears the list of push processes.
	 */
	void resetIndividual() {
		schnaps_StackTraceBeginM();
		mIndividual = NULL;
		mPushList.clear();
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::resetIndividual()");
	}

	/*!
	 * \brief Reset to a null individual and clears the list of push processes.
	 * \throw SCHNAPS::Core::AssertException if the simulation clock has not been defined.
	 */
	void resetObserversNextExecution() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mClock);
		// target environment
		mCurrentObserversForEnvironment.clear();
		mObserversForEnvironmentExecution.clear();
		for (ObserverMap::iterator lIt_i = mObserversForEnvironment.begin(); lIt_i != mObserversForEnvironment.end(); lIt_i++) {
			mObserversForEnvironmentExecution.insert(std::pair<unsigned long, std::string>(
				mClock->getTick(lIt_i->second.mStart, lIt_i->second.mUnits),
				lIt_i->first));
		}
		
		// target all individuals
		mCurrentObserversForIndividuals.clear();
		mObserversForIndividualsExecution.clear();
		for (ObserverMap::iterator lIt_i = mObserversForIndividuals.begin(); lIt_i != mObserversForIndividuals.end(); lIt_i++) {
			mObserversForIndividualsExecution.insert(std::pair<unsigned long, std::string>(
				mClock->getTick(lIt_i->second.mStart, lIt_i->second.mUnits),
				lIt_i->first));
		}
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::resetObserversNextExecution()");
	}

	/*!
	 * \brief Update the clock observers at the current time step.
	 * \throw SCHNAPS::Core::AssertException if the simulation clock has not been defined.
	 */
	void updateCurrentObservers() {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mClock);
		std::pair<ExecutionMap::iterator, ExecutionMap::iterator> lRange;
		ObserverMap::iterator lIt_Observer;
		std::vector<std::pair<unsigned long, std::string> > lUpdateExecution;
		
		// target environment
		lUpdateExecution.clear();
		mCurrentObserversForEnvironment.clear();
		// get observers with next occurence equal to current clock value
		lRange = mObserversForEnvironmentExecution.equal_range(mClock->getValue());
		for (ExecutionMap::iterator lIt_i = lRange.first; lIt_i != lRange.second; lIt_i++) {
			lIt_Observer = mObserversForEnvironment.find(lIt_i->second);
			mCurrentObserversForEnvironment.push_back(lIt_Observer->second.mProcess);
			
			// compute next occurence of the process
			if (lIt_Observer->second.mEnd > 0) {
				// if there is a end condition
				if (mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep <= lIt_Observer->second.mEnd) {
					lUpdateExecution.push_back(std::pair<unsigned long, std::string>(
						mClock->getTick(mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep, lIt_Observer->second.mUnits),
						lIt_i->second));
				}
			} else {
				// if there is no end condition
				lUpdateExecution.push_back(std::pair<unsigned long, std::string>(
					mClock->getTick(mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep, lIt_Observer->second.mUnits),
					lIt_i->second));
			}
		}
		// update next occurence of the processes
		mObserversForEnvironmentExecution.erase(mClock->getValue());
		for (unsigned int i = 0; i < lUpdateExecution.size(); i++) {
			mObserversForEnvironmentExecution.insert(lUpdateExecution[i]);
		}
		
		
		// target all individuals
		lUpdateExecution.clear();
		mCurrentObserversForIndividuals.clear();
		// get observers with next occurence equal to current clock value
		lRange = mObserversForIndividualsExecution.equal_range(mClock->getValue());
		for (ExecutionMap::iterator lIt_i = lRange.first; lIt_i != lRange.second; lIt_i++) {
			lIt_Observer = mObserversForIndividuals.find(lIt_i->second);
			mCurrentObserversForIndividuals.push_back(lIt_Observer->second.mProcess);
			
			// compute next occurence of the process
			if (lIt_Observer->second.mEnd > 0) {
				// if there is a end condition
				if (mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep <= lIt_Observer->second.mEnd) {
					lUpdateExecution.push_back(std::pair<unsigned long, std::string>(
						mClock->getTick(mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep, lIt_Observer->second.mUnits),
						lIt_i->second));
				}
			} else {
				// if there is no end condition
				lUpdateExecution.push_back(std::pair<unsigned long, std::string>(
					mClock->getTick(mClock->getValue(lIt_Observer->second.mUnits) + lIt_Observer->second.mStep, lIt_Observer->second.mUnits),
					lIt_i->second));
			}
		}
		// update next occurence of the processes
		mObserversForIndividualsExecution.erase(mClock->getValue());
		for (unsigned int i = 0; i < lUpdateExecution.size(); i++) {
			mObserversForIndividualsExecution.insert(lUpdateExecution[i]);
		}
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::updateCurrentObserversForEnvironment()");
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
	 * \brief  Return a const reference to the clock observers that target the environment at the current time step.
	 * \return A const reference to clock observers that target the environment at the current time step.
	 */
	const Process::Bag& getCurrentObserversForEnvironment() const {
		schnaps_StackTraceBeginM();
		return mCurrentObserversForEnvironment;
		schnaps_StackTraceEndM("const Process::Bag& SCHNAPS::Simulation::SimulationContext::getCurrentObserversForEnvironment() const");
	}

	/*!
	 * \brief  Return a const reference to the clock observers that target all individuals at the current time step.
	 * \return A const reference to clock observers that target all individuals at the current time step.
	 */
	const Process::Bag& getCurrentObserversForIndividuals() const {
		schnaps_StackTraceBeginM();
		return mCurrentObserversForIndividuals;
		schnaps_StackTraceEndM("const Process::Bag& SCHNAPS::Simulation::SimulationContext::getCurrentObserversForIndividuals() const");
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
	ProcessMap mProcesses;							//!< Stored processes.
	ScenarioMap mScenarios;							//!< Scenario processes (label to scenario).
	ObserverMap mObserversForEnvironment;			//!< Clock observers that target the environment (next occurence in clock units to observer).
	ObserverMap mObserversForIndividuals;			//!< Clock observers that target all individuals (next occurence in clock units to observer).
	
	ExecutionMap mObserversForEnvironmentExecution;	
	ExecutionMap mObserversForIndividualsExecution;
	Process::Bag mCurrentObserversForEnvironment;	//!< Clock observers that target the environment to trigger at current time step.
	Process::Bag mCurrentObserversForIndividuals;	//!< Clock observers that target all individuals to trigger at current time step.

	// current structures
	std::list<Push> mPushList;						//!< List of pushes made while executing current individual.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_SimulationContext_hpp */
