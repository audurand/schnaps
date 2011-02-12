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

#ifndef core_ArrayT_hpp
#define core_ArrayT_hpp

#include <vector>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Exception.hpp"
#include "SCHNAPS/Core/castObjectT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/IOException.hpp"

namespace core {

/*!
 *  \class ArrayT SCHNAPS/Core/ArrayT.hpp "SCHNAPS/Core/ArrayT.hpp"
 *  \brief Template of an ArrayT that adapt an array type (std::vector of the parametrized type)
 *     to the SCHNAPS Object interface.
 *  \param T The array-ed type.
 *
 *  The array-ed type that is wrapped must have the following operator predefined:
 *  \li bool operator<(const T&, const T&); to compare strict weak ordering two arrays.
 *  \li bool operator==(const T&, const T&); to compare equality of two arrays.
 *  \li std::ostream& operator<<(std::ostream&, const T&); to output the array.
 *  \li std::istream& operator>>(std::istream&, T&); to input value the array.
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

	//! Compare the equality of two array of a given type.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Get the strict weak order of two wrapper of a given type.
	virtual bool isLess(const Object& inRightObj) const;
	//! Get the strict weak order of two wrapper of a given type.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write a array into a XML streamer.
	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;
};
}

/*!
 *  \brief Constructor an array of the given size.
 *  \param inSize Size of the array.
 */
template<class T>
core::ArrayT<T>::ArrayT(unsigned int inSize) :
	std::vector<T>(inSize) {
}

/*!
 *  \brief Constructor an array of the given size and model.
 *  \param inSize Size of the array.
 *  \param inModel Model to use to build the array.
 */
template<class T>
core::ArrayT<T>::ArrayT(unsigned int inSize, const T& inModel) :
	std::vector<T>(inSize, inModel) {
}

/*!
 *  \brief Compare the equality of two array of a given type.
 *  \param inRightObj Object reference to the right object to compare.
 *  \return True if arrays are equal, false if not.
 */
template<class T>
bool core::ArrayT<T>::isEqual(const core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
			const ArrayT<T>& lLeftArray  = castObjectT<const ArrayT<T>&>(*this);
			const std::vector<T>& lLeftVector    = lLeftArray;
			const ArrayT<T>& lRightArray = castObjectT<const ArrayT<T>&>(inRightObj);
			const std::vector<T>& lRightVector   = lRightArray;
			return (lLeftVector == lRightVector);
	schnaps_StackTraceEndM("bool ArrayT<T>::isEqual(const Object& inRightObj) const");
}

/*!
 *  \brief Get the strict weak order of two wrapper of a given type.
 *  \param inRightObj Object reference to the right object from which we get the order.
 *  \return True if left (this) array is less than the right one, false if not.
 */
template<class T>
bool core::ArrayT<T>::isLess(const core::Object& inRightObj) const {
	schnaps_StackTraceBeginM();
		const core::ArrayT<T>& lLeftArray = castObjectT<const core::ArrayT<T>&> (*this);
		const std::vector<T>& lLeftVector = lLeftArray;
		const core::ArrayT<T>& lRightArray = castObjectT<const core::ArrayT<T>&> (inRightObj);
		const std::vector<T>& lRightVector = lRightArray;
		return (lLeftVector < lRightVector);
	schnaps_StackTraceEndM("bool ArrayT<T>::isLess(const Object& inRightObj) const");
}

/*!
 *  \brief Read a array from a XML iterator.
 *  \param inIter XML iterator from which the array is read.
 *  \throw IOException If a problem occurred during the operation (usually bad format).
 */
template<class T>
void core::ArrayT<T>::read(PACC::XML::ConstIterator inIter) {
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
	schnaps_StackTraceEndM("void ArrayT<T>::read(PACC::XML::ConstIterator inIter)");
}

/*!
 *  \brief Write a array into a XML streamer.
 *  \param ioStreamer XML streamer into which the array is written.
 *  \param inIndent Whether output should be indented.
 */
template<class T>
void core::ArrayT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		std::ostringstream lOSS;
		for (unsigned int i = 0; i < std::vector<T>::size(); ++i) {
			lOSS << (*this)[i];
			if (i != (std::vector<T>::size() - 1)) {
				lOSS << ',';
			}
		}
		ioStreamer.insertStringContent(lOSS.str());
	schnaps_StackTraceEndM("void ArrayT<T>::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

#endif // core_ArrayT_hpp
