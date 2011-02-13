/*
 * EasierPlugin.cpp
 *
 *  Created on: 2010-11-20
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

#include "SCHNAPS/SCHNAPS.hpp"
#include "SCHNAPS-plugins/Easier/Easier.hpp"

SCHNAPS_Plugin_BeginDefinitionM("EasierPlugin", "0.1");

// Branching operators
SCHNAPS_Plugin_AddAllocM("Easier_Branch", easier::Branch::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_ChoiceIsBetween", easier::ChoiceIsBetween::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_ChoiceIsEqual", easier::ChoiceIsEqual::Alloc);

// Other
SCHNAPS_Plugin_AddAllocM("Easier_AddVariable", easier::AddVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_ConcatVariable", easier::ConcatVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_IsBetweenVariable", easier::IsBetweenVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_IsEqualVariable", easier::IsEqualVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_IsLessVariable", easier::IsLessVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_IsGreaterVariable", easier::IsGreaterVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_MultVariable", easier::MultVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_SetVariable", easier::SetVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Easier_SubVariable", easier::SubVariable::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
