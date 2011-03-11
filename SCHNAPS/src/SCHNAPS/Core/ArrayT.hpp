/*
 * ArrayT.hpp
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

#ifndef SCHNAPS_Core_ArrayT_hpp
#define SCHNAPS_Core_ArrayT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"
#include "SCHNAPS/Core/castObjectT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/IOException.hpp"

#include <vector>

namespace SCHNAPS {
namespace Core {

/*!
 * \class ArrayT SCHNAPS/Core/ArrayT.hpp "SCHNAPS/Core/ArrayT.hpp"
 * \author Christian Gagne
 * \author Marc Parizeau
 * \brief Template of an ArrayT that adapt an array type (std::vector of the parametrized type)
 *    to the SCHNAPS Object interface.
 * \param T The array-ed type.
 *
 * The array-ed type that is wrapped must have the following operator predefined:
 * \li bool operator<(const T&, const T&); to compare strict weak ordering two arrays.
 * \li bool operator==(const T&, const T&); to compare equality of two arrays.
 * \li std::ostream& operator<<(std::ostream&, const T&); to output the array.
 * \li std::istream& operator>>(std::istream&, T&); to input value the array.
 */
template<class T>
class ArrayT: public Object, public std::vector<T> {
public:
	//! ArrayT allocator type.
	typedef AllocatorT<ArrayT<T> , Object::Alloc> Alloc;
	//! ArrayT handle type.
	typedef PointerT<ArrayT<T> , Object::Handle> Handle;
	//! ArrayT bag type.
	typedef ContainerT<ArrayT<T> , Object::Bag> Bag;

	explicit ArrayT(unsigned int inSize = 0);
	explicit ArrayT(unsigned int inSize, const T& inModel);
	virtual ~ArrayT() {}
	
	//! Read object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write object to XML.
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if object is less than another.
	virtual bool isLess(const Object& inRightObj) const;
};
} // end of Core namespace
} // end of SCHNAPS namespace

/*!
 * \brief Constructor an array of the given size.
 * \param inSize Size of the array.
 */
template<class T>
SCHNAPS::Core::ArrayT<T>::ArrayT(unsigned int inSize) :
	std::vector<T>(inSize) {
}

/*!
 * \brief Constructor an array of the given size and model.
 * \param inSize Size of the array.
 * \param inModel Model to use to build the array.
 */
template<class T>
SCHNAPS::Core::ArrayT<T>::ArrayT(unsigned int inSize, const T& inModel) :
	std::vector<T>(inSize, inModel) {
}

/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if wrong tag is encountered.
 */
template<class T>
void SCHNAPS::Core::ArrayT<T>::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eString) {
		throw schnaps_IOExceptionNodeM(*inIter,"expected string to read array!");
	}
	std::vector<T>::clear();
	std::istringstream lISS(inIter->getValue());
	while (lISS.good()) {
		T lValue;
		lISS >> lValue;
		std::vector<T>::push_back(lValue);
		if (lISS.good() == false) {
			break;
		}
		int lDelim = lISS.get();
		if ((lISS.good() == false) || (lDelim == -1)) {
			break;
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::ArrayT<T>::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
template<class T>
void SCHNAPS::Core::ArrayT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	std::ostringstream lOSS;
	for (unsigned int i = 0; i < std::vector<T>::size(); ++i) {
		lOSS << (*this)[i];
		if (i != (std::vector<T>::size() - 1)) {
			lOSS << ',';
		}
	}
	ioStreamer.insertStringContent(lOSS.str());
	schnaps_StackTraceEndM("void SCHNAPS::Core::ArrayT<T>::write(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief  Test if an object is equal to another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is equal to right object, false if not.
 * \throw SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
template<class T>
bool SCHNAPS::Core::ArrayT<T>::isEqual(const SCHNAPS::Core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const ArrayT<T>& lLeftArray  = castObjectT<const ArrayT<T>&>(*this);
	const std::vector<T>& lLeftVector    = lLeftArray;
	const ArrayT<T>& lRightArray = castObjectT<const ArrayT<T>&>(inRightObj);
	const std::vector<T>& lRightVector   = lRightArray;
	return (lLeftVector == lRightVector);
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ArrayT<T>::isEqual(const SCHNAPS::Core::Object&) const");
}

/*!
 * \brief  Test if an object is less than another.
 * \param inRightObject A reference to the object to compare with.
 * \return True if actual object is less than right object, false if not.
 * \throw SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
template<class T>
bool SCHNAPS::Core::ArrayT<T>::isLess(const SCHNAPS::Core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
	const SCHNAPS::Core::ArrayT<T>& lLeftArray = castObjectT<const SCHNAPS::Core::ArrayT<T>&> (*this);
	const std::vector<T>& lLeftVector = lLeftArray;
	const SCHNAPS::Core::ArrayT<T>& lRightArray = castObjectT<const SCHNAPS::Core::ArrayT<T>&> (inRightObj);
	const std::vector<T>& lRightVector = lRightArray;
	return (lLeftVector < lRightVector);
	schnaps_StackTraceEndM("bool SCHNAPS::Core::ArrayT<T>::isLess(const Object&) const");
}

#endif // SCHNAPS_Core_ArrayT_hpp
