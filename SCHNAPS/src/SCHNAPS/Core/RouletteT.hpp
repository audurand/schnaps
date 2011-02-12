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

#ifndef core_RouletteT_hpp
#define core_RouletteT_hpp

#include <vector>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/Randomizer.hpp"

namespace core {
/*!
 *  \class RouletteT RouletteT.hpp "RouletteT.hpp"
 *  \brief Templated class defining a T-type roulette wheel.
 *  \param T Type of the values in the wheel.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  \author Audrey Durand
 */
template<class T>
class RouletteT: public core::Object, public std::vector<std::pair<double, T> > {
public:
	//! RouletteT allocator type.
	typedef AllocatorT<RouletteT<T> , Object::Alloc> Alloc;
	//! RouletteT handle type.
	typedef PointerT<RouletteT<T> , Object::Handle> Handle;
	//! RouletteT bag type.
	typedef ContainerT<RouletteT<T> , Object::Bag> Bag;

	RouletteT() {}
	virtual ~RouletteT() {}

	/*!
	 *  \brief Insert a value into the roulette wheel.
	 *  \param inValue The value to be inserted.
	 *  \param inWeight The weighting to associate with the value.
	 */
	inline void insert(const T& inValue, double inWeight = 1.0) {
		schnaps_StackTraceBeginM();
			if (this->empty() == false) {
				inWeight += this->back().first;
			}
			push_back(std::make_pair(inWeight, inValue));
		schnaps_StackTraceEndM("void core::RouletteT<T>::insert(const T&, double)");
	}

	/*!
	 *  \brief Optimize distribution of values in the roulette.
	 */
	inline void optimize() {
		schnaps_StackTraceBeginM();
			if (this->size() < 2) {
				return;
			}
			for (unsigned int i = (this->size() - 1); i >= 1; --i) {
				(*this)[i].first -= (*this)[i - 1].first;
			}
			std::sort(this->begin(), this->end(), std::greater<std::pair<double, T> >());
			for (unsigned int i = 1; i < this->size(); ++i) {
				(*this)[i].first += (*this)[i - 1].first;
			}
		schnaps_StackTraceEndM("void core::RouletteT<T>::optimize()");
	}

	/*!
	 *  \brief Select a value from the roulette wheel
	 *  \param ioRandomizer Randomizer used to select element.
	 */
	inline const T& select(core::Randomizer& ioRandomizer) const {
		schnaps_StackTraceBeginM();
			schnaps_AssertM(this->empty() == false);
				const double lDice = ioRandomizer.rollUniform(0., this->back().first);
				for (unsigned int i = 0; i < this->size(); i++) {
					if (lDice < (*this)[i].first) {
						return (*this)[i].second;
					}
				}
				schnaps_AssertM(lDice < this->back().first);
				return this->back().second;
		schnaps_StackTraceEndM("const T& core::RouletteT<T>::select(core::Randomizer&)");
	}
};
}

#endif // core_RouletteT_hpp
