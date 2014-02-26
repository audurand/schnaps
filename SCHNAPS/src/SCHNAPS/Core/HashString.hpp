/*
 * HashString.hpp
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

#ifndef SCHNAPS_Core_HashString_hpp
#define SCHNAPS_Core_HashString_hpp

#include "SCHNAPS/config.hpp"
#include "SCHNAPS/macros.hpp"

#include <string>

#ifdef SCHNAPS_HAVE_STDINT_H
#include "stdint.h"
#else // SCHNAPS_HAVE_STDINT_H
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#endif // SCHNAPS_HAVE_STDINT_H

// assumes little-endian notation
#define get16bits(d) (*((const uint16_t *) (d)))

namespace SCHNAPS {
namespace Core {

/*!
 * \class HashString SCHNAPS/Core/HashString.hpp "SCHNAPS/Core/HashString.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Functor used to generate hash value from std::string.
 */
class HashString {
public:
	HashString() {}

	/*!
	 * \brief Compute the hash number of a STL string.
	 * \param inString String to hash
	 * \return Hash number.
	 *
	 * This hash function is a light adaptation of the SuperFastHash
	 * function by Paul Hsieh, available online at
	 * http://www.azillionmonkeys.com/qed/hash.html.
	 *  Complete copyright of that function is retained by the author.
	 */
	inline unsigned int operator()(const std::string& inString) const {
		const char* data = inString.data();
		int len = (int) inString.size();

		uint32_t hash = len, tmp;
		int rem;

		if (len <= 0 || data == NULL)
			return 0;

		rem = len & 3;
		len >>= 2;

		// main loop
		for (; len > 0; len--) {
			hash += get16bits (data);
			tmp = (get16bits (data+2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			data += 2 * sizeof(uint16_t);
			hash += hash >> 11;
		}

		// handle end cases
		switch (rem) {
		case 3:
			hash += get16bits (data);
			hash ^= hash << 16;
			hash ^= data[sizeof(uint16_t)] << 18;
			hash += hash >> 11;
			break;
		case 2:
			hash += get16bits (data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1:
			hash += *data;
			hash ^= hash << 10;
			hash += hash >> 1;
		}

		// force "avalanching" of final 127 bits
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_HashString_hpp
