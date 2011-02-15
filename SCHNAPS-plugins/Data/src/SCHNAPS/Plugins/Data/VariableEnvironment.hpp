/*
 * VariableEnvironment.hpp
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

#ifndef SCHNAPS_Plugins_Data_VariableEnvironment_hpp
#define SCHNAPS_Plugins_Data_VariableEnvironment_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class VariableEnvironment SCHNAPS-plugins/Data/VariableEnvironment.hpp "SCHNAPS-plugins/Data/VariableEnvironment.hpp"
 *  \brief Token terminal primitive class that refers to a variable from the environment.
 */
class VariableEnvironment: public Core::Primitive {
public:
	//! VariableEnvironment allocator type.
	typedef Core::AllocatorT<VariableEnvironment, Core::Primitive::Alloc> Alloc;
	//! VariableEnvironment handle type.
	typedef Core::PointerT<VariableEnvironment, Core::Primitive::Handle> Handle;
	//! VariableEnvironment bag type.
	typedef Core::ContainerT<VariableEnvironment, Core::Primitive::Bag> Bag;

	VariableEnvironment();
	VariableEnvironment(const VariableEnvironment& inOriginal);
	virtual ~VariableEnvironment() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Data_VariableEnvironment");
			return lName;
		schnaps_StackTraceEndM("const std::string& VariableEnvironment::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(Core::ExecutionContext& ioContext) const;

	const std::string& getLabel() const {
		return mLabel;
	}

protected:
	std::string mLabel; //!< label of environment variable
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_VariableEnvironment_hpp */
