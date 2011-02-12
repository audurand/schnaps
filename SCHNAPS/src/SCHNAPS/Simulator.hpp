/*
 * Simulator.hpp
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

#ifndef Simulator_hpp
#define Simulator_hpp

// framework
#include "SCHNAPS/Simulator/State.hpp"
#include "SCHNAPS/Simulator/Environment.hpp"
#include "SCHNAPS/Simulator/Individual.hpp"
#include "SCHNAPS/Simulator/Population.hpp"
#include "SCHNAPS/Simulator/ExecutionContext.hpp"
#include "SCHNAPS/Simulator/PopulationManager.hpp"
#include "SCHNAPS/Simulator/Clock.hpp"
#include "SCHNAPS/Simulator/Process.hpp"
#include "SCHNAPS/Simulator/ProcessPushed.hpp"
#include "SCHNAPS/Simulator/BlackBoard.hpp"
#include "SCHNAPS/Simulator/WaitingQMaps.hpp"
#include "SCHNAPS/Simulator/SimulationContext.hpp"

// population generation
#include "SCHNAPS/Simulator/Demography.hpp"
#include "SCHNAPS/Simulator/SimulationVariables.hpp"
#include "SCHNAPS/Simulator/IndividualModel.hpp"
#include "SCHNAPS/Simulator/GenProfile.hpp"
#include "SCHNAPS/Simulator/Generator.hpp"
#include "SCHNAPS/Simulator/GenerationContext.hpp"

// multi-Threading
#include "SCHNAPS/Simulator/SimulationThread.hpp"
#include "SCHNAPS/Simulator/GenerationThread.hpp"

#include "SCHNAPS/Simulator/Simulator.hpp"

#endif /* Simulator_hpp */
