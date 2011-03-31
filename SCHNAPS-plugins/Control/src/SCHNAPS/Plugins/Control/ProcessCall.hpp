/*
 * ProcessCall.hpp
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

#ifndef SCHNAPS_Plugins_Control_ProcessCall_hpp
#define SCHNAPS_Plugins_Control_ProcessCall_hpp

#include "SCHNAPS/SCHNAPS.hpp"

#include "PACC/XML.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Control {

/*!
 *  \class ProcessCall SCHNAPS/Basic/ProcessCall.hpp "SCHNAPS/Basic/ProcessCall.hpp"
 *  \brief Token terminal primitive that calls a process for immediate execution.
 */
class ProcessCall: public Core::Primitive {
public:
	//! ProcessCall allocator type.
	typedef Core::AllocatorT<ProcessCall, Core::Primitive::Alloc> Alloc;
	//! ProcessCall handle type.
	typedef Core::PointerT<ProcessCall, Core::Primitive::Handle> Handle;
	//! ProcessCall bag type.
	typedef Core::ContainerT<ProcessCall, Core::Primitive::Bag> Bag;

	ProcessCall();
	ProcessCall(const ProcessCall& inOriginal);
	explicit ProcessCall(const std::string& inLabel);
	virtual ~ProcessCall() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Control_ProcessCall");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Control::ProcessCall::getName() const");
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
	std::string mLabel; //!< Label of process to call.
};
} // end of Control namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Control_ProcessCall_hpp */
