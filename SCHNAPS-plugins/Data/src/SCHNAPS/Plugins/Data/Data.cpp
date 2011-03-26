/*
 * Data.cpp
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
#include "SCHNAPS/Plugins/Data/Data.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Data", "1.0");

SCHNAPS_Plugin_AddAllocM("Data_Clock", SCHNAPS::Plugins::Data::Clock::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_Log", SCHNAPS::Plugins::Data::Log::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_RandomUniform", SCHNAPS::Plugins::Data::RandomUniform::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_RouletteDynamic", SCHNAPS::Plugins::Data::RouletteDynamic::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_SetVariable", SCHNAPS::Plugins::Data::SetVariable::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_SetVariableComplex", SCHNAPS::Plugins::Data::SetVariableComplex::Alloc);
SCHNAPS_Plugin_AddAllocM("Data_Value", SCHNAPS::Plugins::Data::Value::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
