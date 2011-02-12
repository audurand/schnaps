/*
 * Simulator.hpp
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

#ifndef simulator_Simulator_hpp
#define simulator_Simulator_hpp

#include <map>
#include <queue>
#include <vector>

#include "SCHNAPS/Core/System.hpp"
#include "SCHNAPS/Simulator/BlackBoard.hpp"
#include "SCHNAPS/Simulator/Clock.hpp"
#include "SCHNAPS/Simulator/Environment.hpp"
#include "SCHNAPS/Simulator/Process.hpp"
#include "SCHNAPS/Simulator/SimulationContext.hpp"
#include "SCHNAPS/Simulator/WaitingQMaps.hpp"
#include "SCHNAPS/Simulator/SimulationThread.hpp"

namespace simulator {

struct OutputParameters {
	std::vector<std::string> mEnvironment;							//!< Environment variables to output.
	std::map<std::string, std::vector<std::string> > mPopulation;	//!< Individuals variables to output by profile.

	OutputParameters() {}
	virtual ~OutputParameters() {}
};

/*!
 *  \class Simulator Simulator/include/Simulator.hpp "Simulator/include/Simulator.hpp"
 *  \brief Simulator class.
 */
class Simulator: public core::Object {
public:
	//! Simulator allocator type.
	typedef core::AllocatorT<Simulator, core::Object::Alloc> Alloc;
	//! Simulator handle type.
	typedef core::PointerT<Simulator, core::Object::Handle> Handle;
	//! Simulator bag type.
	typedef core::ContainerT<Simulator, core::Object::Bag> Bag;

	typedef std::map<unsigned int, std::queue<Process::Handle> > WaitingQMap;

	Simulator();
	virtual ~Simulator();

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Simulator");
			return lName;
		schnaps_StackTraceEndM("const std::string& Simulator::getName() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void configure(const std::string& inParameters);

	std::string getConfiguration();

	void simulate(std::string inScenarioLabel);

	void clearRandomizer();
	void resetRandomizer();

	const core::System::Handle getSystemHandle() const {
		return mSystem;
	}

	const core::System& getSystem() const {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	core::System& getSystem() {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	const Clock::Handle getClockHandle() const {
		return mClock;
	}

	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

	const Environment::Handle getEnvironmentHandle() const {
		return mEnvironment;
	}

	const Environment& getEnvironment() const {
		schnaps_NonNullPointerAssertM(mEnvironment);
		return *mEnvironment;
	}

	const PopulationManager::Handle getPopulationManagerHandle() const {
		return mPopulationManager;
	}

	const PopulationManager& getPopulationManager() const {
		schnaps_NonNullPointerAssertM(mPopulationManager);
		return *mPopulationManager;
	}

	PopulationManager& getPopulationManager() {
		schnaps_NonNullPointerAssertM(mPopulationManager);
		return *mPopulationManager;
	}

	static void processScenario(SimulationThread* inThread);
	static void processClockStep(SimulationThread* inThread);
	static void processSubStep(SimulationThread* inThread);

private:
	void init();

	// Sub reads
	void readInput(PACC::XML::ConstIterator inIter);
	void readSimulation(PACC::XML::ConstIterator inIter);
	void readOutput(PACC::XML::ConstIterator inIter);

	void readRandomizerInfo(PACC::XML::ConstIterator inIter);
	void readEnvironmentOutput(PACC::XML::ConstIterator inIter);
	void readPopulationOutput(PACC::XML::ConstIterator inIter);

	// Sub writes
	void writeInput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeSimulation(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writeEnvironmentOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	void writePopulationOutput(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	// Prints
	void printInput(std::ostream& ioStream, unsigned int inLowerBound) const;
	void printOutput(std::ostream& ioStream) const;
	void printSummary(std::ostream& ioStream) const;

private:
	// System structures
	core::System::Handle mSystem;					//!< Handle to system.
	Clock::Handle mClock;							//!< Handle to clock used for simulation.

	// Data structures
	Environment::Handle mEnvironment;				//!< Handle to environment.
	PopulationManager::Handle mPopulationManager;	//!< Handle to population manager.

	BlackBoard::Handle mBlackBoard;					//!< Handle to blackboard for push tracking.
	WaitingQMaps::Handle mWaitingQMaps;				//!< Handle to environment and individuals waiting queues.

	core::ULongArray mRandomizerInitSeed;			//!< Init registered seed of random number generators (one per thread).
	core::StringArray mRandomizerInitState;			//!< Init state of random number generators (one per thread).
	core::ULongArray mRandomizerCurrentSeed;		//!< Current registered seed of random number generators (one per thread).
	core::StringArray mRandomizerCurrentState;		//!< Current state of random number generators (one per thread).

	SimulationContext::Bag mContext;				//!< All simulation contexts (1 per thread).

	// Multi-threads management structures
	SimulationThread::Bag mSubThreads;				//!< Subthreads used when simulating.
	PACC::Threading::Condition* mParallel;			//!< Thread condition for triggering parallel execution.
	PACC::Threading::Semaphore* mSequential;		//!< Thread semaphore for triggering sequential execution.
	PACC::Threading::Semaphore* mBlackBoardWrt;		//!< Thread semaphore for modifying blackboard.

	OutputParameters mOutputParameters;				//!< Output parameters.
};
} // end of simulator namespace

#endif /* simulator_Simulator_hpp */
