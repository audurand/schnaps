/*
 * Operators.cpp
 *
 *  Created on: 2011-02-14
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
#include "SCHNAPS/Plugins/Operators/Operators.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Operators", "1.0");

// math operators
SCHNAPS_Plugin_AddAllocM("Operators_Abs", SCHNAPS::Plugins::Operators::Abs::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Add", SCHNAPS::Plugins::Operators::Add::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_AddVariable", SCHNAPS::Plugins::Operators::AddVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Div", SCHNAPS::Plugins::Operators::Div::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Exp", SCHNAPS::Plugins::Operators::Exp::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Mod", SCHNAPS::Plugins::Operators::Mod::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Mult", SCHNAPS::Plugins::Operators::Mult::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_MultVariable", SCHNAPS::Plugins::Operators::MultVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Pow", SCHNAPS::Plugins::Operators::Pow::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Sub", SCHNAPS::Plugins::Operators::Sub::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_SubVariable", SCHNAPS::Plugins::Operators::SubVariable::Alloc);

// relational operators
SCHNAPS_Plugin_AddAllocM("Operators_IsBetween", SCHNAPS::Plugins::Operators::IsBetween::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsBetweenVariable", SCHNAPS::Plugins::Operators::IsBetweenVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsEqual", SCHNAPS::Plugins::Operators::IsEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsEqualVariable", SCHNAPS::Plugins::Operators::IsEqualVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreater", SCHNAPS::Plugins::Operators::IsGreater::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreaterVariable", SCHNAPS::Plugins::Operators::IsGreaterVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreaterOrEqual", SCHNAPS::Plugins::Operators::IsGreaterOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLess", SCHNAPS::Plugins::Operators::IsLess::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLessVariable", SCHNAPS::Plugins::Operators::IsLessVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLessOrEqual", SCHNAPS::Plugins::Operators::IsLessOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Not", SCHNAPS::Plugins::Operators::Not::Alloc);

// logical operators
SCHNAPS_Plugin_AddAllocM("Operators_And", SCHNAPS::Plugins::Operators::And::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Or", SCHNAPS::Plugins::Operators::Or::Alloc);

// others
SCHNAPS_Plugin_AddAllocM("Operators_Concatenate", SCHNAPS::Plugins::Operators::Concatenate::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_ConcatVariable", SCHNAPS::Plugins::Operators::ConcatVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_NPV", SCHNAPS::Plugins::Operators::NPV::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Serialize", SCHNAPS::Plugins::Operators::Serialize::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
