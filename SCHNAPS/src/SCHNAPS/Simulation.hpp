/*
 * Simulation.hpp
 *
 *  Created on: 2009-03-12
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

#ifndef Simulation_hpp
#define Simulation_hpp

// framework
#include "SCHNAPS/Simulation/State.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/Individual.hpp"
#include "SCHNAPS/Simulation/Population.hpp"
#include "SCHNAPS/Simulation/ExecutionContext.hpp"
#include "SCHNAPS/Simulation/PopulationManager.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Process.hpp"
#include "SCHNAPS/Simulation/ProcessPushed.hpp"
#include "SCHNAPS/Simulation/BlackBoard.hpp"
#include "SCHNAPS/Simulation/WaitingQMaps.hpp"
#include "SCHNAPS/Simulation/SimulationContext.hpp"

// population generation
#include "SCHNAPS/Simulation/Demography.hpp"
#include "SCHNAPS/Simulation/SimulationVariables.hpp"
#include "SCHNAPS/Simulation/IndividualModel.hpp"
#include "SCHNAPS/Simulation/GenProfile.hpp"
#include "SCHNAPS/Simulation/Generator.hpp"
#include "SCHNAPS/Simulation/GenerationContext.hpp"

// multi-Threading
#include "SCHNAPS/Simulation/SimulationThread.hpp"
#include "SCHNAPS/Simulation/GenerationThread.hpp"

#include "SCHNAPS/Simulation/Simulator.hpp"

#endif /* Simulation_hpp */
