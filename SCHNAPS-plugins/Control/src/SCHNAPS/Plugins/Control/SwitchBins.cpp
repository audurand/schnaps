/*
 * SwitchBins.cpp
 *
 *  Created on: 2009-10-29
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

#include <string>

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new primitive that returns the result of a SwitchBins.
 */
SwitchBins::SwitchBins() :
		Primitive(),	// Unknown number of children
		mBins_Ref(""),
		mBinsType(""),
		mBins(new SCHNAPS::Core::Vector())
{}

SwitchBins::SwitchBins(const SwitchBins& inOriginal) :
		Primitive(inOriginal.getNumberArguments()),
		mBins_Ref(inOriginal.mBins_Ref.c_str()),
		mBinsType(inOriginal.mBinsType.c_str())
{
	if (mBins_Ref.empty()) {
		mBins = new SCHNAPS::Core::Vector();
		for (unsigned int i = 0; i < inOriginal.mBins->size(); i++) {
			mBins->push_back((*inOriginal.mBins)[i]->clone());
		}
	} else {
		mBins = inOriginal.mBins;
	}
}


void SwitchBins::readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem) {
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

		// Retrieve keys
		if (inIter->getAttribute("boundaries").empty()) {
			if (inIter->getAttribute("boundaries.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "boundaries of the bins expected!");
			} else {
				mBins_Ref = inIter->getAttribute("boundaries.ref");
				std::stringstream lSS;
				lSS << "ref." << mBins_Ref;
				mBins = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Vector>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			// Retrieve weight type
			if (inIter->getAttribute("binsType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of the bins expected!");
			}
			mBinsType = inIter->getAttribute("binsType");

			std::stringstream lSS(inIter->getAttribute("boundaries"));
			PACC::Tokenizer lTokenizer(lSS);
			lTokenizer.setDelimiters("|", "");

			SCHNAPS::Core::Number::Alloc::Handle lAlloc = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Number::Alloc>(ioSystem.getFactory().getAllocator(mBinsType));
			if (lAlloc == NULL) {
				std::ostringstream lOSS;
				lOSS << "no allocator named '" <<  mBinsType;
				lOSS << "' found in the factory";
				throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
			}

			std::string lBoundary;
			SCHNAPS::Core::Number::Handle lNumber;

			mBins->clear();
			while (lTokenizer.getNextToken(lBoundary)) {
				// Add to SwitchBins
				lNumber = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Number>(lAlloc->allocate());
				lNumber->readStr(lBoundary);
				if (!mBins->empty()) {
					if ((lNumber->isLess(*mBins->back())) || (lNumber->isEqual(*mBins->back()))) {
						throw schnaps_IOExceptionNodeM(*inIter, "boundaries of the bins expected to be in crescent order!");
					}
				}
				mBins->push_back(lNumber);
			}
		}

		setNumberArguments(mBins->size()+2);	// + 1 for the value switched on, + 1 for default case
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::SwitchBins::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void SwitchBins::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mBins_Ref.empty()) {
			std::stringstream lSS;
			for (unsigned int i = 0; i < mBins->size(); i++) {
				lSS << (*mBins)[i]->writeStr() << "|";
			}
			ioStreamer.insertAttribute("binsType", mBinsType);
			ioStreamer.insertAttribute("boundaries", lSS.str().c_str());
		}

		ioStreamer.insertAttribute("boundaries.ref", mBins_Ref);
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::SwitchBins::writeContent(PACC::XML::Streamer&, bool) const");
}

SwitchBins& SwitchBins::operator=(const SwitchBins& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());

		mBins_Ref = inOriginal.mBins_Ref.c_str();
		mBinsType = inOriginal.mBinsType.c_str();
		if (mBins_Ref.empty()) {
			mBins->clear();
			for (unsigned int i = 0; i < inOriginal.mBins->size(); i++) {
				mBins->push_back((*inOriginal.mBins)[i]->clone());
			}
		} else {
			mBins = inOriginal.mBins;
		}
		return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::SwitchBins& SCHNAPS::Plugins::Control::SwitchBins::operator=(const SCHNAPS::Plugins::Control::SwitchBins&)");
}

SCHNAPS::Core::AnyType::Handle SwitchBins::execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		SCHNAPS::Core::Number::Handle lArg = SCHNAPS::Core::castHandleT<SCHNAPS::Core::Number>(getArgument(inIndex, 0, ioContext));
		for (unsigned int i = 0; i < mBins->size(); i++) {
			if ((lArg->isLess(*(*mBins)[i])) || (lArg->isEqual(*(*mBins)[i]))) {
				return getArgument(inIndex, i+2, ioContext);
			}
		}
		return getArgument(inIndex, 1, ioContext);
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::SwitchBins::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

const std::string& SwitchBins::getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		if (inN == 0) {
			return mBins->back()->getType();
		} else if (inN == 1) {
			const static std::string lType("Any");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::SwitchBins::getArgType(unsigned int, unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}

const std::string& SwitchBins::getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() < 3) {
			unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
			return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
		}

		std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));

		for (unsigned int i = 3; i < getNumberArguments(); i++) {
			lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
		}

		const static std::string lCommonType_Final(lCommonType);
		return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::SwitchBins::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
