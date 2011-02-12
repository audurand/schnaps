/*
 * NPV.cpp
 *
 *  Created on: 2010-04-20
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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

NPV::NPV() :
		core::Primitive(1),
		mRate_Ref(""),
		mRate(NULL)
{}

NPV::NPV(const NPV& inOriginal) :
		core::Primitive(1),
		mRate_Ref(inOriginal.mRate_Ref.c_str())
{
	mRate = core::castHandleT<core::Double>(inOriginal.mRate->clone());
}

void NPV::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
				mRate = core::castHandleT<core::Double>(ioSystem.getParameters()[lSS.str().c_str()]);
			}
		} else {
			mRate = new core::Double(core::str2dbl(inIter->getAttribute("rate")));
		}
	schnaps_StackTraceEndM("void basic::NPV::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void NPV::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mRate_Ref.empty()) {
			ioStreamer.insertAttribute("rate", mRate->writeStr());
		} else {
			ioStreamer.insertAttribute("rate.ref", mRate_Ref);
		}
	schnaps_StackTraceEndM("void basic::NPV::writeContent(PACC::XML::Streamer&, bool) const");
}

NPV& NPV::operator=(const NPV& inOriginal) {
	schnaps_StackTraceBeginM();
		mRate_Ref = inOriginal.mRate_Ref.c_str();
		mRate = core::castHandleT<core::Double>(inOriginal.mRate->clone());
		return *this;
	schnaps_StackTraceEndM("basic::NPV& basic::NPV::operator=(const basic::NPV&)");
}

core::AnyType::Handle NPV::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Number::Handle lArg1 = core::castHandleT<core::Number>(getArgument(inIndex, 0, ioContext));
		if (ioContext.getName() == "SimulationContext") {
			simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);
			double lTime = lContext.getClock().getValue();
			core::Double lDenum(std::pow(mRate->getValue() + 1, lTime));
			lArg1->div(lDenum);
			return lArg1;
		} else if (ioContext.getName() == "GenerationContext") {
			simulator::GenerationContext& lContext = core::castObjectT<simulator::GenerationContext&>(ioContext);
			double lTime = lContext.getClock().getValue();
			core::Double lDenum(std::pow(mRate->getValue() + 1, lTime));
			lArg1->div(lDenum);
			return lArg1;
		} else {
			throw schnaps_InternalExceptionM("NPV primitive is not defined for context '" + ioContext.getName() + "'!");
		}
	schnaps_StackTraceEndM("core::AnyType::Handle basic::NPV::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& NPV::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<1);
		if (inN == 0) {
			const static std::string lType("Number");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& basic::NPV::getArgType(unsigned int , unsigned int, core::ExecutionContext&) const");
}

const std::string& NPV::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 0, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& basic::NPV::getReturnType(unsigned int, core::ExecutionContext&) const");
}
