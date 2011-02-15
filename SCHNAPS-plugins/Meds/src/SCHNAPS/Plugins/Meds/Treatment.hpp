/*
 * Treatment.hpp
 *
 *  Created on: 2010-11-02
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

#ifndef SCHNAPS_Plugins_Meds_Treatment_hpp
#define SCHNAPS_Plugins_Meds_Treatment_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Treatment SCHNAPS/Plugins/Meds/Treatment.hpp "SCHNAPS/Plugins/Meds/Treatment.hpp"
 *  \brief Primitive that implements a medical treatment (or intervention) over an individual.
 */
class Treatment: public Core::Primitive {
public:
	//! Treatment allocator type.
	typedef Core::AllocatorT<Treatment, Core::Primitive::Alloc> Alloc;
	//! Treatment handle type.
	typedef Core::PointerT<Treatment, Core::Primitive::Handle> Handle;
	//! Treatment bag type.
	typedef Core::ContainerT<Treatment, Core::Primitive::Bag> Bag;

	Treatment();
	Treatment(const Treatment& inOriginal);
	virtual ~Treatment() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_Treatment");
			return lName;
		schnaps_StackTraceEndM("const std::string& Treatment::getName() const");
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

#endif /* SCHNAPS_Plugins_Meds_Treatment_hpp */
