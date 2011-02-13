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

#ifndef osteoporosis_Test_hpp
#define osteoporosis_Test_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace osteoporosis {
/*!
 *  \class Test SCHNAPS-plugins/Osteoporosis/Test.hpp "SCHNAPS-plugins/Osteoporosis/Test.hpp"
 *  \brief Primitive that applies the cost of a test.
 */
class Test: public core::Primitive {
public:
	//! Test allocator type.
	typedef core::AllocatorT<Test, core::Primitive::Alloc> Alloc;
	//! Test handle type.
	typedef core::PointerT<Test, core::Primitive::Handle> Handle;
	//! Test bag type.
	typedef core::ContainerT<Test, core::Primitive::Bag> Bag;

	Test();
	Test(const Test& inOriginal);
	virtual ~Test() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_Test");
			return lName;
		schnaps_StackTraceEndM("const std::string& Test::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	core::Double::Handle mCompliance;
	core::Double::Handle mCost;
	core::Double::Handle mDiscountRate;
	std::string mCompliance_Ref;
	std::string mCost_Ref;
	std::string mDiscountRate_Ref;
	std::string mCostVariableLabel;
};
}
// end of osteoporosis namespace

#endif /* osteoporosis_Test_hpp */
