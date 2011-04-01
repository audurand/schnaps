/*
 * DecisionNode.cpp
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

#include "SCHNAPS/Plugins/Learning/Learning.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

/*!
 * \brief Default constructor.
 */
DecisionNode::DecisionNode() :
	Primitive(), // unknown number of children
	mLabel("")
{}

/*!
 * \brief Construct a DecisionNode primitive as a copy of an original.
 * \param inOriginal A const reference to the original DecisionNode primitive.
 */
DecisionNode::DecisionNode(const DecisionNode& inOriginal) :
	Primitive(inOriginal.getNumberArguments()),
	mLabel(inOriginal.mLabel.c_str())
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
DecisionNode& DecisionNode::operator=(const DecisionNode& inOriginal) {
	schnaps_StackTraceBeginM();
	setNumberArguments(inOriginal.getNumberArguments());
	mLabel.assign(inOriginal.mLabel.c_str());
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Learning::DecisionNode& SCHNAPS::Plugins::Learning::DecisionNode::operator=(const SCHNAPS::Plugins::Learning::DecisionNode&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inLabel attribute is missing.
 */
void DecisionNode::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve label
	if (inIter->getAttribute("inLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "DecisionNode label expected!");
	}
	mLabel.assign(inIter->getAttribute("inLabel"));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionNode::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void DecisionNode::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inLabel", mLabel);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::DecisionNode::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle DecisionNode::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	// call the learning module to get the index of action to execute
	unsigned int lActionIndex = Core::castObjectT<LearningModule&>(ioContext.getComponent("LearningModule")).getActionID(mLabel, lContext);
	return getArgument(inIndex, lActionIndex, ioContext);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Learning::DecisionNode::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the index of argument is out of bounds.
 */
const std::string& DecisionNode::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::DecisionNode::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& DecisionNode::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	std::string lCommonType = getArgType(inIndex, 0, ioContext);

	for (unsigned int i = 1; i < getNumberArguments(); i++) {
		lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
	}

	const static std::string lCommonType_Final(lCommonType);
	return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::DecisionNode::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
