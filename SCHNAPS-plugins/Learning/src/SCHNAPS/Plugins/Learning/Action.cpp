/*
 * Action.cpp
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
Action::Action(const std::string& inLabel) :
	mLabel(inLabel.c_str()),
	mTried(false),
	mUpdatedTimes(0),
	mReward(0),
	mReward2(0)
{}

/*!
 * \brief Construct a Action primitive as a copy of an original.
 * \param inOriginal A const reference to the original Action primitive.
 */
Action::Action(const Action& inOriginal) :
	mLabel(inOriginal.mLabel.c_str()),
	mTried(inOriginal.mTried),
	mUpdatedTimes(inOriginal.mUpdatedTimes),
	mReward(inOriginal.mReward),
	mReward2(inOriginal.mReward2)
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 * \throw SCHNAPS::Core::IOException if label, tried, updatedTimes, reward, and reward2 attributes are missing.
 * \throw SCHNAPS::Core::IOException if tried attribute. is given in an unknown format.
 */
void Action::readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem) {
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
	if (inIter->getAttribute("label").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "action label expected!");
	}
	mLabel.assign(inIter->getAttribute("label"));

	// retrieve tried flag
	if (inIter->getAttribute("tried").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "action tried flag expected!");
	}
	std::string lTried = inIter->getAttribute("tried");
	if (lTried == "true" || lTried == "TRUE" || lTried == "True" || lTried == "1") {
		mTried = true;
	} else if (lTried == "false" || lTried == "FALSE" || lTried == "False" || lTried == "0") {
		mTried = false;
	} else {
		throw schnaps_IOExceptionNodeM(*inIter, "action tried attribute unknown format!");
	}
	
	// retrieve the number of updated times
	if (inIter->getAttribute("updatedTimes").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "action number of updated times expected!");
	}
	mUpdatedTimes = SCHNAPS::str2uint(inIter->getAttribute("updatedTimes"));
	
	// retrieve the sum of rewards obtained
	if (inIter->getAttribute("reward").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "action sum of rewards expected!");
	}
	mReward = SCHNAPS::str2dbl(inIter->getAttribute("reward"));
	
	// retrieve the sum of squared rewards obtained
	if (inIter->getAttribute("reward2").empty()) {
		throw schnaps_IOExceptionNodeM(*inIter, "action sum of squared rewards expected!");
	}
	mReward2 = SCHNAPS::str2dbl(inIter->getAttribute("reward2"));
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Action::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.insertAttribute("label", mLabel);
	ioStreamer.insertAttribute("tried", mTried);
	ioStreamer.insertAttribute("updatedTimes", mUpdatedTimes);
	ioStreamer.insertAttribute("reward", mReward);
	ioStreamer.insertAttribute("reward2", mReward2);
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Set the tried flag.
 * \param inTried The new tried flag.
 */
void Action::setTried(bool inTried) {
	schnaps_StackTraceBeginM();
	mTried = inTried;
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::setTried(bool)");
}

/*!
 * \brief Set the number of times the action has been updated until now.
 * \param inUpdatedTimes The number of times the action has been updated until now.
 */
void Action::setUpdatedTimes(unsigned int inUpdatedTimes) {
	schnaps_StackTraceBeginM();
	mUpdatedTimes = inUpdatedTimes;
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::setUpdatedTimes(unsigned int)");
}

/*!
 * \brief Set the sum of reward the action has received until now.
 * \param inReward The sum of reward the action has received until now.
 */
void Action::setReward(double inReward) {
	schnaps_StackTraceBeginM();
	mReward = inReward;
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::setReward(double)");
}

/*!
 * \brief Set the sum of squared reward the action has received until now.
 * \param inReward The sum of squared reward the action has received until now.
 */
void Action::setReward2(double inReward2) {
	schnaps_StackTraceBeginM();
	mReward2 = inReward2;
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::Action::setReward2(double)");
}

