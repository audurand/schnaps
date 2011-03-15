/*
 * RandomUniform.hpp
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

#ifndef SCHNAPS_Plugins_Data_RandomUniform_hpp
#define SCHNAPS_Plugins_Data_RandomUniform_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class RandomUniform SCHNAPS/Plugins/Data/RandomUniform.hpp "SCHNAPS/Plugins/Data/RandomUniform.hpp"
 *  \brief Get a random number according to a uniform distribution [0,1[.
 */
class RandomUniform: public Core::Primitive {
public:
	//! RandomUniform allocator type.
	typedef Core::AllocatorT<RandomUniform, Core::Primitive::Alloc> Alloc;
	//! RandomUniform handle type.
	typedef Core::PointerT<RandomUniform, Core::Primitive::Handle> Handle;
	//! RandomUniform bag type.
	typedef Core::ContainerT<RandomUniform, Core::Primitive::Bag> Bag;

	RandomUniform();
	RandomUniform(const RandomUniform& inOriginal);
	virtual ~RandomUniform() {}

	//! Copy operator.
	RandomUniform& operator=(const RandomUniform& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Data_RandomUniform");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::RandomUniform::getName() const");
	}

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_RandomUniform_hpp */
