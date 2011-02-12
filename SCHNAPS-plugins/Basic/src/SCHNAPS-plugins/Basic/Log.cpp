/*
 * Log.cpp
 *
 *  Created on: 2010-02-20
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

/*!
 *  \brief Construct a new primitive to log a new entry in the current logger.
 */
Log::Log() :
		Primitive(0)
{}

void Log::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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
		// Retrieve log type
		mType = inIter->getAttribute("type");

		// Retrieve log message
		mMessage = inIter->getAttribute("message");
	schnaps_StackTraceEndM("void basic::Log::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Log::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("type", mType);
		ioStreamer.insertAttribute("message", mMessage);
	schnaps_StackTraceEndM("void basic::Log::writeContent(PACC::XML::Streamer&, bool) const");
}

core::AnyType::Handle Log::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		simulator::SimulationContext& lContext = core::castObjectT<simulator::SimulationContext&>(ioContext);

		std::stringstream lLog;
		lLog << lContext.getIndividual().getID() << "," << lContext.getClock().getValue() << "," << mType << "," << "\"" << mMessage << "\"";

		lContext.getLogger().log(lLog.str());
		return NULL;
	schnaps_StackTraceEndM("core::AnyType::Handle basic::Log::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Log::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		const static std::string lType("Void");
		return lType;
	schnaps_StackTraceEndM("const std::string& Log::getReturnType(ExecutionContext& ioContext) const");
}
