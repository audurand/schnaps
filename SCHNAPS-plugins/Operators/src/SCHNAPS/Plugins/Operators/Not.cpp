/*
 * Not.cpp
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

#include "SCHNAPS/Plugins/Operators/Operators.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Operators;

/*!
 * \brief Default constructor.
 */
Not::Not() :
	Core::Primitive(0),
	mArg_Ref(""),
	mArg(NULL)
{}

/*!
 * \brief Construct a logical NOT operator as a copy of original.
 * \param inOriginal A const reference to the original logical NOT operator.
 */
Not::Not(const Not& inOriginal) :
	Core::Primitive(0),
	mArg_Ref(inOriginal.mArg_Ref.c_str())
{
	switch (mArg_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArg = NULL;
			break;
		case '$':
			// parameter value
			mArg = inOriginal.mArg;
			break;
		default:
			// direct value
			mArg = Core::castHandleT<Core::Bool>(inOriginal.mArg->clone());
	}
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inArgLeft or inArgRight attributes are missing.
 */
void Not::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve argument
	if (inIter->getAttribute("inArg").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "argument expected!");
	}
	mArg_Ref.assign(inIter->getAttribute("inArg"));
	
	switch (mArg_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mArg = NULL;
			break;
		case '$':
			// parameter value
			mArg = Core::castHandleT<Core::Bool>(ioSystem.getParameters().getParameterHandle(mArg_Ref.substr(1)));
			break;
		default:
			// direct value
			mArg = new Core::Bool();
			mArg->readStr(mArg_Ref);
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::Not::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Not::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inArg", mArg_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::Not::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::RunTimeException if the primitive is undefined for the specific argument source.
 */
Core::AnyType::Handle Not::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	bool lArg;
	
	if (mArg == NULL) {
		switch (mArg_Ref[0]) {
			case '@':
				// individual variable value
				lArg = Core::castObjectT<const Core::Bool&>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getIndividual().getState().getVariable(mArg_Ref.substr(1))).getValue();
				break;
			case '#':
				// environment variable value
				lArg = Core::castObjectT<const Core::Bool&>(Core::castObjectT<Simulation::ExecutionContext&>(ioContext).getEnvironment().getState().getVariable(mArg_Ref.substr(1))).getValue();
				break;
			case '%':
				// local variable value
				lArg = Core::castObjectT<const Core::Bool&>(Core::castObjectT<Simulation::SimulationContext&>(ioContext).getLocalVariable(mArg_Ref.substr(1))).getValue();
				break;
			default:
				throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific argument source.");
				break;
		}
	} else {
		// parameter value or direct value
		lArg = mArg->getValue();
	}
	
	return new Core::Bool(!lArg);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::Not::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Not::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Bool");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::Not::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
