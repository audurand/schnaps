/*
 * Meds.cpp
 *
 *  Created on: 2011-02-15
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
#include "SCHNAPS/Plugins/Meds/Meds.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Meds", "0.1");

SCHNAPS_Plugin_AddAllocM("Meds_Event", SCHNAPS::Plugins::Meds::Event::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Eventv2", SCHNAPS::Plugins::Meds::Eventv2::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_PreventionCampain", SCHNAPS::Plugins::Meds::PreventionCampain::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Test", SCHNAPS::Plugins::Meds::Test::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Testv2", SCHNAPS::Plugins::Meds::Testv2::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Treatment", SCHNAPS::Plugins::Meds::Treatment::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Treatmentv2", SCHNAPS::Plugins::Meds::Treatmentv2::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
