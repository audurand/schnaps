/*
 * LearningModule.cpp
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
LearningModule::LearningModule()
{}

/*!
 * \brief Construct a branching primitive as a copy of an original.
 * \param inOriginal A const reference to the original branching primitive.
 */
LearningModule::LearningModule(const LearningModule& inOriginal) :
	mDecisionMakers(inOriginal.mDecisionMakers)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void LearningModule::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	unsigned int lNbThreads = Core::castHandleT<Core::UInt>(ioSystem.getParameters().getParameterHandle("threads.simulator"))->getValue();
	mDecisionMakers.clear();
	mDecisionMakers.resize(lNbThreads);
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
			mDecisionMakers[i].readWithSystem(inIter->getFirstChild(), ioSystem);
			mDecisionMakers[i].setThreadNb(i);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void LearningModule::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	mDecisionMakers[0].write(ioStreamer, inIndent);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Open decision makers log files in learning module.
 * \param inFileName A const reference to the common filename.
 */
void LearningModule::open(const std::string& inFileName) {
	schnaps_StackTraceBeginM();
	std::stringstream lSS;
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		lSS.str("");
		lSS << inFileName << "_" << i << ".gz";
		mDecisionMakers[i].open(lSS.str());
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::open(const std::string&)");
}

/*!
 * \brief Close decision makers log files in learning module.
 */
void LearningModule::close() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		mDecisionMakers[i].close();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::clost()");
}

/*!
 * \brief Update decision makers in learning module from specific log files.
 * \param inFileName A const reference to the common filename.
 */
void LearningModule::update(const std::string& inFileName) {
	schnaps_StackTraceBeginM();
	std::stringstream lSS;
	igzstream *lIGZS;
	PACC::Tokenizer *lTokenizer;
	std::string lChoiceNode, lState, lActionIndex, lIndividualIndex;
	
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		lSS.str("");
		lSS << inFileName << "_" << i << ".gz";
		
		lIGZS = new igzstream(lSS.str().c_str());
		lTokenizer = new PACC::Tokenizer(*lIGZS);
		lTokenizer->setDelimiters(",\n", "");
		
		while (lTokenizer->getNextToken(lChoiceNode)) {
			lTokenizer->getNextToken(lState);
			lTokenizer->getNextToken(lActionIndex);
			lTokenizer->getNextToken(lIndividualIndex);
			
			for (unsigned int j = 0; j < mDecisionMakers.size(); j++) {
				mDecisionMakers[j].update(lChoiceNode, lState, SCHNAPS::str2uint(lActionIndex), SCHNAPS::str2uint(lIndividualIndex));
			}
		}
		
		lIGZS->close();
		delete lTokenizer;
		delete lIGZS;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::update(const std::string&)");
}

/*!
 * \brief Set the system of decision makers in learning module.
 * \param inSystem A handle to the system.
 */
void LearningModule::setSystem(Core::System::Handle inSystem) {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		mDecisionMakers[i].setSystem(inSystem);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::setSystem(SCHNAPS::Core::System::Handle)");
}

/*!
 * \brief Set the environment of decision makers in learning module.
 * \param inEnvironment A handle to the environment.
 */
void LearningModule::setEnvironment(Simulation::Environment::Handle inEnvironment) {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		mDecisionMakers[i].setEnvironment(inEnvironment);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::setEnvironment(SCHNAPS::Simulation::Environment::Handle)");
}
/*!
 * \brief Set the clock of decision makers in learning module.
 * \param inClock A handle to the clock.
 */
void LearningModule::setClock(Simulation::Clock::Handle inClock) {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mDecisionMakers.size(); i++) {
		mDecisionMakers[i].setClock(inClock);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningModule::setClock(SCHNAPS::Simulation::Clock::Handle)");
}
