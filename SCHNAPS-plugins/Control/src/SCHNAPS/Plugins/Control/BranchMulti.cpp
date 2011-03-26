/*
 * BranchMulti.cpp
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
BranchMulti::BranchMulti() :
	Primitive(),	// unknown number of children
	mProbabilities_Ref(""),
	mProbabilities(NULL)
{}

/*!
 * \brief Construct a multiple branch primitive as a copy of an original.
 * \param inOriginal A const reference to the original multiple branch primitive.
 */
BranchMulti::BranchMulti(const BranchMulti& inOriginal) :
		mProbabilities_Ref(inOriginal.mProbabilities_Ref.c_str())
{
	switch (mProbabilities_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbabilities = NULL;
			break;
		case '$':
			// parameter value
			mProbabilities = inOriginal.mProbabilities;
			break;
		default:
			// direct value
			mProbabilities = Core::castHandleT<Core::Vector>(inOriginal.mProbabilities->clone());
			break;
	}
	setNumberArguments(inOriginal.getNumberArguments());
}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
BranchMulti& BranchMulti::operator=(const BranchMulti& inOriginal) {
	schnaps_StackTraceBeginM();
	mProbabilities_Ref.assign(inOriginal.mProbabilities_Ref);
	
	switch (mProbabilities_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// TODO: local variable value
			mProbabilities = NULL;
			break;
		case '$':
			// parameter value
			mProbabilities = inOriginal.mProbabilities;
			break;
		default:
			// direct value
			mProbabilities = Core::castHandleT<Core::Vector>(inOriginal.mProbabilities->clone());
			break;
	}
	setNumberArguments(inOriginal.getNumberArguments());

	return *this;
	schnaps_StackTraceEndM("Core::BranchMulti& SCHNAPS::Plugins::Control::BranchMulti::operator=(const SCHNAPS::Core::BranchMulti&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if inProbabilities attribute is missing.
 * \throw SCHNAPS::Core::RunTimeException if the primitive is undefined for the specific probabilities source.
 */
void BranchMulti::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	// retrieve probabilities of executing each branch
	if (inIter->getAttribute("inProbabilities").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "probabilities of executing each branch expected!");
	}
	mProbabilities_Ref.assign(inIter->getAttribute("inProbabilities"));
	
	switch (mProbabilities_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			throw schnaps_RunTimeExceptionM("The primitive is undefined for the specific probabilities source!");
			break;
		case '$':
			// parameter value
			mProbabilities = Core::castHandleT<Core::Vector>(ioSystem.getParameters().getParameterHandle(mProbabilities_Ref.substr(1)));
			break;
		default: {
			// direct value
			std::stringstream lISS(mProbabilities_Ref);
			PACC::Tokenizer lTokenizer(lISS);
			lTokenizer.setDelimiters("|", "");
			
			std::string lProbability;
			double lSum = 0;

			mProbabilities = new Core::Vector();
			while (lTokenizer.getNextToken(lProbability)) {
				mProbabilities->push_back(new Core::Double(SCHNAPS::str2dbl(lProbability)));
				lSum += SCHNAPS::str2dbl(lProbability);
			}
			
#ifndef SCHNAPS_NDEBUG
			if (lSum != 1) {
				printf("Warning: multi branches probabilities must sum to 1 (current sum: %f)!\n", lSum);
				printf("\tIn: void SCHNAPS::Plugins::Control::BranchMulti::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)\n");
			}
#endif
			break; }
	}
	setNumberArguments(mProbabilities->size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::BranchMulti::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void BranchMulti::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("inProbabilities", mProbabilities_Ref);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::BranchMulti::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle BranchMulti::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Core::RouletteT<unsigned int> lRoulette;
	for (unsigned int i = 0; i < mProbabilities->size(); i++) {
		lRoulette.insert(i, Core::castHandleT<Core::Double>((*mProbabilities)[i])->getValue());
	}
	return getArgument(inIndex, lRoulette.select(ioContext.getRandomizer()), ioContext);
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::BranchMulti::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the nth argument requested return type.
 * \param  inIndex Index of the current primitive.
 * \param  inN Index of the argument to get the type.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the type of the nth argument.
 * \throw  SCHNAPS::Core::AssertException if the index of argument is out of bounds.
 */
const std::string& BranchMulti::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_UpperBoundCheckAssertM(inN, getNumberArguments()-1);
	const static std::string lType("Any");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::BranchMulti::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& BranchMulti::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	std::string lCommonType = getArgType(inIndex, 0, ioContext);

	for (unsigned int i = 1; i < getNumberArguments(); i++) {
		lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
	}

	const static std::string lCommonType_Final(lCommonType);
	return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::BranchMulti::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
