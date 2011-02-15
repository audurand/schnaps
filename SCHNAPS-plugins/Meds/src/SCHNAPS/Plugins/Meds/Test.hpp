/*
 * Test.hpp
 *
 *  Created on: 2010-11-18
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

#ifndef SCHNAPS_Plugins_Meds_Test_hpp
#define SCHNAPS_Plugins_Meds_Test_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Test SCHNAPS/Plugins/Meds/Test.hpp "SCHNAPS/Plugins/Meds/Test.hpp"
 *  \brief Primitive that applies the cost of a test.
 */
class Test: public Core::Primitive {
public:
	//! Test allocator type.
	typedef Core::AllocatorT<Test, Core::Primitive::Alloc> Alloc;
	//! Test handle type.
	typedef Core::PointerT<Test, Core::Primitive::Handle> Handle;
	//! Test bag type.
	typedef Core::ContainerT<Test, Core::Primitive::Bag> Bag;

	Test();
	Test(const Test& inOriginal);
	virtual ~Test() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_Test");
			return lName;
		schnaps_StackTraceEndM("const std::string& Test::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	Core::Double::Handle mCompliance;
	Core::Double::Handle mCost;
	Core::Double::Handle mDiscountRate;
	std::string mCompliance_Ref;
	std::string mCost_Ref;
	std::string mDiscountRate_Ref;
	std::string mCostVariableLabel;
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Test_hpp */
