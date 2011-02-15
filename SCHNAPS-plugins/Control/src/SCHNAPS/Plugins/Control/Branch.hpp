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

#ifndef SCHNAPS_Plugins_Control_Branch_hpp
#define SCHNAPS_Plugins_Control_Branch_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class Branch SCHNAPS-plugins/Easier/Branch.hpp "SCHNAPS-plugins/Easier/Branch.hpp"
 *  \brief Executes the first branch with some probability, else second branch.
 */
class Branch: public Core::Primitive {
public:
	//! Branch allocator type.
	typedef Core::AllocatorT<Branch, Core::Primitive::Alloc> Alloc;
	//! Branch handle type.
	typedef Core::PointerT<Branch, Core::Primitive::Handle> Handle;
	//! Branch bag type.
	typedef Core::ContainerT<Branch, Core::Primitive::Bag> Bag;

	Branch();
	Branch(const Branch& inOriginal);
	virtual ~Branch() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_Branch");
			return lName;
		schnaps_StackTraceEndM("const std::string& Branch::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Branch& operator=(const Branch& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mProbability_Ref;
	Core::Double::Handle mProbability;
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_Branch_hpp */
