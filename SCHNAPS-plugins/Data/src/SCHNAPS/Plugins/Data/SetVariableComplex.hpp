/*
 * SetVariableComplex.hpp
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

#ifndef SCHNAPS_Plugins_Data_SetVariableComplex_hpp
#define SCHNAPS_Plugins_Data_SetVariableComplex_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class SetVariableComplex SCHNAPS/Plugins/Data/SetVariableComplex.hpp "SCHNAPS/Plugins/Data/SetVariableComplex.hpp"
 *  \brief Assign a value to a variable primitive class.
 */
class SetVariableComplex: public Core::Primitive {
public:
	//! SetVariableComplex allocator type.
	typedef Core::AllocatorT<SetVariableComplex, Core::Primitive::Alloc> Alloc;
	//! SetVariableComplex handle type.
	typedef Core::PointerT<SetVariableComplex, Core::Primitive::Handle> Handle;
	//! SetVariableComplex bag type.
	typedef Core::ContainerT<SetVariableComplex, Core::Primitive::Bag> Bag;

	SetVariableComplex();
	SetVariableComplex(const SetVariableComplex& inOriginal);
	virtual ~SetVariableComplex() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Data_SetVariableComplex");
			return lName;
		schnaps_StackTraceEndM("const std::string& SetVariableComplex::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; //!< label of variable to set
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_SetVariableComplex_hpp */
