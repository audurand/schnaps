/*
 * GenerationThread.hpp
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

#ifndef SCHNAPS_Simulation_GenerationThread_hpp
#define SCHNAPS_Simulation_GenerationThread_hpp

#include "PACC/PACC.hpp"

#include "SCHNAPS/Core/String.hpp"
#include "SCHNAPS/Simulation/GenerationContext.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class GenerationThread SCHNAPS/Simulation/GenerationThread.hpp "SCHNAPS/Simulation/GenerationThread.hpp"
 *  \brief GenerationThread class.
 */
class GenerationThread: public Core::Object, public PACC::Threading::Thread {
public:
	//! GenerationThread allocator type.
	typedef Core::AllocatorT<GenerationThread, Core::Object::Alloc> Alloc;
	//! GenerationThread handle type.
	typedef Core::PointerT<GenerationThread, Core::Object::Handle> Handle;
	//! GenerationThread bag type.
	typedef Core::ContainerT<GenerationThread, Core::Object::Bag> Bag;

	explicit GenerationThread(PACC::Threading::Condition* inParallel,
								PACC::Threading::Semaphore* inSequential,
								GenerationContext::Handle inContext);
	~GenerationThread();

	void setGenerationInfo(unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex, Core::StringArray::Handle inEraseVariables) {
		mSize = inSize;
		mPrefix = inPrefix.c_str();
		mStartingIndex = inStartingIndex;
		mEraseVariables = inEraseVariables;
	}

	GenerationContext::Handle getContextHandle() {
		return mContext;
	}

	const unsigned int getSize() const {
		return mSize;
	}

	const std::string getPrefix() const {
		return mPrefix;
	}

	const unsigned int getStartingIndex() const {
		return mStartingIndex;
	}

	const Core::StringArray& getEraseVariables() const {
		schnaps_NonNullPointerAssertM(mEraseVariables);
		return *mEraseVariables;
	}

	Individual::Bag& getIndividuals() {
		schnaps_NonNullPointerAssertM(mIndividuals);
		return *mIndividuals;
	}

protected:
	void main();

private:
	// Synch mechanisms
	PACC::Threading::Condition* mParallel;
	PACC::Threading::Semaphore* mSequential;

	// Thread info
	GenerationContext::Handle mContext;

	// Generation info
	unsigned int mSize;							//!< Size of population to generate (by this thread).
	std::string mPrefix;						//!< Individual ID prefix.
	unsigned int mStartingIndex;				//!< First individual index (concatenate with prefix to get ID).
	Core::StringArray::Handle mEraseVariables;	//!< Individual variables to erase.

	// Result
	Individual::Bag::Handle mIndividuals;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenerationThread_hpp */
