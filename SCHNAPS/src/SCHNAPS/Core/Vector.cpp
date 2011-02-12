/*
 * Vector.cpp
 *
 *  Created on: 2010-03-22
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

Vector::Vector(const Vector& inOriginal) {
	this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
}

bool Vector::isEqual(const Object&) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("isEqual","Vector",getName());
	schnaps_StackTraceEndM("bool core::Vector::isEqual(const core::Object&) const");
}

bool Vector::isLess(const Object&) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("isLess","Vector",getName());
	schnaps_StackTraceEndM("bool core::Vector::isLess(const core::Object&) const");
}

Vector& Vector::operator=(const Vector& inOriginal) {
	schnaps_StackTraceBeginM();
		this->insert(this->begin(), inOriginal.begin(), inOriginal.end());
		return *this;
	schnaps_StackTraceEndM("core::Vector& core::Vector::operator=(const core::Vector&)");
}

void Vector::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
		std::istringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters("|", "");

		std::string lValue;

		unsigned int i = 0;
		while (lTokenizer.getNextToken(lValue)) {
#ifndef SIMULATOR_NDEBUG
			if (i == this->size()) {
				std::ostringstream lOSS;
				lOSS << "The number of values in vector reading must be equal to the current size of vector(" << this->size() << ")!";
				throw schnaps_InternalExceptionM(lOSS.str().c_str());
			}
#else
			schnaps_AssertM(i < this->size());
#endif
			(*this)[i]->readStr(lValue);
			i++;
		}
#ifndef SIMULATOR_NDEBUG
			if (i != this->size()) {
				std::ostringstream lOSS;
				lOSS << "The number of values in vector reading must be equal to the current size of vector(" << this->size() << "!";
				throw schnaps_InternalExceptionM(lOSS.str().c_str());
			}
#else
			schnaps_AssertM(i == this->size());
#endif
	schnaps_StackTraceEndM("void core::Vector::readStr(std::string&)");
}

std::string Vector::writeStr() const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		unsigned int i;
		for (i = 0; i < this->size()-1; i++) {
			lOSS << (*this)[i]->writeStr() << "|";
		}
		lOSS << (*this)[i]->writeStr();
		return lOSS.str();
	schnaps_StackTraceEndM("std::string core::Vector::writeStr() const");
}

void Vector::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
				lAlloc = core::castHandleT<core::AnyType::Alloc>(ioSystem.getFactory().getAllocator(lChild->getValue()));
				if (lAlloc == NULL) {
					std::ostringstream lOSS;
					lOSS << "no element named '" <<  lChild->getValue();
					lOSS << "' found in the factory";
					throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
				}
				this->push_back(core::castHandleT<core::AnyType>(lAlloc->allocate()));
				this->back()->readWithSystem(lChild, ioSystem);
			}
		}
	schnaps_StackTraceEndM("void core::Vector::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Vector::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		for (unsigned int i = 0; i < this->size(); i++) {
			(*this)[i]->write(ioStreamer, inIndent);
		}
	schnaps_StackTraceEndM("void core::Vector::writeContent(PACC::XML::Streamer&, bool) const");
}
