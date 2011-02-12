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

#ifndef basic_VariableEnvironment_hpp
#define basic_VariableEnvironment_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {

/*!
 *  \class VariableEnvironment SCHNAPS-plugins/Basic/VariableEnvironment.hpp "SCHNAPS-plugins/Basic/VariableEnvironment.hpp"
 *  \brief Token terminal primitive class that refers to a variable from the environment.
 */
class VariableEnvironment: public core::Primitive {
public:
	//! VariableEnvironment allocator type.
	typedef core::AllocatorT<VariableEnvironment, core::Primitive::Alloc> Alloc;
	//! VariableEnvironment handle type.
	typedef core::PointerT<VariableEnvironment, core::Primitive::Handle> Handle;
	//! VariableEnvironment bag type.
	typedef core::ContainerT<VariableEnvironment, core::Primitive::Bag> Bag;

	VariableEnvironment();
	VariableEnvironment(const VariableEnvironment& inOriginal);
	virtual ~VariableEnvironment() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_VariableEnvironment");
			return lName;
		schnaps_StackTraceEndM("const std::string& VariableEnvironment::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(core::ExecutionContext& ioContext) const;

	const std::string& getLabel() const {
		return mLabel;
	}

protected:
	std::string mLabel; //!< Label of environment variable.
};
} // end of basic namespace

#endif /* basic_VariableEnvironment_hpp */
