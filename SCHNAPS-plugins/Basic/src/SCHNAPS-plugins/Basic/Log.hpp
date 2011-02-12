/*
 * Log.hpp
 *
 *  Created on: 2010-02-20
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

#ifndef simulator_Log_hpp
#define simulator_Log_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {

/*!
 *  \class Log SCHNAPS-plugins/Basic/Log.hpp "SCHNAPS-plugins/Basic/Log.hpp"
 *  \brief Log a new entry in the current logger.
 */
class Log: public core::Primitive {
public:
	//! Log allocator type.
	typedef core::AllocatorT<Log, core::Primitive::Alloc> Alloc;
	//! Log handle type.
	typedef core::PointerT<Log, core::Primitive::Handle> Handle;
	//! Log bag type.
	typedef core::ContainerT<Log, core::Primitive::Bag> Bag;

	explicit Log();
	virtual ~Log() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Log");
			return lName;
		schnaps_StackTraceEndM("const std::string& Log::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mType;		//!< Type of log.
	std::string mMessage;	//!< Message to log.
};
} // end of simulator namespace

#endif /* simulator_Log_hpp */
