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

#ifndef basic_SwitchBins_hpp
#define basic_SwitchBins_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class SwitchBins SCHNAPS-plugins/Basic/SwitchBins.hpp "SCHNAPS-plugins/Basic/SwitchBins.hpp"
 *  \brief Primitive class that executes a switch over bins.
 */
class SwitchBins: public core::Primitive {
public:
	//! SwitchBins allocator type.
	typedef core::AllocatorT<SwitchBins, core::Primitive::Alloc> Alloc;
	//! SwitchBins handle type.
	typedef core::PointerT<SwitchBins, core::Primitive::Handle> Handle;
	//! SwitchBins bag type.
	typedef core::ContainerT<SwitchBins, core::Primitive::Bag> Bag;

	SwitchBins();
	SwitchBins(const SwitchBins& inOriginal);
	virtual ~SwitchBins() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_SwitchBins");
			return lName;
		schnaps_StackTraceEndM("const std::string& SwitchBins::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	SwitchBins& operator=(const SwitchBins& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mBins_Ref;		//!< Reference of bins as parameter.
	std::string mBinsType;		//!< Type of boundaries.
	core::Vector::Handle mBins;	//!< Bounds of bins over which to switch.
};
} // end of basic namespace

#endif /* basic_SwitchBins_hpp */
