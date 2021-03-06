/*
 * SimulationVariables.cpp
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

#include "SCHNAPS/Core.hpp"
#include "SCHNAPS/Simulation.hpp"

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Construct a class for simulation variables as a copy an original.
 * \param inOriginal A const reference to an original class of simulation variables.
 */
SimulationVariables::SimulationVariables(const SimulationVariables& inOriginal) {
	this->mVariables.clear();
	for (unsigned int i = 0; i < inOriginal.mVariables.size(); i++) {
		this->mVariables.push_back(Variable(
			inOriginal.mVariables[i].mLabel,
			inOriginal.mVariables[i].mInitTree));
		for (unsigned int j = 0; j < inOriginal.mVariables[i].mLocalVariables.size(); j++) {
			this->mVariables.back().mLocalVariables.push_back(LocalVariable(
				inOriginal.mVariables[i].mLocalVariables[j].first,
				inOriginal.mVariables[i].mLocalVariables[j].second));
		}
	}
}

/*!
 * \brief  Return a handle to a deep copy of the object.
 * \param  inSystem A const reference to the system.
 * \return A handle to a deep copy of the object.
 */
Core::Object::Handle SimulationVariables::deepCopy(const Core::System& inSystem) const {
	schnaps_StackTraceBeginM();
	SimulationVariables::Handle lCopy = new SimulationVariables();
	
	for (unsigned int i = 0; i < this->mVariables.size(); i++) {
		lCopy->mVariables.push_back(Variable(
			this->mVariables[i].mLabel,
			Core::castHandleT<Core::PrimitiveTree>(this->mVariables[i].mInitTree->deepCopy(inSystem))));
		for (unsigned int j = 0; j < this->mVariables[i].mLocalVariables.size(); j++) {
			lCopy->mVariables.back().mLocalVariables.push_back(LocalVariable(
				this->mVariables[i].mLocalVariables[j].first,
				Core::castHandleT<Core::AnyType>(this->mVariables[i].mLocalVariables[j].second->clone())));
		}
	}
	return lCopy;
	schnaps_StackTraceEndM("SCHNAPS::Core::Object::Handle SCHNAPS::Simulation::SimulationVariables::deepCopy(const SCHNAPS::Core::System&) const");
}
