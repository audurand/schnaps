/*
 * Primitive.cpp
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
 *  \brief Construct a primitive as a copy of another.
 *  \param inOriginal A const reference to the original primitive.
 */
Primitive::Primitive(const Primitive& inOriginal) :
	mNumberArguments(inOriginal.getNumberArguments())
{}

/*!
 *  \brief Construct a primitive.
 *  \param inNumberArguments Number of arguments of the primitive.
 */
Primitive::Primitive(unsigned int inNumberArguments) :
	mNumberArguments(inNumberArguments)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Primitive& Primitive::operator=(const Primitive& inOriginal) {
	schnaps_StackTraceBeginM();
	this->setNumberArguments(inOriginal.getNumberArguments());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Primitive& SCHNAPS::Core::Primitive::operator=(const SCHNAPS::Core::Primitive&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Primitive::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
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
	schnaps_StackTraceEndM("void SCHNAPS::Core::Primitive::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Primitive::writeContent(PACC::XML::Streamer& inStreamer, bool inIndent) const
{}

/*!
 * \brief  Test if an object is equal to another.
 * \param  inRightObject A reference to the object to compare with.
 * \return True if actual object is equal to right object, false if not.
 */
bool Primitive::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const Primitive& lRightPrimitive = castObjectT<const Primitive&> (inRightObj);
	if ((getName() != lRightPrimitive.getName()) || (mNumberArguments != lRightPrimitive.mNumberArguments)) {
		return false;
	}
	return true;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Primitive::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Validate primitive and children recursively.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return True if valid, false if not.
 */
bool Primitive::isValid(unsigned int inIndex, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	std::string lArgReturn, lArgType;
	unsigned int lArgIndex;
	for (unsigned int i = 0; i < mNumberArguments; i++) {
		lArgType = getArgType(inIndex, i, ioContext);
		lArgIndex = getArgumentIndex(inIndex, i, ioContext);
		lArgReturn = ioContext.getPrimitiveTree()[lArgIndex].mPrimitive->getReturnType(inIndex, ioContext);
		if ((lArgType != lArgReturn) && (!ioContext.getSystem().getTypingManager().typeCast(lArgReturn, lArgType))) {
			return false;
		}
	}
	return true;
	schnaps_StackTraceEndM("bool SCHNAPS::Core::Primitive::isValid(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
AnyType::Handle Primitive::execute(unsigned int inIndex, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("execute", "Primitive", getName());
	schnaps_StackTraceEndM("AnyType::Handle Primitive::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
const std::string& Primitive::getArgType(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("getArgType", "Primitive", getName());
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::Primitive::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
const std::string& Primitive::getReturnType(unsigned int inIndex, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("getReturnType", "Primitive", getName());
	schnaps_StackTraceEndM("const std::type_info* SCHNAPS::Core::Primitive::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the index of the Nth argument of the primitive.
 * \param  inIndex Index of the actual primitive in the primitive tree.
 * \param  inN Number of the argument to get the index.
 * \param  ioContext A reference to the execution context.
 * \return The index of the Nth argument of the primitive.
 * \throw  SCHNAPS::Core::AssertException if the argument number is out of bounds.
 */
unsigned int Primitive::getArgumentIndex(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, mNumberArguments-1);
	const PrimitiveTree& lActualTree = ioContext.getPrimitiveTree();
	unsigned int lNodeIndex = inIndex + 1;
	for (unsigned int i = 0; i < inN; i++) {
		lNodeIndex += lActualTree[lNodeIndex].mSubTreeSize;
	}
	return lNodeIndex;
	schnaps_StackTraceEndM("unsigned int SCHNAPS::Core::Primitive::getArgumentIndex(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return a handle to the value of the Nth argument of the primitive.
 * \param  inIndex Index of the actual primitive in the primitive tree.
 * \param  inN Number of the argument to get the index.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the value of the Nth argument of the primitive.
 */
AnyType::Handle Primitive::getArgument(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
	AnyType::Handle outResult = ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->execute(lNodeIndex, ioContext);
	return outResult;
	schnaps_StackTraceEndM("AnyType::Handle Primitive::getArgument(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
