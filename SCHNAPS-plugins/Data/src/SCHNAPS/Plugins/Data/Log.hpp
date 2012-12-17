/*
 * Log.hpp
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

#ifndef SCHNAPS_Plugins_Data_Log_hpp
#define SCHNAPS_Plugins_Data_Log_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class Log SCHNAPS/Plugins/Data/Log.hpp "SCHNAPS/Plugins/Data/Log.hpp"
 *  \brief Log a new entry in the current logger.
 */
class Log: public Core::Primitive {
public:
	//! Log allocator type.
	typedef Core::AllocatorT<Log, Core::Primitive::Alloc> Alloc;
	//! Log handle type.
	typedef Core::PointerT<Log, Core::Primitive::Handle> Handle;
	//! Log bag type.
	typedef Core::ContainerT<Log, Core::Primitive::Bag> Bag;

	Log();
	Log(const Log& inOriginal);
	virtual ~Log() {}

	//! Copy operator.
	Log& operator=(const Log& inOriginal);

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Data_Log");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Plugins::Data::Log::getName() const");
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
	std::string mType_Ref;			//!< Reference to the type of log.
	Core::AnyType::Handle mType;	//!< A handle to the type of log.
	std::string mMessage_Ref;		//!< Reference to the message to log.
	Core::AnyType::Handle mMessage;	//!< A handle to the message to log.
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_Log_hpp */
