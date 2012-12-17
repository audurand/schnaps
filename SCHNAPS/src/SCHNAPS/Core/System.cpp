/*
 * System.cpp
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
 *  \brief Construct a new system with the default basic components.
 */
System::System() :
	mFactory(new Factory()),
	mParameters(new Parameters()),
	mRandomizers(new RandomizerMulti()),
	mLoggers(new LoggerMulti()),
	mTypingManager(new TypingManager()),
	mPlugins(new Plugins())
{
    schnaps_StackTraceBeginM();
	// install basic components
	addComponent(mFactory);
	addComponent(mParameters);
	addComponent(mRandomizers);
	addComponent(mLoggers);
	addComponent(mTypingManager);
	addComponent(mPlugins);
    schnaps_StackTraceEndM("SCHNAPS::Core::System::System()");

}

/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if trying to read unknown components.
 */
void System::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if ((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="System")) {
		throw schnaps_IOExceptionNodeM(*inIter, "invalid node type or tag name!");
	}

	Component::Alloc::Handle lAlloc;

	// read all components.
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; ++lChild) {
		if (lChild->getType() == PACC::XML::eData) {
			if (find(lChild->getValue()) == end()) {
				lAlloc = castHandleT<Component::Alloc>(mFactory->getAllocator(lChild->getValue()));
				if (lAlloc == NULL) {
					throw schnaps_IOExceptionNodeM(*lChild, "WARNING: unknown component named '" + lChild->getValue() + "'; not read!");
				}
				this->addComponent(castHandleT<Component>(lAlloc->allocate()));
			}
			(*this)[lChild->getValue()]->readWithSystem(lChild, *this);
		}
	}

	initComponents();
	schnaps_StackTraceEndM("void SCHNAPS::Core::System::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void System::writeContent(PACC::XML::Streamer& outStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	for (System::const_iterator lItr = begin(); lItr != end(); ++lItr) {
		const Component::Handle lComponent = castHandleT<const Component>(lItr->second);
		lComponent->write(outStreamer, inIndent);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::System::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Test if an object is equal to another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is equal to right object, false if not.
 */
bool System::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const System& lRightSystem = castObjectT<const System&>(inRightObj);
	if (this->size() != lRightSystem.size()) {
		return false;
	}
	return std::equal(this->begin(), this->end(), lRightSystem.begin(), IsEqualMapPairPredicate<std::string>());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::System::isEqual(const Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is less than right object, false if not.
 */
bool System::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const System& lRightSystem = castObjectT<const System&>(inRightObj);
	if (this->size() < lRightSystem.size()) {
		return true;
	}
	return false;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::System::isLess(const Object&) const");
}

/*!
 * \brief Add a new component to the system.
 * \param inComponent A handle to the component to add.
 * \throw SCHNAPS::Core::ObjectException if component is already in system.
 */
void System::addComponent(Component::Handle inComponent) {
	schnaps_StackTraceBeginM();
	// check that component is valid and not yet added
	schnaps_NonNullPointerAssertM(inComponent);
	if (find(inComponent->getName()) != end()) {
		throw schnaps_ObjectExceptionM("A component named '" + inComponent->getName() + "' is already installed in the system!");
	}

	// add component
	(*this)[inComponent->getName()] = inComponent;
	schnaps_StackTraceEndM("void SCHNAPS::Core::System::addComponent(SCHNAPS::Core::Component::Handle)");
}

/*!
*  \brief Initialize the components of system.
 */
void System::initComponents() {
	schnaps_StackTraceBeginM();
	for (iterator lItr = begin(); lItr != end(); ++lItr) {
		Component::Handle lComponent = castHandleT<Component>(lItr->second);
		lComponent->init(*this);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::System::initComponents()");
}
