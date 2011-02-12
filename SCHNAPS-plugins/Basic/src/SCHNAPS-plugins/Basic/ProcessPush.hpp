/*
 * ProcessPush.hpp
 *
 *  Created on: 2010-01-11
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

#ifndef basic_ProcessPush_hpp
#define basic_ProcessPush_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class ProcessPush SCHNAPS-plugins/Basic/ProcessPush.hpp "SCHNAPS-plugins/Basic/ProcessPush.hpp"
 *  \brief Token terminal primitive class to add a process to the execution FIFO.
 */
class ProcessPush: public core::Primitive {
public:
	//! ProcessPush allocator type.
	typedef core::AllocatorT<ProcessPush, core::Primitive::Alloc> Alloc;
	//! ProcessPush handle type.
	typedef core::PointerT<ProcessPush, core::Primitive::Handle> Handle;
	//! ProcessPush bag type.
	typedef core::ContainerT<ProcessPush, core::Primitive::Bag> Bag;

	ProcessPush();
	ProcessPush(const ProcessPush& inOriginal);
	virtual ~ProcessPush() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_ProcessPush");
			return lName;
		schnaps_StackTraceEndM("const std::string& simulator::ProcessPush::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(core::ExecutionContext& ioContext) const;

	const std::string& getLabel() const {
		return mLabel;
	}

	const simulator::Process::Target getTarget() const {
		return mTarget;
	}

	const unsigned long& getDelay() const {
		return mDelay;
	}

private:
	std::string mLabel;					//!< Label of process to push.
	simulator::Process::Target mTarget;	//!< Target of the push.
	unsigned long mDelay;				//!< Delay before execution.
};
} // end of basic namespace

#endif /* basic_ProcessPush_hpp */
