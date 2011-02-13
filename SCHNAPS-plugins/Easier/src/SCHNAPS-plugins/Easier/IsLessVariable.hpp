/*
 * IsLessVariable.hpp
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

#ifndef easier_IsLessVariable_hpp
#define easier_IsLessVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class IsLessVariable SCHNAPS-plugins/Easier/IsLessVariable.hpp "SCHNAPS-plugins/Easier/IsLessVariable.hpp"
 *  \brief Check if variable is less than value.
 */
class IsLessVariable: public core::Primitive {
public:
	//! IsLessVariable allocator type.
	typedef core::AllocatorT<IsLessVariable, core::Primitive::Alloc> Alloc;
	//! IsLessVariable handle type.
	typedef core::PointerT<IsLessVariable, core::Primitive::Handle> Handle;
	//! IsLessVariable bag type.
	typedef core::ContainerT<IsLessVariable, core::Primitive::Bag> Bag;

	IsLessVariable();
	IsLessVariable(const IsLessVariable& inOriginal);
	virtual ~IsLessVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_IsLessVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsLessVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 		//!< Label of concerned variable.
	std::string mValue_Ref; 	//!< Value for comparison (reference).
	core::Number::Handle mValue; //!< Value for comparison.
};
} // end of easier namespace

#endif /* easier_IsLessVariable_hpp */
