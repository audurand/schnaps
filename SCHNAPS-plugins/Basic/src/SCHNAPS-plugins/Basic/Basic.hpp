/*
 * BasicPlugin.hpp
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

#ifndef Basic_hpp
#define Basic_hpp

// Math operators
#include "SCHNAPS-plugins/Basic/Abs.hpp"
#include "SCHNAPS-plugins/Basic/Add.hpp"
#include "SCHNAPS-plugins/Basic/Div.hpp"
#include "SCHNAPS-plugins/Basic/Exp.hpp"
#include "SCHNAPS-plugins/Basic/Mod.hpp"
#include "SCHNAPS-plugins/Basic/Mult.hpp"
#include "SCHNAPS-plugins/Basic/Pow.hpp"
#include "SCHNAPS-plugins/Basic/Sub.hpp"

// Relational operators
#include "SCHNAPS-plugins/Basic/IsBetween.hpp"
#include "SCHNAPS-plugins/Basic/IsEqual.hpp"
#include "SCHNAPS-plugins/Basic/IsGreater.hpp"
#include "SCHNAPS-plugins/Basic/IsGreaterOrEqual.hpp"
#include "SCHNAPS-plugins/Basic/IsLess.hpp"
#include "SCHNAPS-plugins/Basic/IsLessOrEqual.hpp"
#include "SCHNAPS-plugins/Basic/Not.hpp"

// Logical operators
#include "SCHNAPS-plugins/Basic/And.hpp"
#include "SCHNAPS-plugins/Basic/Or.hpp"

// Branching operators
#include "SCHNAPS-plugins/Basic/BranchDynamic.hpp"
#include "SCHNAPS-plugins/Basic/BranchStatic.hpp"
#include "SCHNAPS-plugins/Basic/IfThenElse.hpp"
#include "SCHNAPS-plugins/Basic/Parallel.hpp"
#include "SCHNAPS-plugins/Basic/Sequential.hpp"
#include "SCHNAPS-plugins/Basic/Switch.hpp"
#include "SCHNAPS-plugins/Basic/SwitchBins.hpp"

// Leafs
#include "SCHNAPS-plugins/Basic/Nothing.hpp"
#include "SCHNAPS-plugins/Basic/Token.hpp"
#include "SCHNAPS-plugins/Basic/ProcessCall.hpp"
#include "SCHNAPS-plugins/Basic/Clock.hpp"
#include "SCHNAPS-plugins/Basic/Parameter.hpp"
#include "SCHNAPS-plugins/Basic/ProcessPush.hpp"
#include "SCHNAPS-plugins/Basic/RandomUniform.hpp"
#include "SCHNAPS-plugins/Basic/Variable.hpp"
#include "SCHNAPS-plugins/Basic/VariableEnvironment.hpp"

// Other
#include "SCHNAPS-plugins/Basic/Concatenate.hpp"
#include "SCHNAPS-plugins/Basic/Log.hpp"
#include "SCHNAPS-plugins/Basic/NPV.hpp"
#include "SCHNAPS-plugins/Basic/RouletteDynamic.hpp"
#include "SCHNAPS-plugins/Basic/Serialize.hpp"
#include "SCHNAPS-plugins/Basic/SetVariable.hpp"

#endif /* Basic_hpp */
