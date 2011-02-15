/*
 * NPV.h
 *
 *  Created on: 2010-04-20
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

#ifndef SCHNAPS_Plugins_Operators_NPV_hpp
#define SCHNAPS_Plugins_Operators_NPV_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class NPV SCHNAPS/Plugins/Operators/NPV.hpp "SCHNAPS/Plugins/Operators/NPV.hpp"
 *  \brief Primitive class that actualizes a value to a specific rate according to current time unit.
 */
class NPV: public Core::Primitive {
public:
	//! NPV allocator type.
	typedef Core::AllocatorT<NPV, Core::Primitive::Alloc> Alloc;
	//! NPV handle type.
	typedef Core::PointerT<NPV, Core::Primitive::Handle> Handle;
	//! NPV bag type.
	typedef Core::ContainerT<NPV, Core::Primitive::Bag> Bag;

	NPV();
	NPV(const NPV& inOriginal);
	virtual ~NPV() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_NPV");
			return lName;
		schnaps_StackTraceEndM("const std::string& NPV::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const;

	NPV& operator=(const NPV& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mRate_Ref;
	Core::Double::Handle mRate; //!< rate of actualization
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_NPV_hpp */
