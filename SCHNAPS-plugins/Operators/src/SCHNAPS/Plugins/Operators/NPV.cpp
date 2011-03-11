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
	mRate = Core::castHandleT<Core::Double>(inOriginal.mRate->clone());
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
NPV& NPV::operator=(const NPV& inOriginal) {
	schnaps_StackTraceBeginM();
	mRate_Ref = inOriginal.mRate_Ref.c_str();
	mRate = Core::castHandleT<Core::Double>(inOriginal.mRate->clone());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Operators::NPV& SCHNAPS::Plugins::Operators::NPV::operator=(const SCHNAPS::Plugins::Operators::NPV&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if rate attribute and rate.ref attribute are missing.
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

	if (inIter->getAttribute("rate").empty()) {
		if (inIter->getAttribute("rate.ref").empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "discount rate expected!");
		} else {
			mRate_Ref = inIter->getAttribute("rate.ref");
			std::stringstream lSS;
			lSS << "ref." << mRate_Ref;
			mRate = Core::castHandleT<Core::Double>(ioSystem.getParameters().getParameterHandle(lSS.str().c_str()));
		}
	} else {
		mRate = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("rate")));
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
	if (mRate_Ref.empty()) {
		ioStreamer.insertAttribute("rate", mRate->writeStr());
	} else {
		ioStreamer.insertAttribute("rate.ref", mRate_Ref);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Operators::NPV::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is not defined for the specific context.
 */
Core::AnyType::Handle NPV::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::Number::Handle lArg1 = Core::castHandleT<Core::Number>(getArgument(inIndex, 0, ioContext));
	if (ioContext.getName() == "SimulationContext") {
		Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
		double lTime = lContext.getClock().getValue();
		Core::Double lDenum(std::pow(mRate->getValue() + 1, lTime));
		lArg1->div(lDenum);
		return lArg1;
	} else if (ioContext.getName() == "GenerationContext") {
		Simulation::GenerationContext& lContext = Core::castObjectT<Simulation::GenerationContext&>(ioContext);
		double lTime = lContext.getClock().getValue();
		Core::Double lDenum(std::pow(mRate->getValue() + 1, lTime));
		lArg1->div(lDenum);
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
