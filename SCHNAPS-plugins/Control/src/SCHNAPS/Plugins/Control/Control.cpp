/*
 * Control.cpp
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
#include "SCHNAPS/Plugins/Control/Control.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Control", "1.0");

SCHNAPS_Plugin_AddAllocM("Control_Branch", SCHNAPS::Plugins::Control::Branch::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_BranchDynamic", SCHNAPS::Plugins::Control::BranchDynamic::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_BranchStatic", SCHNAPS::Plugins::Control::BranchStatic::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_ChoiceIsBetween", SCHNAPS::Plugins::Control::ChoiceIsBetween::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_ChoiceIsEqual", SCHNAPS::Plugins::Control::ChoiceIsEqual::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_IfThenElse", SCHNAPS::Plugins::Control::IfThenElse::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_Nothing", SCHNAPS::Plugins::Control::Nothing::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_Parallel", SCHNAPS::Plugins::Control::Parallel::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_ProcessCall", SCHNAPS::Plugins::Control::ProcessCall::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_ProcessPush", SCHNAPS::Plugins::Control::ProcessPush::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_Sequential", SCHNAPS::Plugins::Control::Sequential::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_Switch", SCHNAPS::Plugins::Control::Switch::Alloc);
SCHNAPS_Plugin_AddAllocM("Control_SwitchBins", SCHNAPS::Plugins::Control::SwitchBins::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
