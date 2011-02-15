/*
 * Primitive.cpp
 *
 *  Created on: 2009-02-03
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

using namespace SCHNAPS;
using namespace Core;

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
 *  \brief Reading a primitive.
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
	schnaps_StackTraceEndM("void SCHNAPS::Core::Primitive::readWithSystem(PACC::XML::ConstIterator, Context&)");
}

void Primitive::writeContent(PACC::XML::Streamer& inStreamer, bool inIndent) const
{}

bool Primitive::isEqual(const Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const Primitive& lRightPrimitive = castObjectT<const Primitive&> (inRightObj);
		if ((getName() != lRightPrimitive.getName()) || (mNumberArguments != lRightPrimitive.mNumberArguments))
			return false;
		return true;
	schnaps_StackTraceEndM("bool Primitive::isEqual(const Object&) const");
}

Primitive& Primitive::operator=(const Primitive& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Primitive& SCHNAPS::Core::Primitive::operator=(const SCHNAPS::Core::Primitive&)");
}

/*!
 * \brief Validate primitive and children recursively.
 * \param inIndex Index of the current primitive.
 * \param ioContext Execution context.
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
	schnaps_StackTraceEndM("bool Primitive::isValid(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief  Execute the primitive.
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return The execution result.
 *  \throw  InternalException If the method is not overdefined is a subclass.
 */
AnyType::Handle Primitive::execute(unsigned int inIndex, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("execute", "Primitive", getName());
	schnaps_StackTraceEndM("AnyType::Handle Primitive::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the nth argument requested return type (for STGP).
 *  \param  inIndex Index of the current primitive.
 *  \param  inN Index of the argument to get the type.
 *  \param  ioContext Execution context.
 *  \return Type of the argument's node type.
 *  \throw  InternalException If the method is not overdefined is a subclass.
 */
const std::string& Primitive::getArgType(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("getArgType", "Primitive", getName());
	schnaps_StackTraceEndM("const std::type_info* SCHNAPS::Core::Primitive::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief  Return the primitive return type (for STGP).
 *  \param  inIndex Index of the current primitive.
 *  \param  ioContext Execution context.
 *  \return Type of the primitive return type.
 *  \throw  InternalException If the method is not overdefined is a subclass.
 */
const std::string& Primitive::getReturnType(unsigned int inIndex, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("getReturnType", "Primitive", getName());
	schnaps_StackTraceEndM("const std::type_info* SCHNAPS::Core::Primitive::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief Get index in the primitive tree of the Nth argument to actual primitive.
 *  \param inIndex Index of the actual primitive.
 *  \param inN Number of the argument to get the index from the primitive tree.
 *  \param ioContext Execution context.
 */
unsigned int Primitive::getArgumentIndex(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const PrimitiveTree& lActualTree = ioContext.getPrimitiveTree();
		unsigned int lNodeIndex = inIndex + 1;
		for (unsigned int i = 0; i < inN; i++) {
			lNodeIndex += lActualTree[lNodeIndex].mSubTreeSize;
		}
		return lNodeIndex;
	schnaps_StackTraceEndM("unsigned int SCHNAPS::Core::Primitive::getArgumentIndex(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 *  \brief Get the value of the nth argument.
 *  \param inIndex Index of the actual primitive.
 *  \param inN Number of the argument to get the index from the primitive tree.
 *  \param ioContext Execution context.
 *  \return Value of the nth argument.
 *  \throw AssertException If the argument index is invalid.
 */
AnyType::Handle Primitive::getArgument(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		schnaps_UpperBoundCheckAssertM(inN, mNumberArguments-1);
		unsigned int lNodeIndex = getArgumentIndex(inIndex, inN, ioContext);
		AnyType::Handle outResult = ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->execute(lNodeIndex, ioContext);
		return outResult;
	schnaps_StackTraceEndM("AnyType::Handle Primitive::getArgument(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
