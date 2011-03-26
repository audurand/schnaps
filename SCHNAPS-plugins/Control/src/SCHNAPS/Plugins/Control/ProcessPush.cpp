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
	mDelay_Ref("")
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
	mDelay_Ref(inOriginal.mDelay_Ref.c_str())
{}

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
		ioStreamer.insertAttribute("inDelay", mDelay_Ref);
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
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Control::ProcessPush::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Execute the primitive.
 * \param  inIndex Index of the current primitive.
 * \param  ioContext A reference to the execution context.
 * \return A handle to the execution result.
 * \throw  SCHNAPS::Core::RunTimeException if the method is undefined for the specified delay source.
 */
Core::AnyType::Handle ProcessPush::execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	Simulation::SimulationContext& lContext = Core::castObjectT<Simulation::SimulationContext&>(ioContext);
	
	if (mDelay == NULL) {
		unsigned long lDelay;
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
				// TODO: local variable value
				break;
			default:
				throw schnaps_RunTimeExceptionM("The method is undefined for the specified delay source!");
				break;
		}
		lContext.getPushList().push_back(Simulation::Push(mLabel, mTarget, lContext.getClock().getValue()+lDelay));
	} else {
		// direct value or parameter value
		lContext.getPushList().push_back(Simulation::Push(mLabel, mTarget, lContext.getClock().getValue()+mDelay->getValue()));
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
