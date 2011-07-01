/*
 * Individual.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Construct a new individual.
 * \param inID The individual ID.
 */
Individual::Individual(std::string inID) :
		mID(inID),
		mActive(true)
{}

/*!
 * \brief Construct an individual as a copy of an original.
 * \param inOriginal A const reference to the original individual.
 */
Individual::Individual(const Individual& inOriginal) :
		mID(inOriginal.mID.c_str()),
		mState(inOriginal.mState),
		mActive(true)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if variable id attribute is missing.
 */
void Individual::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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
		if (inIter->getAttribute("id").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "individual ID expected!");
		}

		mID = inIter->getAttribute("id");
		mState.readWithSystem(inIter->getFirstChild(), ioSystem);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Individual::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Individual::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	ioStreamer.insertAttribute("id", mID);
	mState.write(ioStreamer, inIndent);
}

/*!
 * \brief Print individual to file stream.
 * \param ioStream A reference to the file stream.
 * \param inVariables A const reference to the labels of variables to print.
 */
void Individual::print(std::ostream& ioStream, const std::vector<std::string> inVariables) const {
	ioStream << mID;
	mState.print(ioStream, inVariables);
	ioStream << std::endl;
}
