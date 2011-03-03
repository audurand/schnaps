/*
 * Treatmentv2.hpp
 *
 *  Created on: 2010-12-02
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

#ifndef SCHNAPS_Plugins_Meds_Treatmentv2_hpp
#define SCHNAPS_Plugins_Meds_Treatmentv2_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Treatmentv2 SCHNAPS/Plugins/Meds/Treatmentv2.hpp "SCHNAPS/Plugins/Meds/Treatmentv2.hpp"
 *  \brief Primitive that implements a medical Treatmentv2 (or intervention) over an individual.
 */
class Treatmentv2: public Core::Primitive {
public:
	//! Treatmentv2 allocator type.
	typedef Core::AllocatorT<Treatmentv2, Core::Primitive::Alloc> Alloc;
	//! Treatmentv2 handle type.
	typedef Core::PointerT<Treatmentv2, Core::Primitive::Handle> Handle;
	//! Treatmentv2 bag type.
	typedef Core::ContainerT<Treatmentv2, Core::Primitive::Bag> Bag;

	Treatmentv2();
	Treatmentv2(const Treatmentv2& inOriginal);
	virtual ~Treatmentv2() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Treatmentv2");
		return lName;
		schnaps_StackTraceEndM("const std::string& Treatmentv2::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
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

#endif /* SCHNAPS_Plugins_Meds_Treatmentv2_hpp */
