/*
 * Component.cpp
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

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Component class constructor.
 * \param inName Name of the component.
 */
Component::Component(std::string inName) :
	mName(inName)
{}

/*!
 * \brief Initialize this component.
 * \param ioSystem A reference to the system.
 */
void Component::init(System& ioSystem)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
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
	schnaps_StackTraceEndM("void SCHNAPS::Core::Component::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}
