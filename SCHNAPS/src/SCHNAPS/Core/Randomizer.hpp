/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
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
 *  Modified by Audrey Durand.
 *  Updated on: 2010-09-30
 */

#ifndef SCHNAPS_Core_Randomizer_hpp
#define SCHNAPS_Core_Randomizer_hpp

#include <fstream>

#include "PACC/PACC.hpp"

#include "SCHNAPS/Core/Object.hpp"

namespace SCHNAPS {
	
namespace Core {

// forward declaration
class System;

/*!
 *  \class Randomizer SCHNAPS/Core/Randomizer.hpp "SCHNAPS/Core/Randomizer.hpp"
 *  \brief Random number generator class.
 *  \note The operator() allow compliance with the STL random number generator interface.
 */
class Randomizer: public Object, public PACC::Randomizer {
public:
	//! Randomizer allocator type.
	typedef AllocatorT<Randomizer, Object::Alloc> Alloc;
	//! Randomizer handle type.
	typedef PointerT<Randomizer, Object::Handle> Handle;
	//! Randomizer bag type.
	typedef ContainerT<Randomizer, Object::Bag> Bag;

	explicit Randomizer(unsigned long inSeed = 0);
	virtual ~Randomizer() {}

	/*!
	 * \brief Reset random generator to a new state.
	 * \param inSeed New seed.
	 * \param inState New state.
	 */
	void reset(unsigned long inSeed = 0, std::string inState = "") {
		if (inSeed == 0) {
#ifdef SCHNAPS_HAVE_DEV_URANDOM
			std::ifstream lURandom("/dev/urandom", std::ios::binary);
			lURandom.read((char*)&mSeed, sizeof(unsigned long));
			lURandom.close();
#else
			mSeed = MTRand::hash(time(NULL), clock());
#endif
			seed(mSeed);
		} else {
			mSeed = inSeed;
			if (inState.empty()) {
				seed(mSeed);
			} else {
				setState(inState);
			}
		}
	}

	/*!
	 * \brief Get randomizer seed.
	 * \return Randomizer seed.
	 */
	unsigned long getSeed() {
		return mSeed;
	}

	/*!
	 *  \brief Generate a floating-point number following a Gaussian distribution.
	 *  \param inMean Mean of the Gaussain distribution.
	 *  \param inStdDev Standard-error of the Gaussian distribution.
	 */
	inline double rollGaussian(double inMean = 0.0, double inStdDev = 1.0) {
		schnaps_StackTraceBeginM();
			schnaps_AssertM(inStdDev >= 0.0);
			return randNorm(inMean, inStdDev);
		schnaps_StackTraceEndM("double Randomizer::rollGaussian(double, double)");
	}

	/*!
	 *  \brief Generate an positive integer following an uniform discrete distribution.
	 *  \param inLower Lower bound of the distribution.
	 *  \param inUpper Upper bound of the distribution.
	 *  \return Random positive integer generated in [inLower, inUpper].
	 */
	inline unsigned long rollInteger(unsigned long inLower = 0, unsigned long inUpper = ULONG_MAX) {
		schnaps_StackTraceBeginM();
			schnaps_AssertM(inLower <= inUpper);
			return randInt(inUpper - inLower) + inLower;
		schnaps_StackTraceEndM("unsigned long Randomizer::rollInteger(unsigned long, unsigned long)");
	}

	/*!
	 *  \brief Generate a number following a uniform distribution in the interval [inLower,inUpper).
	 *  \param inLower Lower bound of the uniform distribution.
	 *  \param inUpper Upper bound of the uniform distribution.
	 *  \return Random number generated in [inLower,inUpper).
	 */
	inline double rollUniform(double inLower = 0.0, double inUpper = 1.0) {
		schnaps_StackTraceBeginM();
			schnaps_AssertM(inLower <= inUpper);
			return getFloat(inLower, inUpper);
		schnaps_StackTraceEndM("double Randomizer::rollUniform(double, double)");
	}

protected:
	unsigned long mSeed; //!< Seed used to initialize the random number generator.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Randomizer_hpp
