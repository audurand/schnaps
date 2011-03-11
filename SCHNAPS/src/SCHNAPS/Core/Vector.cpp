/*
 * Vector.cpp
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
 * \brief Construct a vector as a copy of the right vector.
 * \param inOrignal Original vector instance.
 */
Vector::Vector(const Vector& inOriginal) {
	this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
Vector& Vector::operator=(const Vector& inOriginal) {
	schnaps_StackTraceBeginM();
	this->clear();
	this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Vector& SCHNAPS::Core::Vector::operator=(const SCHNAPS::Core::Vector&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 */
void Vector::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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

	this->clear();

	AnyType::Alloc::Handle lAlloc;
	AnyType::Handle lElement;
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			lAlloc = SCHNAPS::Core::castHandleT<SCHNAPS::Core::AnyType::Alloc>(ioSystem.getFactory().getAllocator(lChild->getValue()));
			if (lAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "no element named '" <<  lChild->getValue();
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			this->push_back(SCHNAPS::Core::castHandleT<SCHNAPS::Core::AnyType>(lAlloc->allocate()));
			this->back()->readWithSystem(lChild, ioSystem);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Vector::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Vector::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < this->size(); i++) {
		(*this)[i]->write(ioStreamer, inIndent);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Vector::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Test if an object is equal to another.
 * \param inRightObj Object to compare with.
 * \return True if objects are equal.
 */
bool Vector::isEqual(const Object&) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("isEqual","Vector",getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Vector::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Test if an object is less than another.
 * \param inRightObj Object to compare with.
 * \return True if current object is less than right object.
 */
bool Vector::isLess(const Object&) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("isLess","Vector",getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Vector::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Read the object from string.
 * \param inStr String to read the object from.
 * \throw SCHNAPS::Core::InternalException because the method is undefined.
 */
void Vector::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("readStr","Vector",getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Vector::readStr(std::string&)");
}

/*!
 * \brief Write the object to string.
 * \return The object as string.
 */
std::string Vector::writeStr() const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	unsigned int i;
	for (i = 0; i < this->size()-1; i++) {
		lOSS << (*this)[i]->writeStr() << "|";
	}
	lOSS << (*this)[i]->writeStr();
	return lOSS.str();
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Vector::writeStr() const");
}

/*!
 * \brief Return a handle to a clone (deep copy).
 * \return A handle to a clone (deep copy).
 */
AnyType::Handle Vector::clone() const {
	schnaps_StackTraceBeginM();
	Vector::Handle lCopy = new Vector();
	for (unsigned int i = 0; i < this->size(); i++) {
		lCopy->push_back((*this)[i]->clone());
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Core::Vector::clone() const");
}
