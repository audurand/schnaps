/*
 * BasicPlugin.cpp
 *
 *  Created on: 2010-04-20
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
#include "SCHNAPS-plugins/Basic/Basic.hpp"

SCHNAPS_Plugin_BeginDefinitionM("BasicPlugin", "1.0");

// Math operators
SCHNAPS_Plugin_AddAllocM("Basic_Abs", basic::Abs::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Add", basic::Add::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Div", basic::Div::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Exp", basic::Exp::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Mod", basic::Mod::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Mult", basic::Mult::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Pow", basic::Pow::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Sub", basic::Sub::Alloc);

// Relational operators
SCHNAPS_Plugin_AddAllocM("Basic_IsBetween", basic::IsBetween::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IsEqual", basic::IsEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IsGreater", basic::IsGreater::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IsGreaterOrEqual", basic::IsGreaterOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IsLess", basic::IsLess::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IsLessOrEqual", basic::IsLessOrEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Not", basic::Not::Alloc);

// Logical operators
SCHNAPS_Plugin_AddAllocM("Basic_And", basic::And::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Or", basic::Or::Alloc);

// Branching operators
SCHNAPS_Plugin_AddAllocM("Basic_BranchDynamic", basic::BranchDynamic::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_BranchStatic", basic::BranchStatic::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_IfThenElse", basic::IfThenElse::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Parallel", basic::Parallel::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Sequential", basic::Sequential::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Switch", basic::Switch::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_SwitchBins", basic::SwitchBins::Alloc);

// Leafs
SCHNAPS_Plugin_AddAllocM("Basic_Nothing", basic::Nothing::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Token", basic::Token::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_ProcessCall", basic::ProcessCall::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Clock", basic::Clock::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Parameter", basic::Parameter::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_ProcessPush", basic::ProcessPush::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_RandomUniform", basic::RandomUniform::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Variable", basic::Variable::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_VariableEnvironment", basic::VariableEnvironment::Alloc);

// Other
SCHNAPS_Plugin_AddAllocM("Basic_Concatenate", basic::Concatenate::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Log", basic::Log::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_NPV", basic::NPV::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_RouletteDynamic", basic::RouletteDynamic::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_Serialize", basic::Serialize::Alloc);
SCHNAPS_Plugin_AddAllocM("Basic_SetVariable", basic::SetVariable::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
