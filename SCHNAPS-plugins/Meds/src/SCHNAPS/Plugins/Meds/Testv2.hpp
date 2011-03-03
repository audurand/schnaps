/*
 * Testv2.hpp
 *
 *  Created on: 2009-12-02
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

#ifndef SCHNAPS_Plugins_Meds_Testv2_hpp
#define SCHNAPS_Plugins_Meds_Testv2_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Testv2 SCHNAPS/Plugins/Meds/Testv2.hpp "SCHNAPS/Plugins/Meds/Testv2.hpp"
 *  \brief Primitive that applies a Testv2 over individuals.
 */
class Testv2: public Core::Primitive {
public:
	//! Testv2 allocator type.
	typedef Core::AllocatorT<Testv2, Core::Primitive::Alloc> Alloc;
	//! Testv2 handle type.
	typedef Core::PointerT<Testv2, Core::Primitive::Handle> Handle;
	//! Testv2 bag type.
	typedef Core::ContainerT<Testv2, Core::Primitive::Bag> Bag;

	Testv2();
	Testv2(const Testv2& inOriginal);
	virtual ~Testv2() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Testv2");
		return lName;
		schnaps_StackTraceEndM("const std::string& Testv2::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;
	Core::Double::Handle mCost;
	Core::Double::Handle mDiscountRate;
	Core::Double::Handle mSensitivity;
	Core::Double::Handle mSpecificity;
	std::string mCost_Ref;
	std::string mDiscountRate_Ref;
	std::string mSensitivity_Ref;
	std::string mSpecificity_Ref;
	std::string mCostVariableLabel;
	std::string mStateVariableLabel;
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Testv2_hpp */
