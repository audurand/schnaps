/*
 * RandomizerMulti.hpp
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

#ifndef SCHNAPS_Core_RandomizerMulti_hpp
#define SCHNAPS_Core_RandomizerMulti_hpp

#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/Randomizer.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class RandomizerMulti SCHNAPS/Core/RandomizerMulti.hpp "SCHNAPS/Core/RandomizerMulti.hpp"
 *  \brief Multiple random number generator class.
 */
class RandomizerMulti: public Component {
public:
	//! RandomizerMulti allocator type.
	typedef AllocatorT<RandomizerMulti, Component::Alloc> Alloc;
	//! RandomizerMulti handle type.
	typedef PointerT<RandomizerMulti, Component::Handle> Handle;
	//! RandomizerMulti bag type.
	typedef ContainerT<RandomizerMulti, Component::Bag> Bag;

	RandomizerMulti();
	virtual ~RandomizerMulti() {}

	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual void init(System& ioSystem);

	/*!
	 *  \brief Get a handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	Randomizer::Handle operator[](unsigned int inIndex) {
		schnaps_StackTraceBeginM();
		return mRandomizers[inIndex];
		schnaps_StackTraceEndM("SCHNAPS::Core::Randomizer::Handle SCHNAPS::Core::Randomizer::operator[](unsigned int)");
	}

	/*!
	 *  \brief Return a const handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	const Randomizer::Handle operator[](unsigned int inIndex) const {
		schnaps_StackTraceBeginM();
		return mRandomizers[inIndex];
		schnaps_StackTraceEndM("const SCHNAPS::Core::Randomizer::Handle SCHNAPS::Core::Randomizer::operator[](unsigned int) const");
	}

protected:
	Randomizer::Bag mRandomizers; //!< Bag of random number generators, one per thread.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_RandomizerMulti_hpp
