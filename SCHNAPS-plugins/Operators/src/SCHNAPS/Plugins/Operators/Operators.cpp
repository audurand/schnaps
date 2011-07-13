/*
 * Operators.cpp
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

#include "SCHNAPS/SCHNAPS.hpp"
#include "SCHNAPS/Plugins/Operators/Operators.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Operators", "2.0.0");

// math operators
SCHNAPS_Plugin_AddAllocM("Operators_AbsComplex", SCHNAPS::Plugins::Operators::AbsComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_AddComplex", SCHNAPS::Plugins::Operators::AddComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_AddVariable", SCHNAPS::Plugins::Operators::AddVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_DivComplex", SCHNAPS::Plugins::Operators::DivComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_DivVariable", SCHNAPS::Plugins::Operators::DivVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_ExpComplex", SCHNAPS::Plugins::Operators::ExpComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_ModComplex", SCHNAPS::Plugins::Operators::ModComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_MultComplex", SCHNAPS::Plugins::Operators::MultComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_MultVariable", SCHNAPS::Plugins::Operators::MultVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_PowComplex", SCHNAPS::Plugins::Operators::PowComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_SubComplex", SCHNAPS::Plugins::Operators::SubComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_SubVariable", SCHNAPS::Plugins::Operators::SubVariable::Alloc);

// relational operators
SCHNAPS_Plugin_AddAllocM("Operators_IsBetween", SCHNAPS::Plugins::Operators::IsBetween::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsBetweenComplex", SCHNAPS::Plugins::Operators::IsBetweenComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsEqual", SCHNAPS::Plugins::Operators::IsEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsEqualComplex", SCHNAPS::Plugins::Operators::IsEqualComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreater", SCHNAPS::Plugins::Operators::IsGreater::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreaterComplex", SCHNAPS::Plugins::Operators::IsGreaterComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreaterOrEqual", SCHNAPS::Plugins::Operators::IsGreaterOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsGreaterOrEqualComplex", SCHNAPS::Plugins::Operators::IsGreaterOrEqualComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLess", SCHNAPS::Plugins::Operators::IsLess::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLessComplex", SCHNAPS::Plugins::Operators::IsLessComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLessOrEqual", SCHNAPS::Plugins::Operators::IsLessOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_IsLessOrEqualComplex", SCHNAPS::Plugins::Operators::IsLessOrEqualComplex::Alloc);

// logical operators
SCHNAPS_Plugin_AddAllocM("Operators_And", SCHNAPS::Plugins::Operators::And::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_AndComplex", SCHNAPS::Plugins::Operators::AndComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Not", SCHNAPS::Plugins::Operators::Not::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_NotComplex", SCHNAPS::Plugins::Operators::NotComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Or", SCHNAPS::Plugins::Operators::Or::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_OrComplex", SCHNAPS::Plugins::Operators::OrComplex::Alloc);

// others
SCHNAPS_Plugin_AddAllocM("Operators_ConcatComplex", SCHNAPS::Plugins::Operators::ConcatComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_ConcatVariable", SCHNAPS::Plugins::Operators::ConcatVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_NPVComplex", SCHNAPS::Plugins::Operators::NPVComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Operators_Serialize", SCHNAPS::Plugins::Operators::Serialize::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
