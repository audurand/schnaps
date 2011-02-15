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

#ifndef SCHNAPS_Plugins_Data_Variable_hpp
#define SCHNAPS_Plugins_Data_Variable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class Variable SCHNAPS-plugins/Data/Variable.hpp "SCHNAPS-plugins/Data/Variable.hpp"
 *  \brief Token terminal primitive class that refers to a variable from individuals.
 */
class Variable: public Core::Primitive {
public:
	//! Variable allocator type.
	typedef Core::AllocatorT<Variable, Core::Primitive::Alloc> Alloc;
	//! Variable handle type.
	typedef Core::PointerT<Variable, Core::Primitive::Handle> Handle;
	//! Variable bag type.
	typedef Core::ContainerT<Variable, Core::Primitive::Bag> Bag;

	Variable();
	Variable(const Variable& inOriginal);
	virtual ~Variable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Data_Variable");
			return lName;
		schnaps_StackTraceEndM("const std::string& Variable::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

protected:
	std::string mLabel; //!< label of individuals variable
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_Variable_hpp */
