/*
 * Container.cpp
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

#include <algorithm>

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Container constructor.
 * \param inN Initial number of elements.
 * \param inModel Model of initial elements.
 */
Container::Container(unsigned int inN, Object::Handle inModel) :
	std::vector<Object::Handle>(inN,inModel)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Container::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	if ((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName())) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag <"+getName()+"> expected!");
	}

	Object::Alloc::Handle lObjectAlloc;
	Object::Handle lObjectHandle;
	std::string lObjectName;

	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			lObjectName = lChild->getValue();
			lObjectAlloc = castHandleT<Object::Alloc>(ioSystem.getFactory().getAllocator(lObjectName));
			try {
				lObjectHandle = castHandleT<Object>(lObjectAlloc->allocate());
			} catch(BadCastException&) {
				std::ostringstream lOSS;
				lOSS << "Tag '" << lObjectName << "' is not a possible object!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			lObjectHandle->readWithSystem(lChild, ioSystem);
			push_back(lObjectHandle);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Container::readWithSystem(PACC::XML::ConstIterator, System&)");
}

/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if wrong tag is used.
 */
void Container::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Bag"))
		throw schnaps_IOExceptionNodeM(*inIter, "tag <Bag> expected!");
	PACC::XML::ConstIterator lChild=inIter->getFirstChild();
	unsigned int lSize;
	for(lSize=0; lChild; ++lSize) ++lChild;
	if(lSize!=size()) {
		std::ostringstream lOSS;
		lOSS << "number of elements found in XML (" << lSize;
		lOSS << ") mismatch real size of container (" << size() << ")!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}
	lChild = inIter->getFirstChild();
	for(unsigned int j=0; lChild; ++j) {
		if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="NullHandle")) {
			(*this)[j] = NULL;
		} else {
			schnaps_NonNullPointerAssertM((*this)[j]);
			(*this)[j]->read(lChild);
		}
		++lChild;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Container::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Container::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag(getName(), inIndent);
	for(unsigned int i=0; i<size(); ++i) {
		if((*this)[i] == NULL) {
			ioStreamer.openTag("NullHandle");
			ioStreamer.closeTag();
		}
		else {
			(*this)[i]->write(ioStreamer, inIndent);
		}
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Core::Container::write(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Test if an object is equal to another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is equal to right object, false if not.
 * \throw SCHNAPS::Core::BadCastException If compared containers are not of the same type.
 * \par Note:
 *    Returns false if the container does not have the same size, returns true if the
 *    range [ 0, size()) are identical when compared element-by-element, and otherwise returns false.
 */
bool Container::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
	if(size() != lRightContainer.size()) return false;
	Container::const_iterator lFirstIter1 = begin();
	Container::const_iterator lLastIter1  = begin() + size();
	Container::const_iterator lFirstIter2 = lRightContainer.begin();
	return std::equal(lFirstIter1, lLastIter1, lFirstIter2, IsEqualPointerPredicate());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Container::isEqual(const Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is less than right object, false if not.
 * \throw SCHNAPS::Core::BadCastException If compared containers are not of the same type.
 * \par Note:
 *    Returns true if the actual object is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool Container::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
	unsigned int lSizeCompared = minOf<unsigned int>(size(), lRightContainer.size());
	Container::const_iterator lFirstIter1 = begin();
	Container::const_iterator lLastIter1  = begin() + lSizeCompared;
	Container::const_iterator lFirstIter2 = lRightContainer.begin();
	Container::const_iterator lLastIter2  = lRightContainer.begin() + lSizeCompared;
	return std::lexicographical_compare(lFirstIter1,
										lLastIter1,
										lFirstIter2,
										lLastIter2,
										IsLessPointerPredicate());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Container::isLess(const Object&) const");
}
