/*
 * ProcessCall.hpp
 *
 *  Created on: 2009-01-11
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

#ifndef basic_ProcessCall_hpp
#define basic_ProcessCall_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace basic {
/*!
 *  \class ProcessCall SCHNAPS-plugins/Basic/ProcessCall.hpp "SCHNAPS-plugins/Basic/ProcessCall.hpp"
 *  \brief Token terminal primitive that calls a process for immediate execution.
 */
class ProcessCall: public core::Primitive {
public:
	//! ProcessCall allocator type.
	typedef core::AllocatorT<ProcessCall, core::Primitive::Alloc> Alloc;
	//! ProcessCall handle type.
	typedef core::PointerT<ProcessCall, core::Primitive::Handle> Handle;
	//! ProcessCall bag type.
	typedef core::ContainerT<ProcessCall, core::Primitive::Bag> Bag;

	ProcessCall();
	ProcessCall(const ProcessCall& inOriginal);
	explicit ProcessCall(std::string inLabel);
	virtual ~ProcessCall() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_ProcessCall");
			return lName;
		schnaps_StackTraceEndM("const std::string& ProcessCall::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(core::ExecutionContext& ioContext) const;

	/*!
	 * \brief Get the label of process to call.
	 * \return Label of process to call.
	 */
	const std::string& getLabel() const {
		return mLabel;
	}

private:
	std::string mLabel; //!< Label of process to call.
};
} // end of basic namespace

#endif /* basic_ProcessCall_hpp */
