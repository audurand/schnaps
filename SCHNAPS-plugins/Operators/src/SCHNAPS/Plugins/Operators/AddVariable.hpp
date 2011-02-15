/*
 * AddVariable.hpp
 *
 *  Created on: 2010-11-20
 *  Updated on: 2010-11-20
 *      Author: Audrey Durand
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

#ifndef SCHNAPS_Plugins_Operators_AddVariable_hpp
#define SCHNAPS_Plugins_Operators_AddVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class AddVariable EasierPlugin/include/AddVariable.hpp "EasierPlugin/include/AddVariable.hpp"
 *  \brief Add a value to a variable current value.
 */
class AddVariable: public Core::Primitive {
public:
	//! AddVariable allocator type.
	typedef Core::AllocatorT<AddVariable, Core::Primitive::Alloc> Alloc;
	//! AddVariable handle type.
	typedef Core::PointerT<AddVariable, Core::Primitive::Handle> Handle;
	//! AddVariable bag type.
	typedef Core::ContainerT<AddVariable, Core::Primitive::Bag> Bag;

	AddVariable();
	AddVariable(const AddVariable& inOriginal);
	virtual ~AddVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_AddVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& AddVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;			//!< label of concerned variable
	std::string mValue_Ref; 	//!< value to add (reference)
	Core::Number::Handle mValue;//!< value to add
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_AddVariable_hpp */
