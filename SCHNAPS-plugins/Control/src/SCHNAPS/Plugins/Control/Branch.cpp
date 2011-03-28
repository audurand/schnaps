/*
 * Branch.cpp
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 * \brief Default constructor.
 */
Branch::Branch() :
	Primitive(2),
	mProbability_Ref(""),
	mProbability(NULL)
{}

/*!
 * \brief Construct a branching primitive as a copy of an original.
 * \param inOriginal A const reference to the original branching primitive.
 */
Branch::Branch(const Branch& inOriginal) :
		mProbability_Ref(inOriginal.mProbability_Ref.c_str())
{
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbability = NULL;
			break;
		case '$':
			// parameter value
			mProbability = inOriginal.mProbability;
			break;
		default:
			// direct value
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
Branch& Branch::operator=(const Branch& inOriginal) {
	schnaps_StackTraceBeginM();
	mProbability_Ref.assign(inOriginal.mProbability_Ref);
	
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbability = NULL;
			break;
		case '$':
			// parameter value
			mProbability = inOriginal.mProbability;
			break;
		default:
			// direct value
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
			break;
	}

	return *this;
	schnaps_StackTraceEndM("Core::Branch& SCHNAPS::Plugins::Control::Branch::operator=(const SCHNAPS::Core::Branch&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inProbability attribute is missing.
 */
void Branch::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve probability of executing the first branch
	if (inIter->getAttribute("inProbability").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "probability of executing first branch expected!");
	}
	mProbability_Ref = inIter->getAttribute("inProbability");
	
	switch (mProbability_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbability = NULL;
			break;
		case '$':
			// parameter value
			mProbability = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mProbability_Ref.substr(1)));
			break;
		default:
			// direct value
			mProbability = new Core::Double();
			mProbability->readStr(mProbability_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Branch::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Branch::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inProbability", mProbability_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Branch::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw SCHNAPS::Core::RunTimeException if the primitive is undefined for the specific probability source.
 */
Core::AnyType::Handle Branch::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	double lRandom = ioContext.getRandomizer().rollUniform();
	double lProbability;
	
	if (mProbability == NULL) {
		switch (mProbability_Ref[0]) {
			case '@': {
				// individual variable value
				Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
				lProbability = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mProbability_Ref.substr(1))).getValue();
				break; }
			case '#': {
				// environment variable value
				Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
				lProbability = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mProbability_Ref.substr(1))).getValue();
				break; }
			case '%':
				// TODO: local variable value
				break;
			default:
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific probability source!");
				break;
		}
	} else {
		// parameter value or direct value
		lProbability = mProbability->getValue();
	}
	
	if (lRandom < lProbability) {
		// first branch
		return getArgument(inIndex, 0, ioContext);
	} else {
		// second branch
		return getArgument(inIndex, 1, ioContext);
	}
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::Branch::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the index of argument is out of bounds.
 */
const std::string& Branch::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Branch::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Branch::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));
	return lCommonType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Branch::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
