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

#ifndef easier_IsGreaterVariable_hpp
#define easier_IsGreaterVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class IsGreaterVariable SCHNAPS-plugins/Easier/IsGreaterVariable.hpp "SCHNAPS-plugins/Easier/IsGreaterVariable.hpp"
 *  \brief Check if variable is greater than value.
 */
class IsGreaterVariable: public core::Primitive {
public:
	//! IsGreaterVariable allocator type.
	typedef core::AllocatorT<IsGreaterVariable, core::Primitive::Alloc> Alloc;
	//! IsGreaterVariable handle type.
	typedef core::PointerT<IsGreaterVariable, core::Primitive::Handle> Handle;
	//! IsGreaterVariable bag type.
	typedef core::ContainerT<IsGreaterVariable, core::Primitive::Bag> Bag;

	IsGreaterVariable();
	IsGreaterVariable(const IsGreaterVariable& inOriginal);
	virtual ~IsGreaterVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_IsGreaterVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsGreaterVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 		//!< Label of concerned variable.
	std::string mValue_Ref; 	//!< Value for comparison (reference).
	core::Number::Handle mValue;//!< Value for comparison.
};
} // end of easier namespace

#endif /* easier_IsGreaterVariable_hpp */
