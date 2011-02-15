/*
 * SwitchBins.hpp
 *
 *  Created on: 2009-10-29
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

#ifndef SCHNAPS_Plugins_Control_SwitchBins_hpp
#define SCHNAPS_Plugins_Control_SwitchBins_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class SwitchBins SCHNAPS/Basic/SwitchBins.hpp "SCHNAPS/Basic/SwitchBins.hpp"
 *  \brief Primitive class that executes a switch over bins.
 */
class SwitchBins: public SCHNAPS::Core::Primitive {
public:
	//! SwitchBins allocator type.
	typedef SCHNAPS::Core::AllocatorT<SwitchBins, SCHNAPS::Core::Primitive::Alloc> Alloc;
	//! SwitchBins handle type.
	typedef SCHNAPS::Core::PointerT<SwitchBins, SCHNAPS::Core::Primitive::Handle> Handle;
	//! SwitchBins bag type.
	typedef SCHNAPS::Core::ContainerT<SwitchBins, SCHNAPS::Core::Primitive::Bag> Bag;

	SwitchBins();
	SwitchBins(const SwitchBins& inOriginal);
	virtual ~SwitchBins() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Control_SwitchBins");
			return lName;
		schnaps_StackTraceEndM("const std::string& SwitchBins::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, SCHNAPS::Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	SwitchBins& operator=(const SwitchBins& inOriginal);

	virtual SCHNAPS::Core::AnyType::Handle execute(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, SCHNAPS::Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, SCHNAPS::Core::ExecutionContext& ioContext) const;

private:
	std::string mBins_Ref;		//!< Reference of bins as parameter.
	std::string mBinsType;		//!< Type of boundaries.
	SCHNAPS::Core::Vector::Handle mBins;	//!< Bounds of bins over which to switch.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_SwitchBins_hpp */
