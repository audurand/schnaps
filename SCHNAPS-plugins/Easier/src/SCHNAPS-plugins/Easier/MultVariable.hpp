/*
 * MultVariable.hpp
 *
 *  Created on: 2010-11-20
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

#ifndef easier_MultVariable_hpp
#define easier_MultVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class MultVariable SCHNAPS-plugins/Easier/MultVariable.hpp "SCHNAPS-plugins/Easier/MultVariable.hpp"
 *  \brief Multiply a variable by value.
 */
class MultVariable: public core::Primitive {
public:
	//! MultVariable allocator type.
	typedef core::AllocatorT<MultVariable, core::Primitive::Alloc> Alloc;
	//! MultVariable handle type.
	typedef core::PointerT<MultVariable, core::Primitive::Handle> Handle;
	//! MultVariable bag type.
	typedef core::ContainerT<MultVariable, core::Primitive::Bag> Bag;

	MultVariable();
	MultVariable(const MultVariable& inOriginal);
	virtual ~MultVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_MultVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& MultVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 		//!< Label of concerned variable.
	std::string mValue_Ref; 	//!< Value to multiply with (reference).
	core::Number::Handle mValue;//!< Value to multiply with.
};
} // end of easier namespace

#endif /* easier_MultVariable_hpp */
