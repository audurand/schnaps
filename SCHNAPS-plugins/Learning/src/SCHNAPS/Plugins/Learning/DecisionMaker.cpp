/*
 * DecisionMaker.cpp
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

#include "SCHNAPS/Plugins/Learning/Learning.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

/*!
 * \brief Default constructor.
 */
DecisionMaker::DecisionMaker() :
	mOFS(NULL),
	mLearning(NULL),
	mGEAS_Alpha(NULL)
{}

/*!
 * \brief Construct a decision maker as a copy of an original.
 * \param inOriginal A const reference to the original decision maker.
 */
DecisionMaker::DecisionMaker(const DecisionMaker& inOriginal) :
	mOFS(inOriginal.mOFS),
	mLearning(inOriginal.mLearning),
	mGEAS_Alpha(inOriginal.mGEAS_Alpha)
{
	clear();
	for (DecisionMaker::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
		insert(std::pair<std::string, Choice::Handle>(lIt->first, lIt->second));
	}
	
	mContext.setSystem(inOriginal.mContext.getSystemHandle());
	mContext.setEnvironment(inOriginal.mContext.getEnvironmentHandle());
	
	if (mContext.getSystemHandle() != NULL) {
		mLearning = Core::castHandleT<Core::Bool>(mContext.getSystem().getParameters().getParameterHandle("learning.learn"));
		mGEAS_Alpha = Core::castHandleT<Core::Double>(mContext.getSystem().getParameters().getParameterHandle("learning.geas.alpha"));
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
DecisionMaker& DecisionMaker::operator=(const DecisionMaker& inOriginal) {
	mOFS = inOriginal.mOFS;
	mLearning = inOriginal.mLearning;
	mGEAS_Alpha = inOriginal.mGEAS_Alpha;
	
	clear();
	for (DecisionMaker::const_iterator lIt = inOriginal.begin(); lIt != inOriginal.end(); lIt++) {
		insert(std::pair<std::string, Choice::Handle>(lIt->first, lIt->second));
	}
	
	mContext.setSystem(inOriginal.mContext.getSystemHandle());
	mContext.setEnvironment(inOriginal.mContext.getEnvironmentHandle());
	
	if (mContext.getSystemHandle() != NULL) {
		mLearning = Core::castHandleT<Core::Bool>(mContext.getSystem().getParameters().getParameterHandle("learning.learn"));
		mGEAS_Alpha = Core::castHandleT<Core::Double>(mContext.getSystem().getParameters().getParameterHandle("learning.geas.alpha"));
	}
}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \param  inSystem A const reference to the system.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle DecisionMaker::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	DecisionMaker::Handle lCopy = new DecisionMaker();
	
	for (DecisionMaker::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
		lCopy->insert(std::pair<std::string, Choice::Handle>(lIt->first.c_str(), Core::castHandleT<Choice>(lIt->second->deepCopy(inSystem))));
	}
	
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Plugins::Learning::DecisionMaker::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void DecisionMaker::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" << getName() << "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

	// read choices
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*lChild, "choice label expected!");
		}
		this->insert(std::pair<std::string, Choice::Handle>(lChild->getAttribute("label"), new Choice()));
		(*this)[lChild->getAttribute("label")]->readWithSystem(lChild, ioSystem);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write content of object to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void DecisionMaker::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	for (DecisionMaker::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
		lIt->second->write(ioStreamer, inIndent);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Open log file.
 * \param inFilename A const reference to the name of log file.
 */
void DecisionMaker::open(const std::string& inFilename) {
	schnaps_StackTraceBeginM();
	mOFS = new std::ofstream(inFilename.c_str());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::open(const std::string&)");
}

/*!
 * \brief Close log file.
 */
void DecisionMaker::close() {
	schnaps_StackTraceBeginM();
	mOFS->close();
	delete mOFS;
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::close()");
}

/*!
 * \brief  Compute the reward associated to an individual according to specific information.
 * \param  inDecisionNode A const reference to the decision node where choice occured.
 * \param  inState A const reference to the state.
 * \param  inActionID The ID of action taken.
 * \param  inIndividualID The ID of individual on which decision occured.
 * \return The reward associated to an individual according to specific information.
 */
double DecisionMaker::computeReward(const std::string& inDecisionNode, const std::string& inState, unsigned int inActionID, unsigned int inIndividualID) {
	schnaps_StackTraceBeginM();
	DecisionMaker::iterator lIterDecisionNode = this->find(inDecisionNode);
	if (lIterDecisionNode == this->end()) {
		throw schnaps_RunTimeExceptionM("Decision node '" + inDecisionNode + "' is not in decision maker; could not compute reward.");
	}
	// get choice learning data
	Choice& lChoice = *(lIterDecisionNode->second);
	std::vector<Action>& lActions = lChoice.getActions(inState);
	
	// set individual (using the absolute index part of its ID)
	mContext.setIndividualByIndex(inIndividualID);
	
	// set action label
	mContext.setActionLabel(lActions[inActionID].getLabel());
	
	// set state
	mContext.setState(inState);
	
	// compute reward
	return lChoice.computeReward(mContext);
	schnaps_StackTraceEndM("double SCHNAPS::Plugins::Learning::DecisionMaker::computeReward(const std::string&, const std::string&, unsigned int, unsigned int)");
}

/*!
 * \brief Update information of a specific decision node.
 * \param inDecisionNode A const reference to the decision node where choice occured.
 * \param inState A const reference to the state.
 * \param inActionID The ID of action taken.
 * \param inReward The reward obtained in these conditions.
 */
void DecisionMaker::update(const std::string& inDecisionNode, const std::string& inState, unsigned int inActionID, double inReward) {
	schnaps_StackTraceBeginM();
	DecisionMaker::iterator lIterDecisionNode = this->find(inDecisionNode);
	if (lIterDecisionNode == this->end()) {
		throw schnaps_RunTimeExceptionM("Decision node '" + inDecisionNode + "' is not in decision maker; could not update choice.");
	}
	// get choice learning data
	Choice& lChoice = *(lIterDecisionNode->second);
	std::vector<Action>& lActions = lChoice.getActions(inState);
	
	// update parameters
	lActions[inActionID].setTried(true);
	lActions[inActionID].setUpdatedTimes(lActions[inActionID].getUpdatedTimes() + 1);
	lActions[inActionID].setReward(lActions[inActionID].getReward() + inReward);
	lActions[inActionID].setReward2(lActions[inActionID].getReward2() + pow(inReward, 2));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::update(const std::string&, const std::string&, unsigned int, double)");
}

/*!
 * \brief Set the system.
 * \param inSystem A handle to the system.
 */
void DecisionMaker::setSystem(Core::System::Handle inSystem) {
	schnaps_StackTraceBeginM();
	mContext.setSystem(inSystem);
	if (inSystem != NULL) {
		mLearning = Core::castHandleT<Core::Bool>(inSystem->getParameters().getParameterHandle("learning.learn"));
		mGEAS_Alpha = Core::castHandleT<Core::Double>(inSystem->getParameters().getParameterHandle("learning.geas.alpha"));
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::setSystem(SCHNAPS::Core::System::Handle)");
}

/*!
 * \brief Set the environment.
 * \param inEnvironment A handle to the environment.
 */
void DecisionMaker::setEnvironment(Simulation::Environment::Handle inEnvironment) {
	schnaps_StackTraceBeginM();
	mContext.setEnvironment(inEnvironment);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::setEnvironment(SCHNAPS::Simulation::Environment::Handle)");
}

/*!
 * \brief Set the clock.
 * \param inClock A handle to the clock.
 */
void DecisionMaker::setClock(Simulation::Clock::Handle inClock) {
	schnaps_StackTraceBeginM();
	mContext.setClock(inClock);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::setClock(SCHNAPS::Simulation::Clock::Handle)");
}

/*!
 * \brief Set the thread number associated.
 * \param inThreadNb The thread number associated.
 */
void DecisionMaker::setThreadNb(unsigned int inThreadNb) {
	schnaps_StackTraceBeginM();
	mContext.setThreadNb(inThreadNb);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionMaker::setThreadNb(unsigned int)");
}
