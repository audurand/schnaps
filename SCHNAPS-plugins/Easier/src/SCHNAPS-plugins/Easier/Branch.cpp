/*
 * Branch.cpp
 *
 *  Created on: 2010-11-21
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

#include "SCHNAPS-plugins/Easier/Easier.hpp"

using namespace easier;

/*!
 *  \brief Construct a new primitive that executes the first branch with some probability, else the second one.
 */
Branch::Branch() :
		Primitive(2),	// Unknown number of children
		mProbability_Ref(""),
		mProbability(NULL)
{}

Branch::Branch(const Branch& inOriginal) :
		mProbability_Ref(inOriginal.mProbability_Ref.c_str())
{
	if (mProbability_Ref.empty()) {
		mProbability = core::castHandleT<core::Double>(inOriginal.mProbability->clone());
	} else {
		mProbability = inOriginal.mProbability;
	}
}

void Branch::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

		// Retrieve probability of executing the first branch
		if (inIter->getAttribute("probability").empty()) {
			if (inIter->getAttribute("probability.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "probability of executing first branch expected!");
			} else { // Retrieve from parameter
				mProbability_Ref = inIter->getAttribute("probability.ref");
				std::stringstream lSS;
				lSS << "ref." << mProbability_Ref;
				mProbability = core::castHandleT<core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // Retrieve from attribute
			mProbability = new core::Double(core::str2dbl(inIter->getAttribute("probability")));
		}
	schnaps_StackTraceEndM("void core::Branch::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Branch::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mProbability_Ref.empty()) {
			ioStreamer.insertAttribute("probability", mProbability->writeStr());
		} else {
			ioStreamer.insertAttribute("probability.ref", mProbability_Ref);
		}
	schnaps_StackTraceEndM("void core::Branch::writeContent(PACC::XML::Streamer&, bool) const");
}


Branch& Branch::operator=(const Branch& inOriginal) {
	schnaps_StackTraceBeginM();
		mProbability_Ref = inOriginal.mProbability_Ref.c_str();

		if (mProbability_Ref.empty()) {
			mProbability = core::castHandleT<core::Double>(inOriginal.mProbability->clone());
		} else {
			mProbability = inOriginal.mProbability;
		}

		return *this;
	schnaps_StackTraceEndM("core::Branch& core::Branch::operator=(const core::Branch&)");
}

core::AnyType::Handle Branch::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		double lRandom = ioContext.getRandomizer().rollUniform();
		if (lRandom < mProbability->getValue()) {
			// First branch
			return getArgument(inIndex, 0, ioContext);
		} else {
			// Second branch
			return getArgument(inIndex, 1, ioContext);
		}
	schnaps_StackTraceEndM("core::AnyType::Handle core::Branch::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Branch::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& core::Branch::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& Branch::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));
		return lCommonType;
	schnaps_StackTraceEndM("const std::string& core::Branch::getReturnType(unsigned int, core::ExecutionContext&) const");
}
