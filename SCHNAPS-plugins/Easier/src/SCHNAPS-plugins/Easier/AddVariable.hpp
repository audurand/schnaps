/*
 * AddVariable.hpp
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

#ifndef easier_AddVariable_hpp
#define easier_AddVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace easier {
/*!
 *  \class AddVariable SCHNAPS-plugins/Easier/AddVariable.hpp "SCHNAPS-plugins/Easier/AddVariable.hpp"
 *  \brief Add a value to a variable current value.
 */
class AddVariable: public core::Primitive {
public:
	//! AddVariable allocator type.
	typedef core::AllocatorT<AddVariable, core::Primitive::Alloc> Alloc;
	//! AddVariable handle type.
	typedef core::PointerT<AddVariable, core::Primitive::Handle> Handle;
	//! AddVariable bag type.
	typedef core::ContainerT<AddVariable, core::Primitive::Bag> Bag;

	AddVariable();
	AddVariable(const AddVariable& inOriginal);
	virtual ~AddVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_AddVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& AddVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;			//!< Label of concerned variable.
	std::string mValue_Ref; 	//!< Value to add (reference).
	core::Number::Handle mValue;//!< Value to add.
};
} // end of easier namespace

#endif /* easier_AddVariable_hpp */
