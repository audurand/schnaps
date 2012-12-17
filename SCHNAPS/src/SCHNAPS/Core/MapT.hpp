/*
 * MapT.hpp
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

#ifndef SCHNAPS_Core_MapT_hpp
#define SCHNAPS_Core_MapT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/castObjectT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/Exception.hpp"
#include "SCHNAPS/Core/IOException.hpp"

#include <map>

namespace SCHNAPS {
namespace Core {

/*!
 * \class MapT SCHNAPS/Core/MapT.hpp "SCHNAPS/Core/MapT.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Template of an MapT that adapt an map type (std::vector of the parametrized type)
 *    to the SCHNAPS Object interface.
 * \param T The map-ed type.
 *
 * The map-ed type that is wrapped must have the following operator predefined:
 * \li bool operator<(const T&, const T&); to compare strict weak ordering two maps.
 * \li bool operator==(const T&, const T&); to compare equality of two maps.
 * \li std::ostream& operator<<(std::ostream&, const T&); to output the map.
 * \li std::istream& operator>>(std::istream&, T&); to input value the map.
 */
template<class Key, class T>
class MapT: public Object, public std::map<Key, T> {
public:
	//! MapT allocator type.
	typedef AllocatorT<MapT<Key, T> , Object::Alloc> Alloc;
	//! MapT handle type.
	typedef PointerT<MapT<Key, T> , Object::Handle> Handle;
	//! MapT bag type.
	typedef ContainerT<MapT<Key, T> , Object::Bag> Bag;

	explicit MapT(unsigned int inSize = 0);
	explicit MapT(unsigned int inSize, const T& inModel);
	virtual ~MapT() {}

	//! Test if object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if object is less than another.
	virtual bool isLess(const Object& inRightObj) const;
};


/*!
 * \brief Constructor an map of the given size.
 * \param inSize Size of the map.
 */
template<class Key, class T>
MapT<Key, T>::MapT(unsigned int inSize) :
	std::map<Key, T>(inSize) {
}

/*!
 * \brief Constructor an map of the given size and model.
 * \param inSize Size of the map.
 * \param inModel Model to use to build the map.
 */
template<class Key, class T>
MapT<Key, T>::MapT(unsigned int inSize, const T& inModel) :
	std::map<Key, T>(inSize, inModel) {
}

/*!
 * \brief Compare the equality of two map of a given type.
 * \param inRightObj Object reference to the right object to compare.
 * \return True if maps are equal, false if not.
 */
template<class Key, class T>
bool MapT<Key, T>::isEqual(const SCHNAPS::Core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const SCHNAPS::Core::MapT<Key, T>& lLeftArray = castObjectT<const SCHNAPS::Core::MapT<Key, T>&> (*this);
	const std::map<Key, T>& lLeftVector = lLeftArray;
	const SCHNAPS::Core::MapT<Key, T>& lRightArray = castObjectT<const SCHNAPS::Core::MapT<Key, T>&> (inRightObj);
	const std::map<Key, T>& lRightVector = lRightArray;
	return (lLeftVector == lRightVector);
	schnaps_StackTraceEndM("bool SCHNAPS::Core::MapT<Key, T>::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief Get the strict weak order of two wrapper of a given type.
 * \param inRightObj Object reference to the right object from which we get the order.
 * \return True if left (this) map is less than the right one, false if not.
 */
template<class Key, class T>
bool MapT<Key, T>::isLess(const SCHNAPS::Core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const SCHNAPS::Core::MapT<Key, T>& lLeftArray = castObjectT<const SCHNAPS::Core::MapT<Key, T>&> (*this);
	const std::map<Key, T>& lLeftVector = lLeftArray;
	const SCHNAPS::Core::MapT<Key, T>& lRightArray = castObjectT<const SCHNAPS::Core::MapT<Key, T>&> (inRightObj);
	const std::map<Key, T>& lRightVector = lRightArray;
	return (lLeftVector < lRightVector);
	schnaps_StackTraceEndM("bool SCHNAPS::Core::MapT<Key, T>::isLess(const SCHNAPS::Core::Object&) const");
}
} // end of Core namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Core_MapT_hpp */
