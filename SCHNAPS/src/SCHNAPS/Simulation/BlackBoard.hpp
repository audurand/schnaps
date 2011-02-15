/*
 * BlackBoard.hpp
 *
 *  Created on: 2010-03-31
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

#ifndef SCHNAPS_Simulation_BlackBoard_hpp
#define SCHNAPS_Simulation_BlackBoard_hpp

#include <list>
#include <map>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulation/Process.hpp"

namespace SCHNAPS {
namespace Simulation {

struct Push {
	std::string mProcess;
	Process::Target mTarget;
	unsigned long mTime;

	Push(const Push& inOriginal) :
		mProcess(inOriginal.mProcess.c_str()),
		mTarget(inOriginal.mTarget),
		mTime(inOriginal.mTime)
	{}
	Push(std::string inProcess, Process::Target inTarget, unsigned long inTime) :
		mProcess(inProcess.c_str()),
		mTarget(inTarget),
		mTime(inTime)
	{}
};

class BlackBoard: public SCHNAPS::Core::Object, public std::map<unsigned int, std::list<Push> > {
public:
	//! BlackBoard allocator type.
	typedef SCHNAPS::Core::AllocatorT<BlackBoard, SCHNAPS::Core::Object::Alloc> Alloc;
	//! BlackBoard handle type.
	typedef SCHNAPS::Core::PointerT<BlackBoard, SCHNAPS::Core::Object::Handle> Handle;
	//! BlackBoard bag type.
	typedef SCHNAPS::Core::ContainerT<BlackBoard, SCHNAPS::Core::Object::Bag> Bag;

	BlackBoard() {}
	BlackBoard(const BlackBoard& inOriginal);
	virtual ~BlackBoard() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("BlackBoard");
			return lName;
		schnaps_StackTraceEndM("const std::string& BlackBoard::getName() const");
	}
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_BlackBoard_hpp */
