/*
 * RouletteT.hpp
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

#ifndef SCHNAPS_Core_RouletteT_hpp
#define SCHNAPS_Core_RouletteT_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AllocatorT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/Randomizer.hpp"

#include <vector>

namespace SCHNAPS {
namespace Core {

template<class T>
class RouletteT: public Core::Object, public std::vector<std::pair<double, T> > {
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
	 * \brief Insert a value into the roulette wheel.
	 * \param inValue A const reference to the value to insert.
	 * \param inWeight The weighting to associate with the value.
	 */
	inline void insert(const T& inValue, double inWeight = 1.0) {
		schnaps_StackTraceBeginM();
		if (this->empty() == false) {
			inWeight += this->back().first;
		}
		push_back(std::make_pair(inWeight, inValue));
		schnaps_StackTraceEndM("void SCHNAPS::Core::RouletteT<T>::insert(const T&, double)");
	}

	/*!
	 * \brief Optimize distribution of values in the roulette.
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
		schnaps_StackTraceEndM("void SCHNAPS::Core::RouletteT<T>::optimize()");
	}

	/*!
	 * \brief Return a const reference to a value from the roulette wheel.
	 * \param ioRandomizer A const reference to the randomizer used to select element.
	 * \return A const reference to a value from the roulette wheel.
	 * \throw SCHNAPS::Core::AssertException if the roulette is empty.
	 */
	inline const T& select(Randomizer& ioRandomizer) const {
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
		schnaps_StackTraceEndM("const T& SCHNAPS::Core::RouletteT<T>::select(SCHNAPS::Core::Randomizer&)");
	}
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_RouletteT_hpp
