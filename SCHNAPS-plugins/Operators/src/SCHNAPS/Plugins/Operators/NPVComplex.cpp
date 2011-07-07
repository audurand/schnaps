/*
 * NPVComplex.cpp
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
NPVComplex::NPVComplex() :
	Core::Primitive(1),
	mDiscountRate_Ref(""),
	mDiscountRate(NULL)
{}

/*!
 * \brief Construct a nearest present value operator with specific rate as a copy of an original.
 * \param inOriginal A const reference to the original  nearest present value operator with specific rate.
 */
NPVComplex::NPVComplex(const NPVComplex& inOriginal) :
	Core::Primitive(1),
	mDiscountRate_Ref(inOriginal.mDiscountRate_Ref.c_str())
{
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = inOriginal.mDiscountRate;
			break;
		default:
			// direct value
			mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
			break;
	}
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
NPVComplex& NPVComplex::operator=(const NPVComplex& inOriginal) {
	schnaps_StackTraceBeginM();
	mDiscountRate_Ref.assign(inOriginal.mDiscountRate_Ref.c_str());
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = inOriginal.mDiscountRate;
			break;
		default:
			// direct value
			mDiscountRate = Core::castHandleT<Core::Double>(inOriginal.mDiscountRate->clone());
			break;
	}
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::NPVComplex& SCHNAPS::Plugins::Operators::NPVComplex::operator=(const SCHNAPS::Plugins::Operators::NPVComplex&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inRate attribute is missing.
 */
void NPVComplex::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	if (inIter->getAttribute("inDiscountRate").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
	}
	mDiscountRate_Ref.assign(inIter->getAttribute("inDiscountRate"));
	
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDiscountRate = NULL;
			break;
		case '$':
			// parameter value
			mDiscountRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(mDiscountRate_Ref.substr(1)));
			break;
		default:
			// direct value
			mDiscountRate = new Core::Double(SCHNAPS::str2dbl(mDiscountRate_Ref));
			break;
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::NPVComplex::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void NPVComplex::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inDiscountRate", mDiscountRate_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::NPVComplex::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the primitive is not defined for the specific rate source.
 */
Core::AnyType::Handle NPVComplex::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	double lTime = lContext.getClock().getValue(SCHNAPS::Simulation::Clock::eYear);
	double lDiscountRate;
	Core::Double::Handle lDenum;
	
	switch (mDiscountRate_Ref[0]) {
		case '@':
			// individual variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getIndividual().getState().getVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		case '#':
			// environment variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getEnvironment().getState().getVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		case '%':
			// local variable value
			lDiscountRate = Core::castObjectT<const Core::Double&>(lContext.getLocalVariable(mDiscountRate_Ref.substr(1))).getValue();
			break;
		default:
			// parameter value or direct value
			lDiscountRate = mDiscountRate->getValue();
			break;
	}
	lDenum = new Core::Double(std::pow(lDiscountRate + 1, lTime));
	
	Core::Number::Handle lArg = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	
	return lArg->div(*lDenum);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Operators::NPVComplex::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the argument index is out of bounds.
 */
const std::string& NPVComplex::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, 0);
	const static std::string lType("Number");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NPVComplex::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& NPVComplex::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
	return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Operators::NPVComplex::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
