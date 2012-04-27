/*
 * SimulationContext.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Default constructor.
 */ 
SimulationContext::SimulationContext() :
	ExecutionContext()
{}

/*!
 * \brief Construct a simulation context as a copy of an original.
 * \param inOriginal A const reference to the original simulation context.
 */
SimulationContext::SimulationContext(const SimulationContext& inOriginal) :
	ExecutionContext(inOriginal),
	mProcesses(inOriginal.mProcesses),
	mObserversForEnvironment(inOriginal.mObserversForEnvironment),
	mObserversForIndividuals(inOriginal.mObserversForIndividuals),
	mScenarios(inOriginal.mScenarios)
{}

/*!
 * \brief Construct a simulation context with specific system, clock, and environment.
 * \param inSystem A const handle to the system.
 * \param inClock A const handle to the clock.
 * \param inEnvironment A const handle to the environment.
 */
SimulationContext::SimulationContext(const Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment) :
		ExecutionContext(inSystem, inClock, inEnvironment)
{}

/*!
 * \brief Read processes from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::AssertException if the system is NULL.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if process label attribute is missing.
 */
void SimulationContext::readProcesses(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
	
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Processes") {
		std::ostringstream lOSS;
		lOSS << "tag <Processes> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading processes\n");
#endif

	PACC::XML::Document lDocument;
	std::string lFile;

	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Process") {
				std::ostringstream lOSS;
				lOSS << "tag <Process> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			if (lChild->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "process label attribute expected!");
			}

#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading process %s\n", lChild->getAttribute("label").c_str());
#endif

			lFile = lChild->getAttribute("file");
			mProcesses.insert(std::pair<std::string, Process::Handle>(lChild->getAttribute("label"), new Process()));
			if (lFile.empty()) {
				mProcesses[lChild->getAttribute("label")]->readWithSystem(lChild, *mSystem);
			} else {
				lDocument.parse(lFile);
				mProcesses[lChild->getAttribute("label")]->readWithSystem(lDocument.getFirstDataTag(), *mSystem);
			}
//				mProcesses[lChild->getAttribute("label")]->validate(*this);
			// TODO: uncomment process validation on read.
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::readProcesses(PACC::XML::ConstIterator)");
}

/*!
 * \brief Read observers from XML.
 * \param inIter XML iterator of input document.
 * \throw  SCHNAPS::Core::AssertException if the simulation clock has not been defined.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if clock observer process attribute is missing.
 * \throw SCHNAPS::Core::IOException if clock observer target attribute is missing.
 * \throw SCHNAPS::Core::IOException if clock observer start < 1.
 * \throw SCHNAPS::Core::IOException if clock observer step size < 1.
 * \throw SCHNAPS::Core::RunTimeException if observer target attribute is unknown.
 */
void SimulationContext::readObservers(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mClock);
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "ClockObservers") {
		std::ostringstream lOSS;
		lOSS << "tag <ClockObservers> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	mObserversForEnvironment.clear();
	mObserversForIndividuals.clear();
	
	Clock::Units lUnits;
	unsigned long lStart, lEnd, lStep, lFirstOccurence;
	
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Observer") {
				std::ostringstream lOSS;
				lOSS << "tag <Observer> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			if (lChild->getAttribute("process").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "clock observer process attribute expected!");
			}
			if (lChild->getAttribute("target").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "clock observer target attribute expected!");
			}
			
			if (lChild->getAttribute("start").empty()) {
				lStart = 1;
				if (lStart < 1) {
					throw schnaps_IOExceptionNodeM(*lChild, "clock observer start >= 1!");
				}
			} else {
				lStart = SCHNAPS::str2uint(lChild->getAttribute("start"));
			}
			
			if (lChild->getAttribute("end").empty()) {
				lEnd = 0;
			} else {
				lEnd = SCHNAPS::str2uint(lChild->getAttribute("end"));
			}
			
			if (lChild->getAttribute("step").empty()) {
				lStep = 1;
			} else {
				lStep = SCHNAPS::str2uint(lChild->getAttribute("step"));
				if (lStep < 1) {
					throw schnaps_IOExceptionNodeM(*lChild, "clock observer step size must be >= 1!");
				}
			}
			
			if (lChild->getAttribute("units").empty()) {
				lUnits = Clock::eOther;
			} else {
				if (lChild->getAttribute("units") == "year") {
					lUnits = Clock::eYear;
				} else if (lChild->getAttribute("units") == "month") {
					lUnits = Clock::eMonth;
				} else if (lChild->getAttribute("units") == "week") {
                                        lUnits = Clock::eWeek;
				} else if (lChild->getAttribute("units") == "day") {
					lUnits = Clock::eDay;
				} else {
					lUnits = Clock::eOther;
				}
			}

			if (lChild->getAttribute("target") == "environment") {
				mObserversForEnvironment.insert(std::pair<std::string, Observer>(lChild->getAttribute("process"), Observer(this->getProcessHandle(lChild->getAttribute("process")), lStart, lEnd, lStep, lUnits)));
			} else if (lChild->getAttribute("target") == "individuals") {
				mObserversForIndividuals.insert(std::pair<std::string, Observer>(lChild->getAttribute("process"), Observer(this->getProcessHandle(lChild->getAttribute("process")), lStart, lEnd, lStep, lUnits)));
			} else {
				throw schnaps_RunTimeExceptionM("Undefined clock observer target '" + lChild->getAttribute("target") + "'!");
			}
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::readObservers(PACC::XML::ConstIterator)");
}

/*!
 * \brief Read scenarios from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if scenario label attribute is missing.
 */
void SimulationContext::readScenarios(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Scenarios") {
		std::ostringstream lOSS;
		lOSS << "tag <Scenarios> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Scenario") {
				std::ostringstream lOSS;
				lOSS << "tag <Scenario> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			if (lChild->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "scenario label attribute expected!");
			}

			mScenarios.insert(std::pair<std::string, Scenario>(lChild->getAttribute("label"), Scenario()));
			if (!lChild->getAttribute("processEnvironment").empty()) {
				mScenarios[lChild->getAttribute("label")].mProcessEnvironment = this->getProcessHandle(lChild->getAttribute("processEnvironment"));
			}
			if (!lChild->getAttribute("processIndividual").empty()) {
				mScenarios[lChild->getAttribute("label")].mProcessIndividual = this->getProcessHandle(lChild->getAttribute("processIndividual"));
			}
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::readScenarios(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write processes content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SimulationContext::writeProcesses(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Processes");
	for (ProcessMap::const_iterator lIt = mProcesses.begin(); lIt != mProcesses.end(); lIt++) {
		lIt->second->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::writeProcesses(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write clock observers content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SimulationContext::writeObservers(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("ClockObservers");
	for (ObserverMap::const_iterator lIt_i = mObserversForEnvironment.begin(); lIt_i != mObserversForEnvironment.end(); lIt_i++) {
		ioStreamer.openTag("Observer");
		ioStreamer.insertAttribute("target", "environment");
		ioStreamer.insertAttribute("process", lIt_i->second.mProcess->getLabel());
		ioStreamer.insertAttribute("start", lIt_i->second.mStart);
		ioStreamer.insertAttribute("end", lIt_i->second.mEnd);
		ioStreamer.insertAttribute("step", lIt_i->second.mStep);
		switch (lIt_i->second.mUnits) {
			case Clock::eYear:
				ioStreamer.insertAttribute("units", "year");
				break;
			case Clock::eMonth:
				ioStreamer.insertAttribute("units", "month");
				break;
			case Clock::eWeek:
                                ioStreamer.insertAttribute("units", "week");
                                break;
			case Clock::eDay:
				ioStreamer.insertAttribute("units", "day");
				break;
			default:
				ioStreamer.insertAttribute("units", "other");
				break;
		}
		ioStreamer.closeTag();
	}
	for (ObserverMap::const_iterator lIt_i = mObserversForIndividuals.begin(); lIt_i != mObserversForIndividuals.end(); lIt_i++) {
		ioStreamer.openTag("Observer");
		ioStreamer.insertAttribute("target", "individuals");
		ioStreamer.insertAttribute("process", lIt_i->second.mProcess->getLabel());
		ioStreamer.insertAttribute("start", lIt_i->second.mStart);
		ioStreamer.insertAttribute("end", lIt_i->second.mEnd);
		ioStreamer.insertAttribute("step", lIt_i->second.mStep);
		switch (lIt_i->second.mUnits) {
			case Clock::eYear:
				ioStreamer.insertAttribute("units", "Year");
				break;
			case Clock::eMonth:
				ioStreamer.insertAttribute("units", "Month");
				break;
			case Clock::eWeek:
                                ioStreamer.insertAttribute("units", "Week");
                                break;
			case Clock::eDay:
				ioStreamer.insertAttribute("units", "Day");
				break;
			default:
				ioStreamer.insertAttribute("units", "Other");
				break;
		}
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::writeObservers(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write scenarios content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SimulationContext::writeScenarios(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Scenarios");
	for (ScenarioMap::const_iterator lIt = mScenarios.begin(); lIt != mScenarios.end(); lIt++) {
		ioStreamer.openTag("Scenario");
		ioStreamer.insertAttribute("label", lIt->first);
		if (lIt->second.mProcessEnvironment != NULL) {
			ioStreamer.insertAttribute("processEnvironment", lIt->second.mProcessEnvironment->getLabel());
		}
		if (lIt->second.mProcessIndividual != NULL) {
			ioStreamer.insertAttribute("processIndividual", lIt->second.mProcessIndividual->getLabel());
		}
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::SimulationContext::writeScenarios(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Return a handle to a deep copy of the simulation context.
 * \return A handle to a deep copy of the simulation context.
 */
SimulationContext::Handle SimulationContext::deepCopy() const {
	schnaps_StackTraceBeginM();
	SimulationContext::Handle lCopy = new SimulationContext(mSystem, mClock, mEnvironment);

	// copy processes
	for (ProcessMap::const_iterator lIt = this->mProcesses.begin(); lIt != this->mProcesses.end(); lIt++) {
		lCopy->mProcesses[lIt->first] = Core::castHandleT<Process>(lIt->second->deepCopy(*mSystem));
	}

	// copy clock observers
	for (ObserverMap::const_iterator lIt_i = mObserversForEnvironment.begin(); lIt_i != mObserversForEnvironment.end(); lIt_i++) {
		lCopy->mObserversForEnvironment.insert(std::pair<std::string, Observer>(lIt_i->first, Observer(lIt_i->second.mProcess->deepCopy(*mSystem), lIt_i->second.mStart, lIt_i->second.mEnd, lIt_i->second.mStep, lIt_i->second.mUnits)));
	}
	for (ObserverMap::const_iterator lIt_i = mObserversForIndividuals.begin(); lIt_i != mObserversForIndividuals.end(); lIt_i++) {
		lCopy->mObserversForIndividuals.insert(std::pair<std::string, Observer>(lIt_i->first, Observer(lIt_i->second.mProcess->deepCopy(*mSystem), lIt_i->second.mStart, lIt_i->second.mEnd, lIt_i->second.mStep, lIt_i->second.mUnits)));
	}

	// copy scenarios
	for (ScenarioMap::const_iterator lIt = this->mScenarios.begin(); lIt != this->mScenarios.end(); lIt++) {
		if (lIt->second.mProcessEnvironment == NULL) {
			lCopy->mScenarios[lIt->first].mProcessEnvironment = NULL;
		} else {
			lCopy->mScenarios[lIt->first].mProcessEnvironment = Core::castHandleT<Process>(lIt->second.mProcessEnvironment->deepCopy(*mSystem));
		}
		if (lIt->second.mProcessIndividual == NULL) {
			lCopy->mScenarios[lIt->first].mProcessIndividual = NULL;
		} else {
			lCopy->mScenarios[lIt->first].mProcessIndividual = Core::castHandleT<Process>(lIt->second.mProcessIndividual->deepCopy(*mSystem));
		}
	}
	
	// copy local variables
	for (LocalVariablesMap::const_iterator lIt = this->mLocalVariables.begin(); lIt != this->mLocalVariables.end(); lIt++) {
		lCopy->mLocalVariables.insert(std::pair<std::string, Core::AnyType::Handle>(
			lIt->first.c_str(),
			Core::castHandleT<Core::AnyType>(lIt->second->clone())));
	}

	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::SimulationContext::Handle SCHNAPS::Simulation::SimulationContext::deepCopy() const");
}
