/*
 * ExecutionContext.hpp
 *
 *  Created on: 2009-02-19
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

#ifndef SCHNAPS_Core_ExecutionContext_hpp
#define SCHNAPS_Core_ExecutionContext_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"
#include "SCHNAPS/Core/System.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class ExecutionContext SCHNAPS/Core/ExecutionContext.hpp "SCHNAPS/Core/ExecutionContext.hpp"
 *  \brief ExecutionContext class, the context for the execution of a primitive tree.
 */
class ExecutionContext : public Object {
public:
	//! ExecutionContext allocator type.
	typedef AllocatorT<ExecutionContext, Object::Alloc> Alloc;
	//! ExecutionContext handle type.
	typedef PointerT<ExecutionContext, Object::Handle> Handle;
	//! ExecutionContext bag type.
	typedef ContainerT<ExecutionContext, Object::Bag> Bag;

	ExecutionContext();
	ExecutionContext(const ExecutionContext& inExecutionContext);
	explicit ExecutionContext(const System::Handle inSystem);
	virtual ~ExecutionContext(){}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("ExecutionContext");
			return lName;
		schnaps_StackTraceEndM("const std::string& ExecutionContext::getName() const");
	}

	ExecutionContext& operator=(const ExecutionContext& inOriginal);

	void setSystem(System::Handle inSystem) {
		mSystem = inSystem;
	}

	void setPrimitiveTree(PrimitiveTree::Handle inPrimitiveTree) {
		mPrimitiveTree = inPrimitiveTree;
	}

	const System::Handle getSystemHandle() const {
		return mSystem;
	}

	const System& getSystem() const {
		schnaps_NonNullPointerAssertM(mSystem);
		return *mSystem;
	}

	const PrimitiveTree::Handle getPrimitiveTreeHandle() const {
		return mPrimitiveTree;
	}

	const PrimitiveTree& getPrimitiveTree() const {
		schnaps_NonNullPointerAssertM(mPrimitiveTree);
		return *mPrimitiveTree;
	}

	void setThreadNb(unsigned int inThreadNb) {
		mThreadNb = inThreadNb;
	}

	const unsigned int getThreadNb() const {
		return mThreadNb;
	}

	Randomizer& getRandomizer() {
		schnaps_NonNullPointerAssertM(mSystem);
		return mSystem->getRandomizer(mThreadNb);
	}

	Logger& getLogger() {
		schnaps_NonNullPointerAssertM(mSystem);
		return mSystem->getLogger(mThreadNb);
	}

	Component& getComponent(std::string inName) {
		schnaps_NonNullPointerAssertM(mSystem);
		return mSystem->getComponent(inName);
	}

protected:
	System::Handle mSystem;					//!< Handle to the system.
	PrimitiveTree::Handle mPrimitiveTree;	//!< Handle to the tree that defines the current action.

	unsigned int mThreadNb;					//!< Thread nnumber associated to that context.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_ExecutionContext_hpp */
