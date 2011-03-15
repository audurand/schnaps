/*
 * Variable.cpp
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
Variable::Variable() :
	Primitive(0)
{}

/*!
 * \brief Construct a primitive for accessing a variable from the current individual as a copy of an original.
 * \param inOriginal A const reference to the original primitive for accessing a variable from the current individual.
 */
Variable::Variable(const Variable& inOriginal) :
	Primitive(0),
	mLabel(inOriginal.mLabel.c_str())
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 */
void Variable::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of the referenced variable expected!");
	}
	mLabel = inIter->getAttribute("label");
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Variable::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Variable::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Data::Variable::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is executed with a Simulation::GenerationContext and the variable is not defined either in demography and simulation variables.
 */
Core::AnyType::Handle Variable::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::AnyType::Handle lVariable;
	if (ioContext.getName() == "GenerationContext") {
		Simulation::GenerationContext& lContext = Core::castObjectT<Simulation::GenerationContext&>(ioContext);
		if (lContext.getIndividual().getState().hasVariable(mLabel) == false) {
			// intialize the variable before continuing
			
			// save current primitive tree
			Core::PrimitiveTree::Handle lCurrentPrimitiveTree = lContext.getPrimitiveTreeHandle();
			
			if (lContext.getGenProfile().getDemography().hasVariable(mLabel) == false) {
				// variable not in demography, check in simulation variables
				if (lContext.getGenProfile().getSimulationVariables().hasVariable(mLabel)) {
					// variable not in simulation variables either, throw error
					throw schnaps_RunTimeExceptionM("Variable " + mLabel + " is empty for current individual and is not contained in demography nor in simulation variables.");
				} else {
					// variable is in simulation variables
					lVariable = lContext.getGenProfile().getSimulationVariables().getVariableInitTree(mLabel).interpret(ioContext);
				}
			} else {
				// variable is in demography
				lVariable = lContext.getGenProfile().getDemography().getVariableInitTree(mLabel).interpret(ioContext);
			}
			// add newly computed variable to individual
			lContext.getIndividual().getState().insertVariable(mLabel, Core::castHandleT<Core::Atom>(lVariable->clone()));
			
			// restore primitive tree
			lContext.setPrimitiveTree(lCurrentPrimitiveTree);
		} else {
			lVariable = lContext.getIndividual().getState().getVariable(mLabel).clone();
		}
	} else {
		Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
		lVariable = lContext.getIndividual().getState().getVariable(mLabel).clone();
	}
	return lVariable;
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Data::Variable::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& Variable::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::ExecutionContext& lContext = Core::castObjectT<Simulation::ExecutionContext&>(ioContext);
	return lContext.getIndividual().getState().getVariable(mLabel).getType();
	// TODO: this validation needs an individual loaded... validation should be possible without any individual in context.
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Variable::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
