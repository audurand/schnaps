/*
 * ContactsGen.cpp
 *
 * SCHNAPS
 * Copyright (C) 2012 by Xavier Douville
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

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief  Generate contacts.
 * \param  inPop Handle to the individuals to generate contacts for
 * \param  inSystem Handle to the system
 * \param  ioList List of contacts
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined is a subclass.
 */
void ContactsGen::generate(Simulation::Individual::Bag::Handle inPop, Core::System::Handle inSystem, std::vector<Core::Vector::Handle>& ioList) const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("generate", "ContactsGen", getName());
	schnaps_StackTraceEndM("void ContactsGen::generate(Simulation::Individual::Bag::Handle inPop, Core::System::Handle inSystem, std::vector<Core::Vector::Handle>& ioList) const");
}


