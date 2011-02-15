/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2009 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
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
	inline Randomizer::Handle operator[](unsigned int inIndex) {
		return (*mRandomizers)[inIndex];
	}

	/*!
	 *  \brief Return a const handle to the Randomizer associated to the index \c inIndex.
	 *  \param inIndex Index of the Randomizer.
	 */
	inline const Randomizer::Handle operator[](unsigned int inIndex) const {
		return (*mRandomizers)[inIndex];
	}

protected:
	Randomizer::Bag::Handle mRandomizers; //!< Bag of random number generators, one per thread.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_RandomizerMulti_hpp
