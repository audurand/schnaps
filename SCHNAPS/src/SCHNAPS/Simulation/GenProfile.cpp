/*
 * GenProfile.cpp
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
GenProfile::GenProfile() :
	mDemography(new Demography()),
	mAcceptFunction(new SCHNAPS::Core::PrimitiveTree()),
	mIndividualModel(new IndividualModel()),
	mSimulationVariables(new SimulationVariables())
{}

/*!
 * \brief Construct a generation profile as a copy of an original.
 * \param inOriginal A const reference to the original generation profile.
 */
GenProfile::GenProfile(const GenProfile& inOriginal) :
	mDemography(inOriginal.mDemography),
	mAcceptFunction(inOriginal.mAcceptFunction),
	mIndividualModel(inOriginal.mIndividualModel),
	mSimulationVariables(inOriginal.mSimulationVariables)
{}

/*!
 * \brief Construct a generation profile with specific demography, accept function, individual model and simulation variables.
 * \param inDemography A handle to the demography.
 * \param inAcceptFunction A handle to the accept function tree.
 * \param inIndividualModel A handle to the individual model.
 * \param inSimulationVariables A handle to the simulation variables.
 */
GenProfile::GenProfile(Demography::Handle inDemography, Core::PrimitiveTree::Handle inAcceptFunction, IndividualModel::Handle inIndividualModel, SimulationVariables::Handle inSimulationVariables) :
	mDemography(inDemography),
	mAcceptFunction(inAcceptFunction),
	mIndividualModel(inIndividualModel),
	mSimulationVariables(inSimulationVariables)
{}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \param  A const reference to the system.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle GenProfile::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
		GenProfile::Handle lCopy = new GenProfile();

		// deep copy demography
		lCopy->mDemography = Core::castHandleT<Demography>(this->mDemography->deepCopy(inSystem));

		// deep copy accept function
		lCopy->mAcceptFunction = Core::castHandleT<Core::PrimitiveTree>(this->mAcceptFunction->deepCopy(inSystem));

		// deep copy individual model
		lCopy->mIndividualModel = new IndividualModel();
		for (IndividualModel::const_iterator lIt = this->mIndividualModel->begin(); lIt != this->mIndividualModel->end(); lIt++) {
			lCopy->mIndividualModel->insert((*lIt).c_str());
		}

		// deep copy simulator variables
		lCopy->mSimulationVariables = Core::castHandleT<SimulationVariables>(this->mSimulationVariables->deepCopy(inSystem));

		return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::GenProfile::Handle SCHNAPS::Simulation::GenProfile::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if a label attribute is missing.
 */
void GenProfile::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::GenProfile::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void GenProfile::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("label", mLabel);

	mDemography->write(ioStreamer, inIndent);

	ioStreamer.openTag("AcceptFunction", inIndent);
	mAcceptFunction->write(ioStreamer, inIndent);
	ioStreamer.closeTag();

	mIndividualModel->write(ioStreamer, inIndent);
	mSimulationVariables->write(ioStreamer, inIndent);
}
