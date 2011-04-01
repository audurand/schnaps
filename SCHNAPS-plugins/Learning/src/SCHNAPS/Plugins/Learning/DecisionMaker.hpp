/*
 * DecisionMaker.hpp
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

#ifndef SCHNAPS_Plugins_Learning_DecisionMaker_hpp
#define SCHNAPS_Plugins_Learning_DecisionMaker_hpp

#include "SCHNAPS/Plugins/Learning/Action.hpp"
#include "SCHNAPS/Plugins/Learning/Choice.hpp"
#include "SCHNAPS/Plugins/Learning/LearningContext.hpp"

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/PACC.hpp"

#include <map>

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class DecisionMaker SCHNAPS/Plugins/Learning/DecisionMaker.hpp "SCHNAPS/Plugins/Learning/DecisionMaker.hpp"
 *  \brief Make the decision of next action to execute.
 */
class DecisionMaker: public Core::Object, public std::map<std::string, Choice > {
public:
	//! DecisionMaker allocator type.
	typedef Core::AllocatorT<DecisionMaker, Core::Object::Alloc> Alloc;
	//! DecisionMaker handle type.
	typedef Core::PointerT<DecisionMaker, Core::Object::Handle> Handle;
	//! DecisionMaker bag type.
	typedef Core::ContainerT<DecisionMaker, Core::Object::Bag> Bag;
	
	DecisionMaker();
	DecisionMaker(const DecisionMaker& inOriginal);
	virtual ~DecisionMaker() {}
	
	//! Copy operator.
	DecisionMaker& operator=(const DecisionMaker& inOriginal);
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_DecisionMaker");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::DecisionMaker::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	
	//! Open log file.
	void open(const std::string& inFileName);
	//! Close log file.
	void close();
	//! Update information of a specific decision node.
	void update(const std::string& inDecisionNode, const std::string& inState, unsigned int inActionID, unsigned int inIndividualID);
	
	//! Set the system.
	void setSystem(Core::System::Handle inSystem);
	//! Set the environment.
	void setEnvironment(Simulation::Environment::Handle inEnvironment);
	//! Set the clock.
	void setClock(Simulation::Clock::Handle inClock);
	//! Set the thread number associated.
	void setThreadNb(unsigned int inThreadNb);
	
	/*!
	 * \brief  Return the index of next action to execute in specific decision node.
	 * \param  inDecisionNode A const reference to the decision node.
	 * \param  inIndividual A handle to the current individual processing.
	 * \return The index of next action to execute in specific decision node.
	 * \throw  SCHNAPS::RunTimeException if decision node is not in decision maker.
	 */
	unsigned int getActionID(const std::string& inDecisionNode, Simulation::Individual::Handle inIndividual) {
		schnaps_StackTraceBeginM();
		if (this->find(inDecisionNode) == this->end()) {
			throw schnaps_RunTimeExceptionM("Decision node '" + inDecisionNode + "' is not in decision maker; could not make choice.");
		}
		// get choice learning data
		Choice& lChoice = this->find(inDecisionNode)->second;
		
		// set current individual in learning context
		mContext.setIndividual(inIndividual);
		
		// compute current state
		std::string lCurrentState = lChoice.computeState(mContext);
		
		// get action stats for that state
		std::vector<Action>& lActions = lChoice.getActions(lCurrentState);
		
		// make choice using GEAS
		unsigned int lActionID;
		if (mLearning->getValue()) {
			// learning mode
			std::vector<unsigned int> lActionsNeverTried;
			int lNbActionsRequireMoreUpdates = 0;
			double lN = 0;
			double lSum = 0;
			double lSum2 = 0;
			
			for (unsigned int i = 0; i < lActions.size(); i++) {
				if (lActions[i].getTried() == false) {
					lActionsNeverTried.push_back(i);
				} else if (lActions[i].getUpdatedTimes() < 1) {
					lNbActionsRequireMoreUpdates++;
				} else {
					lN += lActions[i].getUpdatedTimes();
					lSum += lActions[i].getReward();
					lSum2 += lActions[i].getReward2();
				}
			}
			
			if (lActionsNeverTried.size() > 0) {
				// some actions still never tried for that state, random actions in that lot
				lActionID = lActionsNeverTried[mContext.getRandomizer().rollInteger(0, lActionsNeverTried.size()-1)];
			} else if (lNbActionsRequireMoreUpdates > 0) {
				// all actions tried, but some require more updates, random all
				lActionID = mContext.getRandomizer().rollInteger(0, lActions.size()-1);
			} else {
				// very actions tried at least two times
				double lVariance, lCoeff, lMean, lCurrentValue, lMaxValue;
				std::vector<unsigned int> lMaxActions;
				
				lVariance = abs(lSum2/lN - pow(lSum/lN, 2));
				
				lMean = lActions[0].getReward()/lActions[0].getUpdatedTimes();
				lCoeff = pow((log(lN)/lActions[0].getUpdatedTimes()), mGEAS_Alpha->getValue());
				
				lMaxValue = mContext.getRandomizer().rollGaussian(lMean, sqrt(lCoeff * lVariance));
				lMaxActions.push_back(0);
				for (unsigned int i = 1; i < lActions.size(); i++) {
					lMean = lActions[i].getReward()/lActions[i].getUpdatedTimes();
					lCoeff = pow((log(lN)/lActions[i].getUpdatedTimes()), mGEAS_Alpha->getValue());
					
					lCurrentValue = mContext.getRandomizer().rollGaussian(lMean, sqrt(lCoeff * lVariance));
					if (lCurrentValue > lMaxValue) {
						lMaxValue = lCurrentValue;
						lMaxActions.clear();
						lMaxActions.push_back(i);
					} else if (lCurrentValue == lMaxValue) {
						lMaxActions.push_back(i);
					}
				}
				
				if (lMaxActions.size() > 1) {
					// multiple best actions, choose randomly between these
					lActionID = lMaxActions[mContext.getRandomizer().rollInteger(0, lMaxActions.size()-1)];
				} else {
					lActionID = lMaxActions[0];
				}
			}
			
			// log move
			(*mOGZS) << inDecisionNode << "," << lCurrentState << "," << lActionID << "," << inIndividual->getIndex() << "\n";
			mOGZS->flush();
		} else {
			// exploitation mode = action with best mean
			double lCurrentValue, lMaxValue;
			std::vector<unsigned int> lMaxActions;
			
			lMaxValue = lActions[0].getReward()/lActions[0].getUpdatedTimes();
			lMaxActions.push_back(0);
			for (unsigned int i = 1; i < lActions.size(); i++) {
				lCurrentValue = lActions[i].getReward()/lActions[i].getUpdatedTimes();
				if (lCurrentValue > lMaxValue) {
					lMaxValue = lCurrentValue;
					lMaxActions.clear();
					lMaxActions.push_back(i);
				} else if (lCurrentValue == lMaxValue) {
					lMaxActions.push_back(i);
				}
			}
			
			if (lMaxActions.size() > 1) {
				// multiple best actions, choose randomly between these
				lActionID = lMaxActions[mContext.getRandomizer().rollInteger(0, lMaxActions.size()-1)];
			} else {
				lActionID = lMaxActions[0];
			}
		}
		
		return lActionID;
		schnaps_StackTraceEndM("unsigned int SCHNAPS::Plugins::Learning::DecisionMaker::getActionID(const std::string&, SCHNAPS::Plugins::Simulation::Individual::Handle)");
	}
	
private:
	LearningContext mContext;			//!< The execution context used for computation of functions.
	ogzstream *mOGZS;					//!< A pointer to the output log file.
	Core::Bool::Handle mLearning;		//!< A handle indicating if learning mode is activated (else it is exploitation mode).
	Core::Double::Handle mGEAS_Alpha;	//!< A handle to the alpha parameter of GEAS.
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_DecisionMaker_hpp */
