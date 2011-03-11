/*
 * Object.cpp
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
 * \brief Default constructor.
 */
Object::Object() :
	mRefCounter(0)
{}

/*!
 * \brief Copy constructor.
 */
Object::Object(const Object&) :
	mRefCounter(0)
{}

/*!
 * \brief Destructor.
 * \throw SCHNAPS::Core::InternalException if reference counter is not equal to zero.
 */
Object::~Object() {
	schnaps_StackTraceBeginM();
#ifndef SCHNAPS_NDEBUG
	if (mRefCounter != 0) {
		std::string lMessage = "Reference counter of actual object is non-zero (value: ";
		lMessage += int2str(mRefCounter);
		lMessage += ")";
		throw schnaps_InternalExceptionM(lMessage);
	}
#endif // SCHNAPS_NDEBUG
	schnaps_StackTraceEndM("Object::~Object()");
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Object& Object::operator=(const Object& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object& SCHNAPS::Core::Object::operator=(const SCHNAPS::Core::Object&)");
}
	
/*!
 * \brief  Return a handle to a deep copy of the object.
 * \param  inSystem A const reference to the system.
 * \return A handle to a deep copy of the object.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Object::Handle Object::deepCopy(const System& inSystem) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("deepCopy", "Object", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Core::Object::deepCopy(const SCHNAPS::Core::System&) const");
}

/*!
 * \brief  Return a const reference to the name of object.
 * \return A const reference to the name of object.
 */
const std::string& Object::getName() const {
	schnaps_StackTraceBeginM();
	const static std::string mName("Non-named Object");
	return mName;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Object::getName() const");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
void Object::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("readWithSystem", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
void Object::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("read", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Object::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag(getName());
	writeContent(ioStreamer, inIndent);
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::write(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 * \throw SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
void Object::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("writeContent", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObject A reference to the object to compare with.
 * \return True if actual object is equal to right object, false if not.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
bool Object::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("isEqual", "Object", getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Object::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param  inRightObject A reference to the object to compare with.
 * \return True if actual object is less than right object, false if not.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
bool Object::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("isLess", "Object", getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Object::isLess(const SCHNAPS::Core::Object&) const");
}
