/*
 * Branch.hpp
 *
 *  Created on: 2010-11-21
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

#ifndef easier_Branch_hpp
#define easier_Branch_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class Branch SCHNAPS-plugins/Easier/Branch.hpp "SCHNAPS-plugins/Easier/Branch.hpp"
 *  \brief Executes the first branch with some probability, else second branch.
 */
class Branch: public core::Primitive {
public:
	//! Branch allocator type.
	typedef core::AllocatorT<Branch, core::Primitive::Alloc> Alloc;
	//! Branch handle type.
	typedef core::PointerT<Branch, core::Primitive::Handle> Handle;
	//! Branch bag type.
	typedef core::ContainerT<Branch, core::Primitive::Bag> Bag;

	Branch();
	Branch(const Branch& inOriginal);
	virtual ~Branch() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_Branch");
			return lName;
		schnaps_StackTraceEndM("const std::string& Branch::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Branch& operator=(const Branch& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mProbability_Ref;
	core::Double::Handle mProbability;
};
} // end of easier namespace

#endif /* easier_Branch_hpp */
