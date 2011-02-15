/*
 * GenerationThread.hpp
 *
 *  Created on: 2010-04-10
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
class GenerationThread: public SCHNAPS::Core::Object, public PACC::Threading::Thread {
public:
	//! GenerationThread allocator type.
	typedef SCHNAPS::Core::AllocatorT<GenerationThread, SCHNAPS::Core::Object::Alloc> Alloc;
	//! GenerationThread handle type.
	typedef SCHNAPS::Core::PointerT<GenerationThread, SCHNAPS::Core::Object::Handle> Handle;
	//! GenerationThread bag type.
	typedef SCHNAPS::Core::ContainerT<GenerationThread, SCHNAPS::Core::Object::Bag> Bag;

	enum Position {eGENERATION, eEND};

	explicit GenerationThread(PACC::Threading::Condition* inParallel,
								PACC::Threading::Semaphore* inSequential,
								GenerationContext::Handle inContext);
	~GenerationThread();

	void setGenerationInfo(unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex, SCHNAPS::Core::StringArray::Handle inEraseVariables) {
		mSize = inSize;
		mPrefix = inPrefix.c_str();
		mStartingIndex = inStartingIndex;
		mEraseVariables = inEraseVariables;
	}

	void setPosition(Position inPosition) {
		mPosition = inPosition;
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

	const SCHNAPS::Core::StringArray& getEraseVariables() const {
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
	SCHNAPS::Core::StringArray::Handle mEraseVariables;	//!< Individual variables to erase.

	Position mPosition;							//!< Thread position in generation cycle.

	// Result
	Individual::Bag::Handle mIndividuals;
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_GenerationThread_hpp */
