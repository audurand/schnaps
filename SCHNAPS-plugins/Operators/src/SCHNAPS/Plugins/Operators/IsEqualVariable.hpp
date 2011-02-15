/*
 * IsEqualVariable.hpp
 *
 *  Created on: 2010-11-22
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

#ifndef SCHNAPS_Plugins_Operators_IsEqualVariable_hpp
#define SCHNAPS_Plugins_Operators_IsEqualVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsEqualVariable SCHNAPS/Plugins/Operators/IsEqualVariable.hpp "SCHNAPS/Plugins/Operators/IsEqualVariable.hpp"
 *  \brief Check if variable equals value.
 */
class IsEqualVariable: public Core::Primitive {
public:
	//! IsEqualVariable allocator type.
	typedef Core::AllocatorT<IsEqualVariable, Core::Primitive::Alloc> Alloc;
	//! IsEqualVariable handle type.
	typedef Core::PointerT<IsEqualVariable, Core::Primitive::Handle> Handle;
	//! IsEqualVariable bag type.
	typedef Core::ContainerT<IsEqualVariable, Core::Primitive::Bag> Bag;

	IsEqualVariable();
	IsEqualVariable(const IsEqualVariable& inOriginal);
	virtual ~IsEqualVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_IsEqualVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsEqualVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 		//!< label of concerned variable
	std::string mValue_Ref; 	//!< value for comparison (reference)
	Core::Atom::Handle mValue;	//!< value for comparison
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsEqualVariable_hpp */
