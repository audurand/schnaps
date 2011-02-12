/*
 * Component.cpp
 *
 *  Created on: 2009-01-23
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

using namespace core;


/*!
 *  \brief Component class constructor.
 *  \param inName Name of the component.
 */
Component::Component(std::string inName) : mName(inName), mInitialized(false)
{}

/*!
 *  \brief Initialize this component.
 */
void Component::init(System&) {
	this->setInitializedFlag(true);
}

/*!
 *  \brief Read the system component.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 */
void Component::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
		if(inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if(inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}
	schnaps_StackTraceEndM("void Component::readWithSystem(PACC::XML::ConstIterator, System&)");
}
