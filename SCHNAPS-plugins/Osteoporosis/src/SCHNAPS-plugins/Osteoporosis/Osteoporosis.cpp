/*
 * OsteoporosisPlugin.cpp
 *
 *  Created on: 2010-11-02
 *  Updated on: 2010-11-02
 *      Author: Audrey Durand
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
#include "SCHNAPS-plugins/Osteoporosis/Osteoporosis.hpp"

SCHNAPS_Plugin_BeginDefinitionM("OsteoporosisPlugin", "0.1");

SCHNAPS_Plugin_AddAllocM("Osteo_Event", osteoporosis::Event::Alloc);
SCHNAPS_Plugin_AddAllocM("Osteo_PreventionCampain", osteoporosis::PreventionCampain::Alloc);
SCHNAPS_Plugin_AddAllocM("Osteo_Test", osteoporosis::Test::Alloc);
SCHNAPS_Plugin_AddAllocM("Osteo_Treatment", osteoporosis::Treatment::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
