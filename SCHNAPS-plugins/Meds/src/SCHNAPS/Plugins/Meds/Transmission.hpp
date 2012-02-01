/*
 * Transmission.hpp
 *
 * SCHNAPS
 * Copyright (C) 2011 by Xavier Douville
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

#ifndef SCHNAPS_Plugins_Meds_Transmission_hpp
#define SCHNAPS_Plugins_Meds_Transmission_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Meds {

/*!
 *  \class Transmission SCHNAPS/Plugins/Meds/Transmission.hpp "SCHNAPS/Plugins/Meds/Transmission.hpp"
 *  \brief Transmission of an infectious disease from an individual to its contacts
 */
class Transmission: public Core::Primitive {
public:
	//! Transmission allocator type.
	typedef Core::AllocatorT<Transmission, Core::Primitive::Alloc> Alloc;
	//! Transmission handle type.
	typedef Core::PointerT<Transmission, Core::Primitive::Handle> Handle;
	//! Transmission bag type.
	typedef Core::ContainerT<Transmission, Core::Primitive::Bag> Bag;

	Transmission();
	Transmission(const Transmission& inOriginal);
	virtual ~Transmission() {}

	//! Copy operator.
	Transmission& operator=(const Transmission& inOriginal);

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Meds_Transmission");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Meds::Transmission::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Execute the primitive.
	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type.
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, Core::ExecutionContext& ioContext) const;
	//! Return the primitive return type.
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel;					//!< Label of process to push
	std::string mContacts_Ref;			//!< Reference to contacts of the individual.
	Core::Vector::Handle mContacts;		//!< A handle to the contacts of the individual.
	std::string mProbability_Ref;		//!< Reference to the probability.
	Core::Double::Handle mProbability;	//!< A handle to the probability.
};
} // end of Meds namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Meds_Transmission_hpp */
