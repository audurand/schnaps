/*
 * SetVariable.hpp
 *
 *  Created on: 2009-02-26
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

#ifndef basic_SetVariable_hpp
#define basic_SetVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class SetVariable SCHNAPS-plugins/Basic/SetVariable.hpp "SCHNAPS-plugins/Basic/SetVariable.hpp"
 *  \brief Assign a value to a variable primitive class.
 */
class SetVariable: public core::Primitive {
public:
	//! SetVariable allocator type.
	typedef core::AllocatorT<SetVariable, core::Primitive::Alloc> Alloc;
	//! SetVariable handle type.
	typedef core::PointerT<SetVariable, core::Primitive::Handle> Handle;
	//! SetVariable bag type.
	typedef core::ContainerT<SetVariable, core::Primitive::Bag> Bag;

	SetVariable();
	SetVariable(const SetVariable& inOriginal);
	virtual ~SetVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_SetVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& SetVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; //!< Label of variable to set.
};
} // end of basic namespace

#endif /* basic_SetVariable_hpp */
