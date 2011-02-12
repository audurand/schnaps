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

#ifndef core_IsLessMapPairPredicate_hpp
#define core_IsLessMapPairPredicate_hpp

#include <string>
#include <utility>

#include "SCHNAPS/Core/Object.hpp"

namespace core {

/*!
 *  \class IsLessMapPairPredicate SCHNAPS/Core/IsLessMapPairPredicate.hpp
 *    "SCHNAPS/Core/IsLessMapPairPredicate.hpp"
 *  \brief Binary predicate functor testing the ordering of two
 *    std::pair<string,Object::Handle>.
 *  \par Note:
 *    The IsLessMapPairPredicate is compliant with the STL BinaryPredicate functor.
 *    See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessMapPairPredicate {
public:
	IsLessMapPairPredicate() {}
	~IsLessMapPairPredicate() {}

	/*!
	 *  \brief Test the ordering of two std::pair<string,Object::Handle>.
	 *  \param inLeftPair Left map pair to compare.
	 *  \param inRightPair Right map pair to compare.
	 */
	inline bool operator()(const std::pair<std::string, Object::Handle>& inLeftPair, const std::pair<std::string,
			Object::Handle>& inRightPair) const {
		schnaps_StackTraceBeginM();
			if ((inLeftPair.first < inRightPair.first) == false)
				return false;
			if ((!inLeftPair.second) || (!inRightPair.second))
				return false;
			return inLeftPair.second->isLess(*inRightPair.second);
		schnaps_StackTraceEndM(
				"bool operator()(const std::pair<std::string,Object::Handle>& inLeftPair, const std::pair<std::string,Object::Handle>& inRightPair) const");
	}
};
}

#endif // core_IsLessMapPairPredicate_hpp
