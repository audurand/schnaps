/*
 * Qaly.hpp
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

#ifndef SCHNAPS_Plugins_Meds_Qaly_hpp
#define SCHNAPS_Plugins_Meds_Qaly_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Qaly SCHNAPS/Plugins/Meds/Qaly.hpp "SCHNAPS/Plugins/Meds/Qaly.hpp"
 *  \brief Compute new QALY and possibly replace old QALY if new QALY is smaller.
 */
class Qaly: public Core::Primitive {
public:
	//! Qaly allocator type.
	typedef Core::AllocatorT<Qaly, Core::Primitive::Alloc> Alloc;
	//! Qaly handle type.
	typedef Core::PointerT<Qaly, Core::Primitive::Handle> Handle;
	//! Qaly bag type.
	typedef Core::ContainerT<Qaly, Core::Primitive::Bag> Bag;

	Qaly();
	Qaly(const Qaly& inOriginal);
	virtual ~Qaly() {}

	//! Copy operator.
	Qaly& operator=(const Qaly& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Qaly");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Qaly::getName() const");
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
	std::string mOutQaly_Ref;		//!< Reference to the output qaly destination.
	std::string mOldQaly_Ref;		//!< Reference to the old qaly value.
	std::string mQaly_Ref;			//!< Reference to the qaly value.
	Core::Double::Handle mQaly;		//!< A handle to the qaly value.
	std::string mDiscountRate_Ref;		//!< Reference to the discount rate.
	Core::Double::Handle mDiscountRate;	//!< A handle to the discount rate value.
	
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Qaly_hpp */
