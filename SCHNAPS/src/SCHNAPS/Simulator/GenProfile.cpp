/*
 * GenProfile.cpp
 *
 *  Created on: 2010-04-14
 * Author: Audrey Durand
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
#include "SCHNAPS/Simulator.hpp"

using namespace simulator;

GenProfile::GenProfile() :
		mDemography(new Demography()),
		mAcceptFunction(new core::PrimitiveTree()),
		mIndividualModel(new IndividualModel()),
		mSimulationVariables(new SimulationVariables())
{}

GenProfile::GenProfile(const GenProfile& inOriginal) :
		mDemography(inOriginal.mDemography),
		mAcceptFunction(inOriginal.mAcceptFunction),
		mIndividualModel(inOriginal.mIndividualModel),
		mSimulationVariables(inOriginal.mSimulationVariables)
{}

GenProfile::GenProfile(Demography::Handle inDemography, core::PrimitiveTree::Handle inAcceptFunction, IndividualModel::Handle inIndividualModel, SimulationVariables::Handle inSimulationVariables) :
		mDemography(inDemography),
		mAcceptFunction(inAcceptFunction),
		mIndividualModel(inIndividualModel),
		mSimulationVariables(inSimulationVariables)
{}

void GenProfile::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" + getName() + "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
		if (inIter->getAttribute("label").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "label attribute expected!");
		}

#ifdef SCHNAPS_FULL_DEBUG
		printf("Reading profile: %s\n", mLabel.c_str());
#endif

		PACC::XML::Document lDocument;

		mLabel = inIter->getAttribute("label");

		inIter = inIter->getFirstChild();
		mDemography->readWithSystem(inIter++, ioSystem);

		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != "AcceptFunction") {
			std::ostringstream lOSS;
			lOSS << "tag <AcceptFunction> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
		mAcceptFunction->readWithSystem(inIter->getFirstChild(), ioSystem);
		inIter++;

		mIndividualModel->readWithSystem(inIter++, ioSystem);
		mSimulationVariables->readWithSystem(inIter++, ioSystem);
	schnaps_StackTraceEndM("void simulator::GenProfile::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void GenProfile::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("label", mLabel);

	mDemography->write(ioStreamer, inIndent);

	ioStreamer.openTag("AcceptFunction", inIndent);
	mAcceptFunction->write(ioStreamer, inIndent);
	ioStreamer.closeTag();

	mIndividualModel->write(ioStreamer, inIndent);
	mSimulationVariables->write(ioStreamer, inIndent);
}

GenProfile::Handle GenProfile::deepCopy(const core::System& ioSystem) const {
	schnaps_StackTraceBeginM();
		GenProfile::Handle lGenProfile = new GenProfile();

		// Deep copy demography
		lGenProfile->mDemography = new Demography();
		lGenProfile->mDemography->reserve(this->mDemography->size());
		for (unsigned int i = 0; i < this->mDemography->size(); i++) {
			lGenProfile->mDemography->push_back(std::pair<std::string, core::PrimitiveTree::Handle>((*this->mDemography)[i].first.c_str(), (*this->mDemography)[i].second->deepCopy(ioSystem)));
		}

		// Deep copy accept function
		lGenProfile->mAcceptFunction = this->mAcceptFunction->deepCopy(ioSystem);

		// Deep copy individual model
		lGenProfile->mIndividualModel = new IndividualModel();
		for (IndividualModel::const_iterator lIt = this->mIndividualModel->begin(); lIt != this->mIndividualModel->end(); lIt++) {
			lGenProfile->mIndividualModel->insert((*lIt).c_str());
		}

		// Deep copy simulator variables
		lGenProfile->mSimulationVariables = new SimulationVariables();
		lGenProfile->mSimulationVariables->reserve(this->mSimulationVariables->size());
		for (unsigned int i = 0; i < this->mSimulationVariables->size(); i++) {
			lGenProfile->mSimulationVariables->push_back(std::pair<std::string, core::PrimitiveTree::Handle>((*this->mSimulationVariables)[i].first.c_str(), (*this->mSimulationVariables)[i].second->deepCopy(ioSystem)));
		}

		return lGenProfile;
	schnaps_StackTraceEndM("simulator::GenProfile::Handle simulator::GenProfile::deepCopy(const core::System&) const");
}
