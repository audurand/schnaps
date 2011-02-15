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

#include "SCHNAPS/Plugins/Control/Control.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Control;

/*!
 *  \brief Construct a new primitive that executes the first branch with some probability, else the second one.
 */
Branch::Branch() :
		Primitive(2),	// unknown number of children
		mProbability_Ref(""),
		mProbability(NULL)
{}

Branch::Branch(const Branch& inOriginal) :
		mProbability_Ref(inOriginal.mProbability_Ref.c_str())
{
	if (mProbability_Ref.empty()) {
		mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
	} else {
		mProbability = inOriginal.mProbability;
	}
}

void Branch::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

		// retrieve probability of executing the first branch
		if (inIter->getAttribute("probability").empty()) {
			if (inIter->getAttribute("probability.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "probability of executing first branch expected!");
			} else { // retrieve from parameter
				mProbability_Ref = inIter->getAttribute("probability.ref");
				std::stringstream lSS;
				lSS << "ref." << mProbability_Ref;
				mProbability = Core::castHandleT<Core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else { // retrieve from attribute
			mProbability = new Core::Double(SCHNAPS::str2dbl(inIter->getAttribute("probability")));
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Branch::readWithSystem(PACC::XML::ConstIterator, Core::System&)");
}

void Branch::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mProbability_Ref.empty()) {
			ioStreamer.insertAttribute("probability", mProbability->writeStr());
		} else {
			ioStreamer.insertAttribute("probability.ref", mProbability_Ref);
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::Branch::writeContent(PACC::XML::Streamer&, bool) const");
}

Branch& Branch::operator=(const Branch& inOriginal) {
	schnaps_StackTraceBeginM();
		mProbability_Ref = inOriginal.mProbability_Ref.c_str();

		if (mProbability_Ref.empty()) {
			mProbability = Core::castHandleT<Core::Double>(inOriginal.mProbability->clone());
		} else {
			mProbability = inOriginal.mProbability;
		}

		return *this;
	schnaps_StackTraceEndM("Core::Branch& SCHNAPS::Plugins::Control::Branch::operator=(const Core::Branch&)");
}

Core::AnyType::Handle Branch::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		double lRandom = ioContext.getRandomizer().rollUniform();
		if (lRandom < mProbability->getValue()) {
			// first branch
			return getArgument(inIndex, 0, ioContext);
		} else {
			// second branch
			return getArgument(inIndex, 1, ioContext);
		}
	schnaps_StackTraceEndM("Core::AnyType::Handle SCHNAPS::Plugins::Control::Branch::execute(unsigned int, Core::ExecutionContext&) const");
}

const std::string& Branch::getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		const static std::string lType("Any");
		return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Branch::getArgType(unsigned int, unsigned int, Core::ExecutionContext&) const");
}

const std::string& Branch::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 0, ioContext), getArgType(inIndex, 1, ioContext));
		return lCommonType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::Branch::getReturnType(unsigned int, Core::ExecutionContext&) const");
}
