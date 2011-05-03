/*
 * Operators.hpp
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

#ifndef Operators_hpp
#define Operators_hpp

// mathematical operators
#include "SCHNAPS/Plugins/Operators/AbsComplex.hpp"
#include "SCHNAPS/Plugins/Operators/AddComplex.hpp"
#include "SCHNAPS/Plugins/Operators/AddVariable.hpp"
#include "SCHNAPS/Plugins/Operators/DivComplex.hpp"
#include "SCHNAPS/Plugins/Operators/DivVariable.hpp"
#include "SCHNAPS/Plugins/Operators/ExpComplex.hpp"
#include "SCHNAPS/Plugins/Operators/ModComplex.hpp"
#include "SCHNAPS/Plugins/Operators/MultComplex.hpp"
#include "SCHNAPS/Plugins/Operators/MultVariable.hpp"
#include "SCHNAPS/Plugins/Operators/PowComplex.hpp"
#include "SCHNAPS/Plugins/Operators/SubComplex.hpp"
#include "SCHNAPS/Plugins/Operators/SubVariable.hpp"

// relational operators (comparison)
#include "SCHNAPS/Plugins/Operators/IsBetween.hpp"
#include "SCHNAPS/Plugins/Operators/IsBetweenComplex.hpp"
#include "SCHNAPS/Plugins/Operators/IsEqual.hpp"
#include "SCHNAPS/Plugins/Operators/IsEqualComplex.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreater.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreaterComplex.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreaterOrEqual.hpp"
#include "SCHNAPS/Plugins/Operators/IsGreaterOrEqualComplex.hpp"
#include "SCHNAPS/Plugins/Operators/IsLess.hpp"
#include "SCHNAPS/Plugins/Operators/IsLessComplex.hpp"
#include "SCHNAPS/Plugins/Operators/IsLessOrEqual.hpp"
#include "SCHNAPS/Plugins/Operators/IsLessOrEqualComplex.hpp"

// logical operators
#include "SCHNAPS/Plugins/Operators/And.hpp"
#include "SCHNAPS/Plugins/Operators/AndComplex.hpp"
#include "SCHNAPS/Plugins/Operators/Not.hpp"
#include "SCHNAPS/Plugins/Operators/NotComplex.hpp"
#include "SCHNAPS/Plugins/Operators/Or.hpp"
#include "SCHNAPS/Plugins/Operators/OrComplex.hpp"

// other
#include "SCHNAPS/Plugins/Operators/ConcatComplex.hpp"
#include "SCHNAPS/Plugins/Operators/ConcatVariable.hpp"
#include "SCHNAPS/Plugins/Operators/NPVComplex.hpp"
#include "SCHNAPS/Plugins/Operators/Serialize.hpp"

#endif /* Operators_hpp */
