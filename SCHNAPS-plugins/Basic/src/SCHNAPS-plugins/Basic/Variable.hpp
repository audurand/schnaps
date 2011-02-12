/*
 * Variable.hpp
 *
 *  Created on: 2009-03-17
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

#ifndef basic_Variable_hpp
#define basic_Variable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Variable SCHNAPS-plugins/Basic/Variable.hpp "SCHNAPS-plugins/Basic/Variable.hpp"
 *  \brief Token terminal primitive class that refers to a variable from individuals.
 */
class Variable: public core::Primitive {
public:
	//! Variable allocator type.
	typedef core::AllocatorT<Variable, core::Primitive::Alloc> Alloc;
	//! Variable handle type.
	typedef core::PointerT<Variable, core::Primitive::Handle> Handle;
	//! Variable bag type.
	typedef core::ContainerT<Variable, core::Primitive::Bag> Bag;

	Variable();
	Variable(const Variable& inOriginal);
	virtual ~Variable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Variable");
			return lName;
		schnaps_StackTraceEndM("const std::string& Variable::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

protected:
	std::string mLabel; //!< Label of individuals variable.
};
} // end of basic namespace

#endif /* basic_Variable_hpp */
