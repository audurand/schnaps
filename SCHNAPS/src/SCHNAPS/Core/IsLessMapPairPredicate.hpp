/*
 * IsLessMapPairPredicate.hpp
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

#ifndef SCHNAPS_Core_IsLessMapPairPredicate_hpp
#define SCHNAPS_Core_IsLessMapPairPredicate_hpp

#include "SCHNAPS/Core/Object.hpp"

#include <string>
#include <utility>

namespace SCHNAPS {
namespace Core {

/*!
 * \class IsLessMapPairPredicate SCHNAPS/Core/IsLessMapPairPredicate.hpp "SCHNAPS/Core/IsLessMapPairPredicate.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Binary predicate functor testing the ordering of two
 *   std::pair<string,Object::Handle>.
 * \par Note:
 *   The IsLessMapPairPredicate is compliant with the STL BinaryPredicate functor.
 *   See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessMapPairPredicate {
public:
	IsLessMapPairPredicate() {}
	~IsLessMapPairPredicate() {}

	/*!
	 * \brief Test the ordering of two std::pair<string,Object::Handle>.
	 * \param inLeftPair Left map pair to compare.
	 * \param inRightPair Right map pair to compare.
	 */
	inline bool operator()(const std::pair<std::string, Object::Handle>& inLeftPair, const std::pair<std::string, Object::Handle>& inRightPair) const {
		schnaps_StackTraceBeginM();
		if ((inLeftPair.first < inRightPair.first) == false)
			return false;
		if ((!inLeftPair.second) || (!inRightPair.second))
			return false;
		return inLeftPair.second->isLess(*inRightPair.second);
		schnaps_StackTraceEndM( "bool operator()(const std::pair<std::string, SCHNAPS::Core::Object::Handle>& inLeftPair, const std::pair<std::string, SCHNAPS::Core::Object::Handle>& inRightPair) const");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_IsLessMapPairPredicate_hpp
