/*
 * LearningModule.hpp
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

#ifndef SCHNAPS_Plugins_Learning_LearningModule_hpp
#define SCHNAPS_Plugins_Learning_LearningModule_hpp

#include "SCHNAPS/Plugins/Learning/DecisionMaker.hpp"
#include "SCHNAPS/Plugins/Learning/UpdateThread.hpp"

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/PACC.hpp"

#include <vector>

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class LearningModule SCHNAPS/Plugins/Learning/LearningModule.hpp "SCHNAPS/Plugins/Learning/LearningModule.hpp"
 *  \brief System component for managing the learning process.
 */
class LearningModule: public Core::Component {
public:
	//! LearningModule allocator type.
	typedef Core::AllocatorT<LearningModule, Core::Component::Alloc> Alloc;
	//! LearningModule handle type.
	typedef Core::PointerT<LearningModule, Core::Component::Handle> Handle;
	//! LearningModule bag type.
	typedef Core::ContainerT<LearningModule, Core::Component::Bag> Bag;
	
	LearningModule();
	LearningModule(const LearningModule& inOriginal);
	virtual ~LearningModule() {}
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_LearningModule");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::LearningModule::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Initialize the component.
	virtual void init(Core::System& ioSystem);
	
	//! Open decision makers log files in learning module.
	void open(const std::string& inFileName);
	//! Close decision makers log files in learning module.
	void close();
	//! Update decision makers in learning module from specific log files.
	void update(const std::string& inFileName);
	
	//! Set the system of decision makers in learning module.
	void setSystem(Core::System::Handle inSystem);
	//! Set the environment of decision makers in learning module.
	void setEnvironment(Simulation::Environment::Handle inEnvironment);
	//! Set the clock of decision makers in learning module.
	void setClock(Simulation::Clock::Handle inClock);
	
	/*!
	 * \brief  Return the ID of selected action.
	 * \param  inDecisionNode A const reference to the decision node.
	 * \param  ioContext A referene to the simulation context.
	 * \return The ID of selected action.
	 */
	unsigned int getActionID(const std::string& inDecisionNode, Simulation::SimulationContext& ioContext) {
		schnaps_StackTraceBeginM();
		return mDecisionMakers[ioContext.getThreadNb()]->getActionID(inDecisionNode, ioContext.getIndividualHandle());
		schnaps_StackTraceEndM("unsigned int SCHNAPS::Plugins::Learning::LearningModule::getActionID(const std::string&, SCHNAPS::Simulation::SimulationContext&)");
	}
	
private:
	DecisionMaker::Bag mDecisionMakers;
	
#ifdef PARALLEL_UPDATE
	// multi-threads update management structures
	UpdateThread::Bag mSubThreads;				//!< Subthreads used when updating.
	PACC::Threading::Condition* mParallel;		//!< Thread condition for triggering parallel execution in update process.
	PACC::Threading::Semaphore* mSequential;	//!< Thread semaphore for triggering sequential execution in update process.
#endif
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_LearningModule_hpp */
