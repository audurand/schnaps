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
 */

#ifndef core_HashString_hpp
#define core_HashString_hpp

#include <string>

#include "SCHNAPS/config.hpp"
#include "SCHNAPS/macros.hpp"

#ifdef SCHNAPS_HAVE_STDINT_H
#include "stdint.h"
#else // SCHNAPS_HAVE_STDINT_H
#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#endif // SCHNAPS_HAVE_STDINT_H

// assumes little-endian notation
#define get16bits(d) (*((const uint16_t *) (d)))

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#endif // defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)

#ifdef SCHNAPS_HAVE_GNUCXX_HASHMAP
#include <ext/hash_map>
#endif // SCHNAPS_HAVE_GNUCXX_HASHMAP

namespace core {

/*!
 *  \class HashString SCHNAPS/Core/HashString.hpp "SCHNAPS/Core/HashString.hpp"
 *  \brief Functor used to generate hash value from std::string.
 */
class HashString {
public:
	HashString() {}

	/*!
	 *  \brief Compute the hash number of a STL string.
	 *  \param inString String to hash
	 *  \return Hash number.
	 *
	 *  This hash function is a light adaptation of the SuperFastHash
	 *  function by Paul Hsieh, available online at
	 *  http://www.azillionmonkeys.com/qed/hash.html.
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

		/* Main loop */
		for (; len > 0; len--) {
			hash += get16bits (data);
			tmp = (get16bits (data+2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			data += 2 * sizeof(uint16_t);
			hash += hash >> 11;
		}

		/* Handle end cases */
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

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}
};
}

#endif // core_HashString_hpp
