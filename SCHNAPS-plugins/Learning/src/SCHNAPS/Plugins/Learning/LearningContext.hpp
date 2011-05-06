/*
 * LearningContext.hpp
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

#ifndef SCHNAPS_Plugins_Learning_LearningContext_hpp
#define SCHNAPS_Plugins_Learning_LearningContext_hpp

#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Learning {

/*!
 *  \class LearningContext SCHNAPS/Plugins/Learning/LearningContext.hpp "SCHNAPS/Plugins/Learning/LearningContext.hpp"
 *  \brief Execution context to use inside the learning module.
 */
class LearningContext: public Simulation::ExecutionContext {
public:
	//! LearningContext allocator type.
	typedef Core::AllocatorT<LearningContext, Simulation::ExecutionContext::Alloc> Alloc;
	//! LearningContext handle type.
	typedef Core::PointerT<LearningContext, Simulation::ExecutionContext::Handle> Handle;
	//! LearningContext bag type.
	typedef Core::ContainerT<LearningContext, Simulation::ExecutionContext::Bag> Bag;
	
	LearningContext();
	LearningContext(const LearningContext& inOriginal);
	virtual ~LearningContext() {}
	
	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Learning_LearningContext");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::LearningContext::getName() const");
	}

	/*!
	 * \brief Set the state.
	 * \param inState A const reference to the new state.
	 */
	void setState(const std::string& inState) {
		schnaps_StackTraceBeginM();
		mState.assign(inState.c_str());
		schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningContext::setState(const std::string&)");
	}

	/*!
	 * \brief Set the action label.
	 * \param inActionLabel A const reference to the new action label.
	 */
	void setActionLabel(const std::string& inActionLabel) {
		schnaps_StackTraceBeginM();
		mActionLabel.assign(inActionLabel.c_str());
		schnaps_StackTraceEndM("void SCHNAPS::Plugins::Learning::LearningContext::setActionLabel(const std::string&)");
	}

	/*!
	 * \brief  Return a const reference to the state.
	 * \return A const reference to the state.
	 */
	const std::string& getState() const {
		schnaps_StackTraceBeginM();
		return mState;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::LearningContext::getState() const");
	}

	/*!
	 * \brief  Return a const reference to the action label.
	 * \return A const reference to the action label.
	 */
	const std::string& getActionLabel() const {
		schnaps_StackTraceBeginM();
		return mActionLabel;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Learning::LearningContext::getActionLabel() const");
	}
	
private:
	std::string mState;			//!< State of individual.
	std::string mActionLabel;	//!< Label of action chosen.
};
} // end of Learning namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Learning_LearningContext_hpp */
