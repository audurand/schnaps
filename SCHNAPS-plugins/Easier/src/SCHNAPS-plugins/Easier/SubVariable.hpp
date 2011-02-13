/*
 * SubVariable.hpp
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

#ifndef easier_SubVariable_hpp
#define easier_SubVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class SubVariable SCHNAPS-plugins/Easier/SubVariable.hpp "SCHNAPS-plugins/Easier/SubVariable.hpp"
 *  \brief Substract a value from a variable current value.
 */
class SubVariable: public core::Primitive {
public:
	//! SubVariable allocator type.
	typedef core::AllocatorT<SubVariable, core::Primitive::Alloc> Alloc;
	//! SubVariable handle type.
	typedef core::PointerT<SubVariable, core::Primitive::Handle> Handle;
	//! SubVariable bag type.
	typedef core::ContainerT<SubVariable, core::Primitive::Bag> Bag;

	SubVariable();
	SubVariable(const SubVariable& inOriginal);
	virtual ~SubVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_SubVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& SubVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;			//!< Label of concerned variable.
	std::string mValue_Ref;		//!< Value to substract (reference).
	core::Number::Handle mValue;//!< Value to substract.
};
} // end of easier namespace

#endif /* easier_SubVariable_hpp */
