/*
 * Learning.cpp
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
#include "SCHNAPS/Plugins/Learning/Learning.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Learning", "1.1.0");

SCHNAPS_Plugin_AddAllocM("Learning_DecisionNode", SCHNAPS::Plugins::Learning::DecisionNode::Alloc);
SCHNAPS_Plugin_AddAllocM("Learning_LearningModule", SCHNAPS::Plugins::Learning::LearningModule::Alloc);

SCHNAPS_Plugin_EndDefinitionM();
