/*
 * Action.hpp
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

#ifndef SCHNAPS_Plugins_Learning_Action_hpp
#define SCHNAPS_Plugins_Learning_Action_hpp

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/PACC.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class Action SCHNAPS/Plugins/Learning/Action.hpp "SCHNAPS/Plugins/Learning/Action.hpp"
 *  \brief Cumulated information on an action that can be taken at a choice.
 */
class Action: public Core::Object {
public:
	//! Action allocator type.
	typedef Core::AllocatorT<Action, Core::Object::Alloc> Alloc;
	//! Action handle type.
	typedef Core::PointerT<Action, Core::Object::Handle> Handle;
	//! Action bag type.
	typedef Core::ContainerT<Action, Core::Object::Bag> Bag;
	
	explicit Action(const std::string& inLabel = "");
	Action(const Action& inOriginal);
	virtual ~Action() {}
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_Action");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::Action::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	
	//! Set the tried flag.
	void setTried(bool inTried);
	//! Set the number of times the action has been updated until now.
	void setUpdatedTimes(unsigned int inUpdatedTimes);
	//! Set the sum of reward the action has received until now.
	void setReward(double inReward);
	//! Set the sum of squared reward the action has received until now.
	void setReward2(double inReward2);
	
	/*!
	 * \brief  Return a const reference to the label.
	 * \return A const reference to the label.
	 */
	const std::string& getLabel() const {
		schnaps_StackTraceBeginM();
		return mLabel;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::Action::getLabel() const");
	}
	
	/*!
	 * \brief  Return a const reference to the tried flag.
	 * \return A const reference to the tried flag.
	 */
	const bool& getTried() const {
		schnaps_StackTraceBeginM();
		return mTried;
		schnaps_StackTraceEndM("const bool& SCHNAPS::Plugins::Learning::Action::getTried() const");
	}
	
	/*!
	 * \brief  Return a const reference to the number of times the action has been updated.
	 * \return A const reference to the number of times the action has been updated.
	 */
	const unsigned int& getUpdatedTimes() const {
		schnaps_StackTraceBeginM();
		return mUpdatedTimes;
		schnaps_StackTraceEndM("const unsigned int& SCHNAPS::Plugins::Learning::Action::getUpdatedTimes() const");
	}
	
	/*!
	 * \brief  Return a const reference to the sum of received rewards.
	 * \return A const reference to the sum of received rewards.
	 */
	const double& getReward() const {
		schnaps_StackTraceBeginM();
		return mReward;
		schnaps_StackTraceEndM("const double& SCHNAPS::Plugins::Learning::Action::getReward() const");
	}
	
	/*!
	 * \brief  Return a const reference to the sum of squared received rewards.
	 * \return A const reference to the sum of squared received rewards.
	 */
	const double& getReward2() const {
		schnaps_StackTraceBeginM();
		return mReward2;
		schnaps_StackTraceEndM("const double& SCHNAPS::Plugins::Learning::Action::getReward2() const");
	}
	
private:
	std::string mLabel;
	bool mTried;
	unsigned int mUpdatedTimes;
	double mReward;
	double mReward2;
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_Action_hpp */
