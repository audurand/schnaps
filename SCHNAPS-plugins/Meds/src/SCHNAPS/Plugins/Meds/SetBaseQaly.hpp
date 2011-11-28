/*
 * SetBaseQaly.hpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#ifndef SCHNAPS_Plugins_Meds_SetBaseQaly_hpp
#define SCHNAPS_Plugins_Meds_SetBaseQaly_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class SetBaseQaly SCHNAPS/Plugins/Meds/SetBaseQaly.hpp "SCHNAPS/Plugins/Meds/SetBaseQaly.hpp"
 *  \brief Set base QALY if new base QALY is smaller.
 */
class SetBaseQaly: public Core::Primitive {
public:
	//! SetBaseQaly allocator type.
	typedef Core::AllocatorT<SetBaseQaly, Core::Primitive::Alloc> Alloc;
	//! SetBaseQaly handle type.
	typedef Core::PointerT<SetBaseQaly, Core::Primitive::Handle> Handle;
	//! SetBaseQaly bag type.
	typedef Core::ContainerT<SetBaseQaly, Core::Primitive::Bag> Bag;

	SetBaseQaly();
	SetBaseQaly(const SetBaseQaly& inOriginal);
	virtual ~SetBaseQaly() {}

	//! Copy operator.
	SetBaseQaly& operator=(const SetBaseQaly& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_SetBaseQaly");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::SetBaseQaly::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mBaseQaly_Ref;		//!< Reference to the base qaly destination.
	std::string mQaly_Ref;			//!< Reference to the new base qaly value.
	Core::Double::Handle mQaly;		//!< A handle to the new base qaly value.
	
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_SetBaseQaly_hpp */
