/*
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

/*!
 *  \file   SCHNAPS/Core/Object.cpp
 *  \brief  Source code of class Object.
 */

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;

/*!
 *  \brief Default constructor.
 */
Object::Object() :
	mRefCounter(0)
{}

/*!
 *  \brief Copy constructor.
 */
Object::Object(const Object&) :
	mRefCounter(0)
{}

/*!
 *  \brief Destructor.
 *  \throw SCHNAPS::Core::InternalException If reference counter is not equal to zero.
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
 *  \brief Returns the name of the object, in this case "Non-named Object".
 */
const std::string& Object::getName() const {
	schnaps_StackTraceBeginM();
		const static std::string mName("Non-named Object");
		return mName;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Object::getName() const");
}

void Object::readWithSystem(PACC::XML::ConstIterator InIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("readWithSystem", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 *  \brief Extract Object from a XML tree.
 *  \throw SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
void Object::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("read", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::read(PACC::XML::ConstIterator)");
}

/*!
 *  \brief Insert an Object into a XML streamer.
 */
void Object::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag(getName());
		writeContent(ioStreamer, inIndent);
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::write(PACC::XML::Streamer&, bool) const");
}

/*!
 *  \brief Insert an Object content into a XML streamer.
 *  \throw  SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
void Object::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("writeContent", "Object", getName());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Object::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 *  \brief  Test if an object is equal to another.
 *  \return True if actual object is equal to right object, false if not.
 *  \throw  SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("isEqual", "Object", getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Object::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 *  \brief  Test if an object is less than another.
 *  \return True if actual object is less than right object, false if not.
 *  \throw  SCHNAPS::Core::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isLess(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("isLess", "Object", getName());
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Object::isLess(const SCHNAPS::Core::Object&) const");
}

/*!
 *  \brief Copy operator of Object.
 *  \return Actual object.
 */
Object& Object::operator=(const Object& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object& SCHNAPS::Core::Object::operator=(const SCHNAPS::Core::Object&)");
}
