/*
 * PreventionCampain.hpp
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

#ifndef osteoporosis_PreventionCampain_hpp
#define osteoporosis_PreventionCampain_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace osteoporosis {
/*!
 *  \class PreventionCampain SCHNAPS-plugins/Osteoporosis/PreventionCampain.hpp "SCHNAPS-plugins/Osteoporosis/PreventionCampain.hpp"
 *  \brief Primitive that implements a prevention campain on the population.
 */
class PreventionCampain: public core::Primitive {
public:
	//! PreventionCampain allocator type.
	typedef core::AllocatorT<PreventionCampain, core::Primitive::Alloc> Alloc;
	//! PreventionCampain handle type.
	typedef core::PointerT<PreventionCampain, core::Primitive::Handle> Handle;
	//! PreventionCampain bag type.
	typedef core::ContainerT<PreventionCampain, core::Primitive::Bag> Bag;

	PreventionCampain();
	PreventionCampain(const PreventionCampain& inOriginal);
	virtual ~PreventionCampain() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Osteo_PreventionCampain");
			return lName;
		schnaps_StackTraceEndM("const std::string& PreventionCampain::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	core::Double::Handle mCost;
	core::Double::Handle mDiscountRate;
	std::string mCost_Ref;
	std::string mDiscountRate_Ref;
	std::string mCostVariableLabel;
};
}
// end of osteoporosis namespace

#endif /* osteoporosis_PreventionCampain_hpp */
