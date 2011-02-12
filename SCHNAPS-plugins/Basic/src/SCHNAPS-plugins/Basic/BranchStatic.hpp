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

#ifndef basic_BranchStatic_hpp
#define basic_BranchStatic_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class BranchStatic SCHNAPS-plugins/Basic/BranchStatic.hpp "SCHNAPS-plugins/Basic/BranchStatic.hpp"
 *  \brief Primitive class that returns the result of a branching with static weights.
 */
class BranchStatic: public core::Primitive {
public:
	//! BranchStatic allocator type.
	typedef core::AllocatorT<BranchStatic, core::Primitive::Alloc> Alloc;
	//! BranchStatic handle type.
	typedef core::PointerT<BranchStatic, core::Primitive::Handle> Handle;
	//! BranchStatic bag type.
	typedef core::ContainerT<BranchStatic, core::Primitive::Bag> Bag;

	BranchStatic();
	BranchStatic(const BranchStatic& inOriginal);
	virtual ~BranchStatic() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_BranchStatic");
			return lName;
		schnaps_StackTraceEndM("const std::string& BranchStatic::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	BranchStatic& operator=(const BranchStatic& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mWeights_Ref;
	core::Vector::Handle mWeights;
};
} // end of basic namespace

#endif /* basic_BranchStatic_hpp */
