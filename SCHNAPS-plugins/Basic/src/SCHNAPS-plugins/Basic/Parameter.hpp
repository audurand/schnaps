/*
 * Parameter.hpp
 *
 *  Created on: 2010-08-03
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

#ifndef basic_Parameter_hpp
#define basic_Parameter_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Parameter SCHNAPS-plugins/Basic/Core/include/Parameter.hpp "SCHNAPS-plugins/Basic/Core/include/Parameter.hpp"
 *  \brief Returns the value of a simulation parameter.
 */
class Parameter: public core::Primitive {
public:
	//! Parameter allocator type.
	typedef core::AllocatorT<Parameter, core::Primitive::Alloc> Alloc;
	//! Parameter handle type.
	typedef core::PointerT<Parameter, core::Primitive::Handle> Handle;
	//! Parameter bag type.
	typedef core::ContainerT<Parameter, core::Primitive::Bag> Bag;

	Parameter();
	Parameter(const Parameter& inOriginal);
	virtual ~Parameter() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Parameter");
			return lName;
		schnaps_StackTraceEndM("const std::string& Parameter::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Parameter& operator=(const Parameter& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	core::AnyType::Handle mParameterHandle;
};
} // end of basic namespace

#endif /* basic_Parameter_hpp */
