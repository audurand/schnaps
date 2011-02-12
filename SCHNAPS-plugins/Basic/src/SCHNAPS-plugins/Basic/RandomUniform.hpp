/*
 * RandomUniform.hpp
 *
 *  Created on: 2009-02-19
 *  Author: Audrey Durand
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

#ifndef basic_RandomUniform_hpp
#define basic_RandomUniform_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class RandomUniform SCHNAPS-plugins/Basic/Core/include/RandomUniform.hpp "SCHNAPS-plugins/Basic/Core/include/RandomUniform.hpp"
 *  \brief Token terminal primitive class that returns a random generated number.
 */
class RandomUniform: public core::Primitive {
public:
	//! RandomUniform allocator type.
	typedef core::AllocatorT<RandomUniform, core::Primitive::Alloc> Alloc;
	//! RandomUniform handle type.
	typedef core::PointerT<RandomUniform, core::Primitive::Handle> Handle;
	//! RandomUniform bag type.
	typedef core::ContainerT<RandomUniform, core::Primitive::Bag> Bag;

	RandomUniform();
	RandomUniform(const RandomUniform& inOriginal);
	virtual ~RandomUniform() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_RandomUniform");
			return lName;
		schnaps_StackTraceEndM("const std::string& RandomUniform::getName() const");
	}

	RandomUniform& operator=(const RandomUniform& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;
};
} // end of basic namespace

#endif /* basic_RandomUniform_hpp */
