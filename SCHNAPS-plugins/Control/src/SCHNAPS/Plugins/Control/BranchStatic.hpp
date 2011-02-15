/*
 * BranchStatic.hpp
 *
 *  Created on: 2010-05-15
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

#ifndef SCHNAPS_Plugins_Control_BranchStatic_hpp
#define SCHNAPS_Plugins_Control_BranchStatic_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class BranchStatic SCHNAPS/Basic/BranchStatic.hpp "SCHNAPS/Basic/BranchStatic.hpp"
 *  \brief Primitive class that returns the result of a branching with static weights.
 */
class BranchStatic: public SCHNAPS::Core::Primitive {
public:
	//! BranchStatic allocator type.
	typedef SCHNAPS::Core::AllocatorT<BranchStatic, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! BranchStatic handle type.
	typedef SCHNAPS::Core::PointerT<BranchStatic, SCHNAPS::Core::Primitive::Handle> Handle;
	//! BranchStatic bag type.
	typedef SCHNAPS::Core::ContainerT<BranchStatic, SCHNAPS::Core::Primitive::Bag> Bag;

	BranchStatic();
	BranchStatic(const BranchStatic& inOriginal);
	virtual ~BranchStatic() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_BranchStatic");
			return lName;
		schnaps_StackTraceEndM("const std::string& BranchStatic::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	BranchStatic& operator=(const BranchStatic& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;

private:
	std::string mWeights_Ref;
	SCHNAPS::Core::Vector::Handle mWeights;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_BranchStatic_hpp */
