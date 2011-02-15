/*
 * BranchStatic.cpp
 *
 *  Created on: 2010-05-15
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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new primitive that returns the result of a BranchStatic.
 */
BranchStatic::BranchStatic() :
		Primitive(),	// unknown number of children
		mWeights_Ref(""),
		mWeights(new SCHNAPS::Core::Vector())
{}

BranchStatic::BranchStatic(const BranchStatic& inOriginal) :
		Primitive(inOriginal.getNumberArguments()),
		mWeights_Ref(inOriginal.mWeights_Ref.c_str())
{
	mWeights = new SCHNAPS::Core::Vector();

	if (mWeights_Ref.empty()) {
		for (unsigned int i = 0; i < inOriginal.mWeights->size(); i++) {
			mWeights->push_back((*inOriginal.mWeights)[i]->clone());
		}
	} else {
		mWeights = inOriginal.mWeights;
	}
}

void BranchStatic::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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
		// Retrieve branches weights
		if (inIter->getAttribute("weights").empty()) {
			if (inIter->getAttribute("weights.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "weights expected!");
			} else {
				mWeights_Ref = inIter->getAttribute("weights.ref");
				std::stringstream lSS;
				lSS << "ref." << mWeights_Ref;
				mWeights = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Vector>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			std::stringstream lSS(inIter->getAttribute("weights"));
			PACC::Tokenizer lTokenizer(lSS);
			lTokenizer.setDelimiters("|", "");

			std::string lWeight;
			double lSum = 0;

			mWeights->clear();
			while (lTokenizer.getNextToken(lWeight)) {
				mWeights->push_back(new SCHNAPS::Core::Double(SCHNAPS::str2dbl(lWeight)));
				lSum += SCHNAPS::str2dbl(lWeight);
			}
#ifndef SIMULATOR_NDEBUG
			if (lSum != 1.0) {
				printf("Warning: branches (static) weights must sum to 1 (current sum: %f)!\n", lSum);
			}
#endif
		}

		setNumberArguments(mWeights->size());
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::BranchStatic::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void BranchStatic::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mWeights_Ref.empty()) {
			std::stringstream lSS;
			for (unsigned int i = 0; i < mWeights->size(); i++) {
				lSS << (*mWeights)[i]->writeStr() << "|";
			}
			ioStreamer.insertAttribute("weights", lSS.str().c_str());
		} else {
			ioStreamer.insertAttribute("weights.ref", mWeights_Ref);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::BranchStatic::writeContent(PACC::XML::Streamer&, bool) const");
}


BranchStatic& BranchStatic::operator=(const BranchStatic& inOriginal) {
	schnaps_StackTraceBeginM();
		mWeights_Ref = inOriginal.mWeights_Ref.c_str();

		if (mWeights_Ref.empty()) {
			mWeights->clear();
			for (unsigned int i = 0; i < inOriginal.mWeights->size(); i++) {
				mWeights->push_back((*inOriginal.mWeights)[i]);
			}
		} else {
			mWeights = inOriginal.mWeights;
		}
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::BranchStatic& SCHNAPS::Plugins::Control::BranchStatic::operator=(const SCHNAPS::Plugins::Control::BranchStatic&)");
}

SCHNAPS::Core::AnyType::Handle BranchStatic::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::RouletteT<unsigned int> lRoulette;
		for (unsigned int i = 0; i < mWeights->size(); i++) {
			lRoulette.insert(i, SCHNAPS::Core::castHandleT<SCHNAPS::Core::Double>((*mWeights)[i])->getValue());
		}
		// Retrieve children index to execute.
		unsigned int lN = lRoulette.select(ioContext.getRandomizer());
		// Return executed children result.
		return getArgument(inIndex, lN, ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::BranchStatic::execute(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& BranchStatic::getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::BranchStatic::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& BranchStatic::getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() < 2) {
			unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
			return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
		}

		std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));

		for (unsigned int i = 2; i < getNumberArguments(); i++) {
			lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
		}

		const static std::string lCommonType_Final(lCommonType);
		return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::BranchStatic::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
