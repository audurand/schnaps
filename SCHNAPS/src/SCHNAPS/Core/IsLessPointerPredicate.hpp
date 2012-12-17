/*
 * IsLessPointerPredicate.hpp
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

#ifndef SCHNAPS_Core_IsLessPointerPredicate_hpp
#define SCHNAPS_Core_IsLessPointerPredicate_hpp

#include "SCHNAPS/Core/Object.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 * \class IsLessPointerPredicate SCHNAPS/Core/IsLessPointerPredicate.hpp "SCHNAPS/Core/IsLessPointerPredicate.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Binary predicate functor that test the ordering of two pointed Objects.
 * \par Note:
 *   The IsLessPointerPredicate is compliant with the STL BinaryPredicate functor.
 *   See http://www.sgi.com/tech/stl/BinaryPredicate.html
 */
class IsLessPointerPredicate {
public:
	IsLessPointerPredicate() {}
	~IsLessPointerPredicate() {}

	/*!
	 * \brief Test the ordering of two pointed Objects.
	 * \param inLeftPtr Pointer to the left object to compare.
	 * \param inRightPtr Pointer to the right object to compare.
	 */
	inline bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const {
		schnaps_StackTraceBeginM();
		if ((!inLeftPtr) || (!inRightPtr))
			return false;
		return inLeftPtr->isLess(*inRightPtr);
		schnaps_StackTraceEndM("bool operator()(const Pointer& inLeftPtr, const Pointer& inRightPtr) const");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_IsLessPointerPredicate_hpp
