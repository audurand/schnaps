/*
 * ProcessPush.cpp
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
ProcessPush::ProcessPush() :
	Primitive(0),
	mLabel(""),
	mDelay(NULL),
	mDelay_Ref(""),
	mRepeat(NULL),
	mRepeat_Ref(""),
	mDelta(NULL),
	mDelta_Ref(""),
	mUnits(Simulation::Clock::eOther)
{}

/*!
 * \brief Construct a primitive for pushing process as a copy of an original.
 * \param inOriginal A const reference to the original primitive for pushing process.
 */
ProcessPush::ProcessPush(const ProcessPush& inOriginal) :
	Primitive(0),
	mLabel(inOriginal.mLabel.c_str()),
	mTarget(inOriginal.mTarget),
	mDelay(inOriginal.mDelay),
	mDelay_Ref(inOriginal.mDelay_Ref.c_str()),
	mRepeat(inOriginal.mRepeat),
	mRepeat_Ref(inOriginal.mRepeat_Ref.c_str()),
	mDelta(inOriginal.mDelta),
	mDelta_Ref(inOriginal.mDelta_Ref.c_str()),
	mUnits(inOriginal.mUnits)
{}

/*!
 * \brief  Copy operator.
 * \return A reference to the current object.
 */
ProcessPush& ProcessPush::operator=(const ProcessPush& inOriginal) {
	schnaps_StackTraceBeginM();
	mLabel.assign(inOriginal.mLabel.c_str());
	mTarget = inOriginal.mTarget;
	mDelay_Ref.assign(inOriginal.mDelay_Ref.c_str());
	mRepeat_Ref.assign(inOriginal.mRepeat_Ref.c_str());
	mDelta_Ref.assign(inOriginal.mDelta_Ref.c_str());
	
	switch (mDelay_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDelay = NULL;
			break;
		case '$':
			// parameter value
			mDelay = inOriginal.mDelay;
			break;
		default:
			// direct value
			mDelay = Core::castHandleT<Core::ULong>(inOriginal.mDelay->clone());
			break;
	}
	
	switch (mRepeat_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mRepeat = NULL;
			break;
		case '$':
			// parameter value
			mRepeat = inOriginal.mRepeat;
			break;
		default:
			// direct value
			mRepeat = Core::castHandleT<Core::ULong>(inOriginal.mRepeat->clone());
			break;
	}
	
	switch (mDelta_Ref[0]) {
		case '@':
			// individual variable value
		case '#':
			// environment variable value
		case '%':
			// local variable value
			mDelta = NULL;
			break;
		case '$':
			// parameter value
			mDelta = inOriginal.mDelta;
			break;
		default:
			// direct value
			mDelta = Core::castHandleT<Core::ULong>(inOriginal.mDelta->clone());
			break;
	}
	
	mUnits = inOriginal.mUnits;

	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Plugins::Control::ProcessPush& SCHNAPS::Plugins::Control::ProcessPush::operator=(const SCHNAPS::Plugins::Control::ProcessPush&)");
}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label attribute is missing.
 * \throw SCHNAPS::Core::IOException if target attribute is missing.
 * \throw SCHNAPS::Core::IOException if target attribute value is unknown.
 */
void ProcessPush::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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

	if (inIter->getAttribute("inLabel").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "label of the referenced process expected!");
	}
	mLabel.assign(inIter->getAttribute("inLabel"));

	if (inIter->getAttribute("inTarget").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "target of the referenced process " + mLabel + " expected!");
	}
	std::string lTarget = inIter->getAttribute("inTarget");

	if (lTarget == "current") {
		mTarget = Simulation::Process::eCurrent;
	} else if (lTarget == "environment") {
		mTarget = Simulation::Process::eEnvironment;
	} else if (lTarget == "individuals") {
		mTarget = Simulation::Process::eIndividuals;
	} else {
		throw schnaps_IOExceptionNodeM(*inIter, "ProcessPush " + mLabel + " unknown target: " + lTarget);
	}

	mDelay_Ref.assign(inIter->getAttribute("inDelay"));
	if (mDelay_Ref.empty()) {
		mDelay = new Core::ULong(0);
	} else {
		switch (mDelay_Ref[0]) {
			case '@':
				// individual variable value
			case '#':
				// environment variable value
			case '%':
				// local variable value
				mDelay = NULL;
				break;
			case '$':
				// parameter value
				mDelay = Core::castHandleT<Core::ULong>(ioSystem.getParameters().getParameterHandle(mDelay_Ref.substr(1).c_str()));
				break;
			default:
				// direct value
				mDelay = new Core::ULong();
				mDelay->readStr(mDelay_Ref);
				break;
		}
	}

	mRepeat_Ref.assign(inIter->getAttribute("inRepeat"));
	if (mRepeat_Ref.empty()) {
		mRepeat = new Core::ULong(0);
	} else {
		switch (mRepeat_Ref[0]) {
			case '@':
				// individual variable value
			case '#':
				// environment variable value
			case '%':
				// local variable value
				mRepeat = NULL;
				break;
			case '$':
				// parameter value
				mRepeat = Core::castHandleT<Core::ULong>(ioSystem.getParameters().getParameterHandle(mRepeat_Ref.substr(1).c_str()));
				break;
			default:
				// direct value
				mRepeat = new Core::ULong();
				mRepeat->readStr(mRepeat_Ref);
				break;
		}
	}

	mDelta_Ref.assign(inIter->getAttribute("inDelta"));
	if (mDelta_Ref.empty()) {
		mDelta = new Core::ULong(1);
	} else {
		switch (mDelta_Ref[0]) {
			case '@':
				// individual variable value
			case '#':
				// environment variable value
			case '%':
				// local variable value
				mDelta = NULL;
				break;
			case '$':
				// parameter value
				mDelta = Core::castHandleT<Core::ULong>(ioSystem.getParameters().getParameterHandle(mDelta_Ref.substr(1).c_str()));
				break;
			default:
				// direct value
				mDelta = new Core::ULong();
				mDelta->readStr(mDelta_Ref);
				break;
		}
	}
	
	std::string lUnits = inIter->getAttribute("inUnits");
	if (lUnits.empty()) {
		mUnits = Simulation::Clock::eOther;
	} else {
		if (lUnits == "year") {
			mUnits = Simulation::Clock::eYear;
		} else if (lUnits == "month") {
			mUnits = Simulation::Clock::eMonth;
		} else if (lUnits == "day") {
			mUnits = Simulation::Clock::eDay;
		} else {
			mUnits = Simulation::Clock::eOther;
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessPush::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void ProcessPush::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("inLabel", mLabel);
		switch (mTarget) {
			case Simulation::Process::eCurrent:
				ioStreamer.insertAttribute("inTarget", "current");
				break;
			case Simulation::Process::eEnvironment:
				ioStreamer.insertAttribute("inTarget", "environment");
				break;
			case Simulation::Process::eIndividuals:
				ioStreamer.insertAttribute("inTarget", "individuals");
				break;
			default:
				break;
		}
		ioStreamer.insertAttribute("inDelay", mDelay_Ref);
		ioStreamer.insertAttribute("inRepeat", mRepeat_Ref);
		ioStreamer.insertAttribute("inDelta", mDelta_Ref);
		switch (mUnits) {
			case Simulation::Clock::eYear:
				ioStreamer.insertAttribute("units", "year");
				break;
			case Simulation::Clock::eMonth:
				ioStreamer.insertAttribute("units", "month");
				break;
			case Simulation::Clock::eDay:
				ioStreamer.insertAttribute("units", "day");
				break;
			default:
				ioStreamer.insertAttribute("units", "other");
				break;
		}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessPush::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 */
Core::AnyType::Handle ProcessPush::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	
	unsigned long lDelay, lRepeat, lDelta;
	switch (mDelay_Ref[0]) {
		case '@':
			// individual variable value
			lDelay = Core::castHandleT<Core::ULong>(lContext.getIndividual().getState().getVariableHandle(mDelay_Ref.substr(1).c_str()))->getValue();
			break;
		case '#':
			// environment variable value
			lDelay = Core::castHandleT<Core::ULong>(lContext.getEnvironment().getState().getVariableHandle(mDelay_Ref.substr(1).c_str()))->getValue();
			break;
		case '%':
			// local variable value
			lDelay = Core::castHandleT<Core::ULong>(lContext.getLocalVariableHandle(mDelay_Ref.substr(1).c_str()))->getValue();
			break;
		default:
			// parameter or direct value
			lDelay = mDelay->getValue();
			break;
	}
	switch (mRepeat_Ref[0]) {
		case '@':
			// individual variable value
			lRepeat = Core::castHandleT<Core::ULong>(lContext.getIndividual().getState().getVariableHandle(mRepeat_Ref.substr(1).c_str()))->getValue();
			break;
		case '#':
			// environment variable value
			lRepeat = Core::castHandleT<Core::ULong>(lContext.getEnvironment().getState().getVariableHandle(mRepeat_Ref.substr(1).c_str()))->getValue();
			break;
		case '%':
			// local variable value
			lRepeat = Core::castHandleT<Core::ULong>(lContext.getLocalVariableHandle(mRepeat_Ref.substr(1).c_str()))->getValue();
			break;
		default:
			// parameter or direct value
			lRepeat = mRepeat->getValue();
			break;
	}
	switch (mDelta_Ref[0]) {
		case '@':
			// individual variable value
			lDelta = Core::castHandleT<Core::ULong>(lContext.getIndividual().getState().getVariableHandle(mDelta_Ref.substr(1).c_str()))->getValue();
			break;
		case '#':
			// environment variable value
			lDelta = Core::castHandleT<Core::ULong>(lContext.getEnvironment().getState().getVariableHandle(mDelta_Ref.substr(1).c_str()))->getValue();
			break;
		case '%':
			// local variable value
			lDelta = Core::castHandleT<Core::ULong>(lContext.getLocalVariableHandle(mDelta_Ref.substr(1).c_str()))->getValue();
			break;
		default:
			// parameter or direct value
			lDelta = mDelta->getValue();
			break;
	}
	
	unsigned long lStartValue = lContext.getClock().getValue(mUnits) + lDelay;
	for (unsigned int i = 0; i < lRepeat; i++) {
		lContext.getPushList().push_back(Simulation::Push(mLabel, mTarget, lContext.getClock().getTick(lStartValue + i * lDelta, mUnits)));
	}
	return NULL;
	schnaps_StackTraceEndM("SCHNAPS::Core::AnyType::Handle SCHNAPS::Plugins::Control::ProcessPush::execute(unsigned int, SCHNAPS::Core::ExecutionContext&)");
}

/*!
 * \brief  Return the primitive return type.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A const reference to the return type.
 */
const std::string& ProcessPush::getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	const static std::string lType("Void");
	return lType;
	schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessPush::getReturnType(unsigned int, SCHNAPS::Core::ExecutionContext&) const");
}
