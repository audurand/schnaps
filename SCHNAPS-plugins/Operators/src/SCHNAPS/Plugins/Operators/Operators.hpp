/*
 * Operators.hpp
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

#ifndef Operators_hpp
#define Operators_hpp

// Math operators
#include "SCHNAPS/Plugins/Operators/Abs.hpp"
#include "SCHNAPS/Plugins/Operators/Add.hpp"
#include "SCHNAPS/Plugins/Operators/AddVariable.hpp"
#include "SCHNAPS/Plugins/Operators/Div.hpp"
#include "SCHNAPS/Plugins/Operators/Exp.hpp"
#include "SCHNAPS/Plugins/Operators/Mod.hpp"
#include "SCHNAPS/Plugins/Operators/Mult.hpp"
#include "SCHNAPS/Plugins/Operators/MultVariable.hpp"
#include "SCHNAPS/Plugins/Operators/Pow.hpp"
#include "SCHNAPS/Plugins/Operators/Sub.hpp"
#include "SCHNAPS/Plugins/Operators/SubVariable.hpp"

// Relational operators
#include "SCHNAPS/Plugins/Operators/IsBetween.hpp"
#include "SCHNAPS/Plugins/Operators/IsBetweenVariable.hpp"
#include "SCHNAPS/Plugins/Operators/IsEqual.hpp"
#include "SCHNAPS/Plugins/Operators/IsEqualVariable.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreater.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreaterVariable.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreaterOrEqual.hpp"
#include "SCHNAPS/Plugins/Operators/IsLess.hpp"
#include "SCHNAPS/Plugins/Operators/IsLessVariable.hpp"
#include "SCHNAPS/Plugins/Operators/IsLessOrEqual.hpp"
#include "SCHNAPS/Plugins/Operators/Not.hpp"

// Logical operators
#include "SCHNAPS/Plugins/Operators/And.hpp"
#include "SCHNAPS/Plugins/Operators/Or.hpp"

// Other
#include "SCHNAPS/Plugins/Operators/Concatenate.hpp"
#include "SCHNAPS/Plugins/Operators/ConcatVariable.hpp"
#include "SCHNAPS/Plugins/Operators/NPV.hpp"
#include "SCHNAPS/Plugins/Operators/Serialize.hpp"

#endif /* Operators_hpp */
