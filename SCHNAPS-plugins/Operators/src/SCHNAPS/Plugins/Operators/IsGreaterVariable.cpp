/*
 * IsGreaterVariable.hpp
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

#ifndef SCHNAPS_Plugins_Operators_IsGreaterVariable_hpp
#define SCHNAPS_Plugins_Operators_IsGreaterVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsGreaterVariable SCHNAPS/Plugins/Operators/IsGreaterVariable.hpp "SCHNAPS/Plugins/Operators/IsGreaterVariable.hpp"
 *  \brief Check if variable is greater than value.
 */
class IsGreaterVariable: public Core::Primitive {
public:
	//! IsGreaterVariable allocator type.
	typedef Core::AllocatorT<IsGreaterVariable, Core::Primitive::Alloc> Alloc;
	//! IsGreaterVariable handle type.
	typedef Core::PointerT<IsGreaterVariable, Core::Primitive::Handle> Handle;
	//! IsGreaterVariable bag type.
	typedef Core::ContainerT<IsGreaterVariable, Core::Primitive::Bag> Bag;

	IsGreaterVariable();
	IsGreaterVariable(const IsGreaterVariable& inOriginal);
	virtual ~IsGreaterVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_IsGreaterVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsGreaterVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 		//!< label of concerned variable
	std::string mValue_Ref; 	//!< value for comparison (reference)
	Core::Number::Handle mValue;//!< value for comparison
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsGreaterVariable_hpp */
