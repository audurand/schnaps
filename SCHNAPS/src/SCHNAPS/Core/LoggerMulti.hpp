/*
 * LoggerMulti.hpp
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

#ifndef SCHNAPS_Core_LoggerMulti_hpp
#define SCHNAPS_Core_LoggerMulti_hpp

#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/Logger.hpp"

namespace SCHNAPS {
namespace Core {

/*!
 *  \class LoggerMulti SCHNAPS/Core/LoggerMulti.hpp "SCHNAPS/Core/LoggerMulti.hpp"
 *  \brief Multiple logger class.
 */
class LoggerMulti: public Component {
public:
	//! LoggerMulti allocator type.
	typedef AllocatorT<LoggerMulti, Component::Alloc> Alloc;
	//! LoggerMulti handle type.
	typedef PointerT<LoggerMulti, Component::Handle> Handle;
	//! LoggerMulti bag type.
	typedef ContainerT<LoggerMulti, Component::Bag> Bag;

	LoggerMulti();
	virtual ~LoggerMulti() {}

	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Initialize the component.
	virtual void init(System& ioSystem);

	/*!
	 * \brief Return a handle to the logger associated to a specific index.
	 * \param inIndex Index of the logger.
	 * \return A handle to the logger associated to a specific index.
	 */
	inline Logger::Handle operator[](unsigned int inIndex) {
		return (*mLoggers)[inIndex];
	}

	/*!
	 * \brief Return a const handle to the logger associated to a specific index.
	 * \param inIndex Index of the logger.
	 * \return A const handle to the logger associated to a specific index.
	 */
	inline const Logger::Handle operator[](unsigned int inIndex) const {
		return (*mLoggers)[inIndex];
	}

protected:
	Logger::Bag::Handle mLoggers; //!< Bag of random number generators, one per thread.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_LoggerMulti_hpp
