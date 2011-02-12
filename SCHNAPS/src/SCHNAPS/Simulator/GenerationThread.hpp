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

#ifndef simulator_GenerationThread_hpp
#define simulator_GenerationThread_hpp

#include "PACC/PACC.hpp"

#include "SCHNAPS/Core/String.hpp"
#include "SCHNAPS/Simulator/GenerationContext.hpp"

namespace simulator {

/*!
 *  \class GenerationThread Simulator/include/GenerationThread.hpp "Simulator/include/GenerationThread.hpp"
 *  \brief GenerationThread class.
 */
class GenerationThread: public core::Object, public PACC::Threading::Thread {
public:
	//! GenerationThread allocator type.
	typedef core::AllocatorT<GenerationThread, core::Object::Alloc> Alloc;
	//! GenerationThread handle type.
	typedef core::PointerT<GenerationThread, core::Object::Handle> Handle;
	//! GenerationThread bag type.
	typedef core::ContainerT<GenerationThread, core::Object::Bag> Bag;

	enum Position {eGENERATION, eEND};

	explicit GenerationThread(PACC::Threading::Condition* inParallel,
								PACC::Threading::Semaphore* inSequential,
								GenerationContext::Handle inContext);
	~GenerationThread();

	void setGenerationInfo(unsigned int inSize, std::string inPrefix, unsigned int inStartingIndex, core::StringArray::Handle inEraseVariables) {
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

	const core::StringArray& getEraseVariables() const {
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
	core::StringArray::Handle mEraseVariables;	//!< Individual variables to erase.

	Position mPosition;							//!< Thread position in generation cycle.

	// Result
	Individual::Bag::Handle mIndividuals;
};
} // end of simulator namespace

#endif /* simulator_GenerationThread_hpp */
