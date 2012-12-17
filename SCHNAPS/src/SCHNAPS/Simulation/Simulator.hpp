/*
 * Simulator.hpp
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

#ifndef SCHNAPS_Simulation_Simulator_hpp
#define SCHNAPS_Simulation_Simulator_hpp

#include "SCHNAPS/Core/System.hpp"
#include "SCHNAPS/Simulation/BlackBoard.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/Process.hpp"
#include "SCHNAPS/Simulation/SimulationContext.hpp"
#include "SCHNAPS/Simulation/WaitingQMaps.hpp"
#include "SCHNAPS/Simulation/SimulationThread.hpp"

#include <map>
#include <queue>
#include <vector>

namespace SCHNAPS {
namespace Simulation {

/*!
 * \struct OutputParameters SCHNAPS/Simulation/Simulator.hpp "SCHNAPS/Simulation/Simulator.hpp"
 * \brief  Output parameters of simulation.
 */
struct OutputParameters {
	std::vector<std::string> mEnvironment;							//!< Environment variables to output.
	std::map<std::string, std::vector<std::string> > mPopulation;	//!< Individuals variables to output by profile.

	OutputParameters() {}
	virtual ~OutputParameters() {}
};

/*!
 *  \class Simulator SCHNAPS/Simulation/Simulator.hpp "SCHNAPS/Simulation/Simulator.hpp"
 *  \brief SCHNAPS Simulator.
 */
class Simulator: public Core::Object {
public:
	//! Simulator allocator type.
	typedef Core::AllocatorT<Simulator, Core::Object::Alloc> Alloc;
	//! Simulator handle type.
	typedef Core::PointerT<Simulator, Core::Object::Handle> Handle;
	//! Simulator bag type.
	typedef Core::ContainerT<Simulator, Core::Object::Bag> Bag;

	typedef std::map<unsigned int, std::queue<Process::Handle> > WaitingQMap;

	Simulator();
	virtual ~Simulator();

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Simulator");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::Simulator::getName() const");
	}

	//! Read object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Configure the simulator using specific parameters.
	void configure(const std::string& inParameters);
	//! Return the current simulator configuration.
	std::string getConfiguration();

	//! Execute the simulation of scpecific scenario.
	void simulate(const std::string& inScenarioLabel);
	
	//! Refresh simulator structure with up-to-date parameters.
	void refresh();
	//! Clear the randomizer values for seeds and states.
	void clearRandomizer();
	//! Reset the randomizer values for seeds and states to initial values.
	void resetRandomizer();

	//! Return a const pointer to the system.
	const Core::System::Handle getSystemHandle() const {
		return mSystem;
	}

	//! Return a const reference to the system.
	const Core::System& getSystem() const {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	//! Return a system reference to the system.
	Core::System& getSystem() {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	//! Return a const pointer to the clock.
	const Clock::Handle getClockHandle() const {
		return mClock;
	}

	//! Return a const reference to the clock.
	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

	//! Return a const pointer to the environment.
	const Environment::Handle getEnvironmentHandle() const {
		return mEnvironment;
	}

	//! Return a const referencce to the environment.
	const Environment& getEnvironment() const {
		schnaps_NonNullPointerAssertM(mEnvironment);
		return *mEnvironment;
	}

	//! Return a const pointer to the population manager.
	const PopulationManager::Handle getPopulationManagerHandle() const {
		return mPopulationManager;
	}

	//! Return a const reference to the population manager.
	const PopulationManager& getPopulationManager() const {
		schnaps_NonNullPointerAssertM(mPopulationManager);
		return *mPopulationManager;
	}

	//! Return a reference to the population manager.
	PopulationManager& getPopulationManager() {
		schnaps_NonNullPointerAssertM(mPopulationManager);
		return *mPopulationManager;
	}

	//! Scenario processing by a specific thread.
	static void processScenario(SimulationThread::Handle inThread);
	//! Clock step processing by a specific thread.
	static void processClockStep(SimulationThread::Handle inThread);
	//! Sub clock step processing by a specific thread.
	static void processSubStep(SimulationThread::Handle inThread);

private:
	// sub reads
	//! Read input section of configuration file.
	void readInput(PACC::XML::ConstIterator inIter);
	//! Read simulation section of configuration file.
	void readSimulation(PACC::XML::ConstIterator inIter);
	//! Read output section of configuration file.
	void readOutput(PACC::XML::ConstIterator inIter);

	//! Read randomizer information in simulation section of configuration file.
	void readRandomizerInfo(PACC::XML::ConstIterator inIter);
	//! Read environment information in output section of configuration file.
	void readEnvironmentOutput(PACC::XML::ConstIterator inIter);
	//! Read population information in output section of configuration file.
	void readPopulationOutput(PACC::XML::ConstIterator inIter);

	// sub writes
	//! Write input section to configuration file.
	void writeInput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write simulation section to configuration file.
	void writeSimulation(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write output section to configuration file.
	void writeOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Write randomizer information in simulation section to configuration file.
	void writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write environemtn information in output section to configuration file.
	void writeEnvironmentOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	//! Write population information in output section to configuration file.
	void writePopulationOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	// prints
	//! Print the current state of the environment.
	void printEnvironment(std::ostream& ioStream) const;
	//! Print the current state of individuals in population, between specific indexes.
	void printIndividuals(std::ostream& ioStream, unsigned int inLowerIndex, unsigned int inUpperIndex) const;
	//! Print the summary that describes the output variables and order.
	void printSummary(std::ostream& ioStream) const;

private:
	// system structures
	Core::System::Handle mSystem;				 	//!< Handle to system.
	Clock::Handle mClock;							//!< Handle to clock used for simulation.

	// data structures
	Environment::Handle mEnvironment;				//!< Handle to environment.
	PopulationManager::Handle mPopulationManager;	//!< Handle to population manager.

	BlackBoard::Handle mBlackBoard;					//!< Handle to blackboard for push tracking.
	WaitingQMaps::Handle mWaitingQMaps;				//!< Handle to environment and individuals waiting queues.

	Core::ULongArray mRandomizerInitSeed;			//!< Init registered seed of random number generators (one per thread).
	Core::StringArray mRandomizerInitState;			//!< Init state of random number generators (one per thread).
	Core::ULongArray mRandomizerCurrentSeed;		//!< Current registered seed of random number generators (one per thread).
	Core::StringArray mRandomizerCurrentState;		//!< Current state of random number generators (one per thread).

	SimulationContext::Bag mContext;				//!< All simulation contexts (1 per thread).

	// multi-threads management structures
	SimulationThread::Bag mSubThreads;				//!< Subthreads used when simulating.
	PACC::Threading::Condition* mParallel;			//!< Thread condition for triggering parallel execution.
	PACC::Threading::Semaphore* mSequential;		//!< Thread semaphore for triggering sequential execution.
	PACC::Threading::Semaphore* mBlackBoardWrt;		//!< Thread semaphore for modifying blackboard.

	OutputParameters mOutputParameters;				//!< Output parameters.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Simulator_hpp */
