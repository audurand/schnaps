/*
 * NPV.cpp
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
NPV::NPV() :
	Core::Primitive(1),
	mRate_Ref(""),
	mRate(NULL)
{}

/*!
 * \brief Construct a nearest present value operator with specific rate as a copy of an original.
 * \param inOriginal A const reference to the original  nearest present value operator with specific rate.
 */
NPV::NPV(const NPV& inOriginal) :
	Core::Primitive(1),
	mRate_Ref(inOriginal.mRate_Ref.c_str())
{
	switch (mRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mRate = NULL;
			break;
		case '$':
			// parameter value
			mRate = inOriginal.mRate;
		default:
			// direct value
			mRate = Core::castHandleT<Core::Double>(inOriginal.mRate->clone());
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
NPV& NPV::operator=(const NPV& inOriginal) {
	schnaps_StackTraceBeginM();
	mRate_Ref.assign(inOriginal.mRate_Ref);
	switch (mRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mRate = NULL;
			break;
		case '$':
			// parameter value
			mRate = inOriginal.mRate;
		default:
			// direct value
			mRate = Core::castHandleT<Core::Double>(inOriginal.mRate->clone());
			break;
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::NPV& SCHNAPS::Plugins::Operators::NPV::operator=(const SCHNAPS::Plugins::Operators::NPV&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inRate attribute is missing.
 */
void NPV::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve actualization rate
	if (inIter->getAttribute("inRate").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
	}
	mRate_Ref.assign(inIter->getAttribute("inRate"));
	
	switch (mRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mRate = NULL;
			break;
		case '$':
			// parameter value
			mRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mRate_Ref.substr(1)));
		default:
			// direct value
			mRate = new Core::Double(SCHNAPS::str2dbl(mRate_Ref));
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::NPV::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void NPV::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inRate", mRate_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::NPV::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific context.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific rate source.
 */
Core::AnyType::Handle NPV::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	
	if (ioContext.getName() == "SimulationContext") {
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		double lTime = lContext.getClock().getValue();
		Core::Double::Handle lDenum;
		
		if (mRate == NULL) {
			double lRate;
			
			switch (mRate_Ref[0]) {
				case '@':
					// individual variable value
					lRate = Core::castHandleT<Core::Double>(lContext.getIndividual().getState().getVariableHandle(mRate_Ref.substr(1)))->getValue();
					break;
				case '#':
					// environment variable value
					lRate = Core::castHandleT<Core::Double>(lContext.getEnvironment().getState().getVariableHandle(mRate_Ref.substr(1)))->getValue();
					break;
				case '%':
					// TODO: local variable value
					break;
				default:
					throw schnaps_RunTimeExceptionM("The method is undefined for the specific rate source.");
					break;
			}
			lDenum = new Core::Double(std::pow(lRate + 1, lTime));	
		} else {
			// parameter value or direct value
			lDenum = new Core::Double(std::pow(mRate->getValue() + 1, lTime));	
		}
		
		lArg1->div(*lDenum);
		return lArg1;
	} else {
		throw schnaps_RunTimeExceptionM("NPV primitive is not defined for context '" + ioContext.getName() + "'!");
	}
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::NPV::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& NPV::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 0);
	const static std::string lType("Number");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NPV::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& NPV::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NPV::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
