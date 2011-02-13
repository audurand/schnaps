/*
 * ConcatVariable.hpp
 *
 *  Created on: 2010-11-27
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

#ifndef easier_ConcatVariable_hpp
#define easier_ConcatVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class ConcatVariable SCHNAPS-plugins/Easier/ConcatVariable.hpp "SCHNAPS-plugins/Easier/ConcatVariable.hpp"
 *  \brief Concat a string value to a string variable.
 */
class ConcatVariable: public core::Primitive {
public:
	//! ConcatVariable allocator type.
	typedef core::AllocatorT<ConcatVariable, core::Primitive::Alloc> Alloc;
	//! ConcatVariable handle type.
	typedef core::PointerT<ConcatVariable, core::Primitive::Handle> Handle;
	//! ConcatVariable bag type.
	typedef core::ContainerT<ConcatVariable, core::Primitive::Bag> Bag;

	ConcatVariable();
	ConcatVariable(const ConcatVariable& inOriginal);
	virtual ~ConcatVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_ConcatVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& ConcatVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;			//!< Label of concerned variable.
	std::string mValue_Ref; 	//!< Value to concat (reference).
	core::String::Handle mValue;//!< Value to concat.
};
} // end of easier namespace

#endif /* easier_ConcatVariable_hpp */
