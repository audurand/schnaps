/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

#include "SCHNAPS/Core.hpp"

using namespace core;

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
		// Install basic components
		addComponent(mFactory);
		addComponent(mParameters);
		addComponent(mRandomizers);
		addComponent(mLoggers);
		addComponent(mTypingManager);
		addComponent(mPlugins);
    schnaps_StackTraceEndM("core::System::System()");

}

void System::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		if ((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="System")) {
			throw schnaps_IOExceptionNodeM(*inIter, "invalid node type or tag name!");
		}

		Component::Alloc::Handle lAlloc;

		// Read all components.
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; ++lChild) {
			if (lChild->getType() == PACC::XML::eData) {
				if (find(lChild->getValue()) == end()) {
					lAlloc = castHandleT<Component::Alloc>(mFactory->getAllocator(lChild->getValue()));
					if (lAlloc == NULL) {
						throw schnaps_ObjectExceptionM("WARNING: unknown component named '" +
								lChild->getValue() +
								"'; not read!");
					}
					this->addComponent(castHandleT<Component>(lAlloc->allocate()));
				}
				(*this)[lChild->getValue()]->readWithSystem(lChild, *this);
			}
		}

		initComponents();
	schnaps_StackTraceEndM("void core::System::read(PACC::XML::ConstIterator)");
}

void System::writeContent(PACC::XML::Streamer& outStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		for (System::const_iterator lItr = begin(); lItr != end(); ++lItr) {
			const Component::Handle lComponent = castHandleT<const Component>(lItr->second);
			lComponent->write(outStreamer, inIndent);
		}
	schnaps_StackTraceEndM("void core::System::writeContent(PACC::XML::Streamer&, bool) const");
}

bool System::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const System& lRightSystem = castObjectT<const System&>(inRightObj);
		if (this->size() != lRightSystem.size()) {
			return false;
		}
		return std::equal(this->begin(), this->end(), lRightSystem.begin(), IsEqualMapPairPredicate<std::string>());
	schnaps_StackTraceEndM("bool core::System::isEqual(const Object&) const");
}

bool System::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const System& lRightSystem = castObjectT<const System&>(inRightObj);
		if (this->size() < lRightSystem.size()) {
			return true;
		}
		return false;
	schnaps_StackTraceEndM("bool core::System::isLess(const Object&) const");
}

/*!
 *  \brief Add a new component to the system.
 *  \param inComponent The component to add.
 *
 *  To be initialized, components need to be added to the system.
 */
void System::addComponent(Component::Handle inComponent) {
	schnaps_StackTraceBeginM();
		// Check that component is valid and not yet added
		schnaps_NonNullPointerAssertM(inComponent);
		if (find(inComponent->getName()) != end()) {
				throw schnaps_ObjectExceptionM("A component named '" +
						inComponent->getName() +
						"' is already installed in the system!");
		}

		// Add component
		(*this)[inComponent->getName()] = inComponent;
	schnaps_StackTraceEndM("void core::System::addComponent(core::Component::Handle)");
}

/*!
*  \brief Initialize the components of this system.
 */
void System::initComponents() {
	schnaps_StackTraceBeginM();
		for (iterator lItr = begin(); lItr != end(); ++lItr) {
				Component::Handle lComponent = castHandleT<Component>(lItr->second);
				if (lComponent->isInitialized() == false) {
						lComponent->init(*this);
						lComponent->setInitializedFlag(true);
				}
		}
	schnaps_StackTraceEndM("void core::System::initComponents()");
}
