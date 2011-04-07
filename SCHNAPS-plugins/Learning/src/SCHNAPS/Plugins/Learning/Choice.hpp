/*
 * Choice.hpp
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

#ifndef SCHNAPS_Plugins_Learning_Choice_hpp
#define SCHNAPS_Plugins_Learning_Choice_hpp

#include "SCHNAPS/Plugins/Learning/Action.hpp"
#include "SCHNAPS/Plugins/Learning/LearningContext.hpp"

#include "SCHNAPS/SCHNAPS.hpp"
#include "PACC/PACC.hpp"

#include <map>
#include <vector>

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class Choice SCHNAPS/Plugins/Learning/Choice.hpp "SCHNAPS/Plugins/Learning/Choice.hpp"
 *  \brief Make choice among different options.
 */
class Choice: public Core::Object, public std::map<std::string, std::vector<Action> > {
private:
	typedef std::pair<std::string, Core::AnyType::Handle> LocalVariable;
	
	struct Function {
		std::vector<LocalVariable> mLocalVariables;
		Core::PrimitiveTree::Handle mExecution;
		
		Function() {
			mExecution = NULL;
		}
		
		//! Read local variables from XML using system.
		void readLocalVariables(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
		//! Write content to XML.
		virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	};

public:
	//! Choice allocator type.
	typedef Core::AllocatorT<Choice, Core::Object::Alloc> Alloc;
	//! Choice handle type.
	typedef Core::PointerT<Choice, Core::Object::Handle> Handle;
	//! Choice bag type.
	typedef Core::ContainerT<Choice, Core::Object::Bag> Bag;
	
	Choice();
	Choice(const Choice& inOriginal);
	virtual ~Choice() {}
	
	//! Return a handle to a deep copy of the object.
	virtual Core::Object::Handle deepCopy(const Core::System& inSystem) const;
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_Choice");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::Choice::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
	
	//! Return the state computed using a specific learning context.
	std::string computeState(LearningContext& ioContext) const;
	//! Return the reward computed using a specific learning context.
	double computeReward(LearningContext& ioContext) const;
	
	/*!
	 * \brief  Return a reference to all actions in a specific state.
	 * \param  inState A const reference to the specific state.
	 * \return A reference to all actions in a specific state.
	 */
	std::vector<Action>& getActions(const std::string& inState) {
		schnaps_StackTraceBeginM();
		if (this->find(inState) == this->end()) {
			// unknown state, add it and all possible actions (options)
			std::vector<Action> lActions;
			lActions.reserve(mOptions.size());
			for (unsigned int i = 0; i < mOptions.size(); i++) {
				lActions.push_back(Action(mOptions[i]));
			}
			this->insert(std::pair<std::string, std::vector<Action> >(inState, lActions));
		}
		return (*this)[inState];
		schnaps_StackTraceEndM("std::vector<SCHNAPS::Plugins::Learning::Action>& SCHNAPS::Plugins::Learning::Choice::getActions(const std::string&)");
	}
	
private:
	//! Read options from XML using system.
	void readOptions(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Read state function from XML using system.
	void readFunctionState(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Read reward function from XML using system.
	void readFunctionReward(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Read choice map from XML using system.
	void readChoiceMap(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of options to XML.
	void writeOptions(PACC::XML::Streamer& ioStreamer, bool inIndent) const;
	//! Write content of state function to XML.
	void writeFunctionState(PACC::XML::Streamer& ioStreamer, bool inIndent) const;
	//! Write content of reward function to XML.
	void writeFunctionReward(PACC::XML::Streamer& ioStreamer, bool inIndent) const;
	//! Write content of choice map to XML.
	void writeChoiceMap(PACC::XML::Streamer& ioStreamer, bool inIndent) const;
	
private:
	std::string mLabel;					//!< Choice identification label.
	std::vector<std::string> mOptions;	//!< The labels of all possible actions to be taken from that choice.
	Function mFunctionState;			//!< The function to compute current state.
	Function mFunctionReward;			//!< The function to compute the reward given for having taken an action. 
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_Choice_hpp */
