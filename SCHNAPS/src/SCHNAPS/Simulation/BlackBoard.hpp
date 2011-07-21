/*
 * BlackBoard.hpp
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

#ifndef SCHNAPS_Simulation_BlackBoard_hpp
#define SCHNAPS_Simulation_BlackBoard_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulation/Process.hpp"

#include <list>
#include <map>

namespace SCHNAPS {
namespace Simulation {

/*!
 * \struct Push SCHNAPS/Simulation/BlackBoard.hpp "SCHNAPS/Simulation/BlackBoard.hpp"
 * \brief  The encapsulation of a process pushed at a specific time toward a specific target.
 */
struct Push {
	std::string mProcess;		//!< The name of the process pushed.
	Process::Target mTarget;	//!< The target of the push.
	unsigned long mTime;		//!< The time step of process execution.

	/*!
	 * \brief Construct a push as a copy of an original.
	 * \param inOriginal A const reference to the original push.
	 */
	Push(const Push& inOriginal) :
		mProcess(inOriginal.mProcess.c_str()),
		mTarget(inOriginal.mTarget),
		mTime(inOriginal.mTime)
	{}
	/*!
	 * \brief Construct a push with specific process, target and time.
	 * \param inProcess A const reference to the name of the process pushed.
	 * \param inTarget The target of the push.
	 * \param inTime The time step of process execution.
	 */
	Push(const std::string& inProcess, Process::Target inTarget, unsigned long inTime) :
		mProcess(inProcess.c_str()),
		mTarget(inTarget),
		mTime(inTime)
	{}
};

/*!
 * \class BlackBoard SCHNAPS/Simulation/BlackBoard.hpp "SCHNAPS/Simulation/BlackBoard.hpp"
 * \brief A blackboard where each threads indicates that a push of process must be done.
 * 		  It is necessary to ensure that threads do not access simultaneously to the same FIFOs.
 */
class BlackBoard: public Core::Object, public std::map<unsigned int, std::list<Push> > {
public:
	//! BlackBoard allocator type.
	typedef Core::AllocatorT<BlackBoard, Core::Object::Alloc> Alloc;
	//! BlackBoard handle type.
	typedef Core::PointerT<BlackBoard, Core::Object::Handle> Handle;
	//! BlackBoard bag type.
	typedef Core::ContainerT<BlackBoard, Core::Object::Bag> Bag;

	BlackBoard() {}
	BlackBoard(const BlackBoard& inOriginal);
	virtual ~BlackBoard() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("BlackBoard");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::BlackBoard::getName() const");
	}
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_BlackBoard_hpp */
