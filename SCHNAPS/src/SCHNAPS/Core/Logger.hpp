/*
 * Logger.hpp
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

#ifndef SCHNAPS_Core_Logger_hpp
#define SCHNAPS_Core_Logger_hpp

#include "SCHNAPS/gzstream.h"
#include "SCHNAPS/Core/Object.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class Logger SCHNAPS/Core/Logger.hpp "SCHNAPS/Core/Logger.hpp"
 *  \brief Logger class for writing logs to file.
 */
class Logger: public Object {
public:
	//! Logger allocator type.
	typedef AllocatorT<Logger, Object::Alloc> Alloc;
	//! Logger handle type.
	typedef PointerT<Logger, Object::Handle> Handle;
	//! Logger bag type.
	typedef ContainerT<Logger, Object::Bag> Bag;

	Logger();
	virtual ~Logger() {}

	//! Open logger file and enable logger.
	void open(const std::string& inFileName);

	//! Log message to file.
	void log(const std::string& inMessage);

	//! Close logger file and disable logger.
	void close();

private:
	bool mEnabled;		//!< True if log is enabled.
	ogzstream* mOGZS; 	//!< Output pointer.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_Logger_hpp
