/*
 * SetVariableComplex.cpp
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

#include "SCHNAPS/Plugins/Data/Data.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Data;

/*!
 * \brief Default constructor.
 */
SetVariableComplex::SetVariableComplex() :
	Core::Primitive(1),
	mVariable_Ref("")
{}

/*!
 * \brief Construct a primitive for setting complex variable value as a copy of an original.
 * \param inOriginal The original primitive for setting complex variable value.
 */
SetVariableComplex::SetVariableComplex(const SetVariableComplex& inOriginal) :
	Core::Primitive(1),
	mVariable_Ref(inOriginal.mVariable_Ref.c_str())
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void SetVariableComplex::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	
	// retrieve variable
	if (inIter->getAttribute("outVariable").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "variable to set expected!");
	}
	mVariable_Ref.assign(inIter->getAttribute("outVariable"));
	
	if (mVariable_Ref[0] != '@') {
		// TODO: store in local variable
		throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific variable source!");
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariableComplex::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void SetVariableComplex::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("outVariable", mVariable_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::SetVariableComplex::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the type of new value does not match the type of variable.
 */
Core::AnyType::Handle SetVariableComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	Core::AnyType::Handle lArg = getArgument(inIndex, 0, ioContext);

	std::string lTypeVariable = lContext.getIndividual().getState().getVariable(mVariable_Ref.substr(1)).getType();
	std::string lTypeArg = lArg->getType();
	if (lTypeVariable != lTypeArg) {
		std::stringstream lOSS;
		lOSS << "The type of variable '" << mVariable_Ref.substr(1) << "' (" << lTypeVariable << ") ";
		lOSS << "does not match the type of argument (" << lTypeArg << "); ";
		lOSS << "could not set the variable.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}

	lContext.getIndividual().getState().setVariable(mVariable_Ref.substr(1), lArg);
	return NULL;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::SetVariableComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is higher than 1.
 */
const std::string& SetVariableComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
	const static std::string lType("AnyType");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetVariableComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& SetVariableComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::SetVariableComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
