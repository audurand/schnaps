/*
 * Core.hpp
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

#ifndef Core_hpp
#define Core_hpp

#include "PACC/Math.hpp"
#include "PACC/Util.hpp"
#include "PACC/XML.hpp"

#include "SCHNAPS/config.hpp"
#include "SCHNAPS/gzstream.h"
#include "SCHNAPS/macros.hpp"

#include "Core/Object.hpp"
#include "Core/AnyType.hpp"
#include "Core/Vector.hpp"
#include "Core/Atom.hpp"
#include "Core/Number.hpp"
#include "Core/Pointer.hpp"
#include "Core/PointerT.hpp"
#include "Core/castObjectT.hpp"
#include "Core/Allocator.hpp"
#include "Core/AllocatorT.hpp"
#include "Core/AbstractAllocT.hpp"
#include "Core/Plugin.hpp"
#include "Core/Exception.hpp"
#include "Core/TargetedException.hpp"
#include "Core/ValidationException.hpp"
#include "Core/AssertException.hpp"
#include "Core/BadCastException.hpp"
#include "Core/InternalException.hpp"
#include "Core/ObjectException.hpp"
#include "Core/IOException.hpp"
#include "Core/RunTimeException.hpp"
#include "Core/IsEqualPointerPredicate.hpp"
#include "Core/IsLessPointerPredicate.hpp"
#include "Core/IsMorePointerPredicate.hpp"
#include "Core/IsEqualMapPairPredicate.hpp"
#include "Core/IsLessMapPairPredicate.hpp"
#include "Core/HashString.hpp"
#include "Core/Container.hpp"
#include "Core/ContainerT.hpp"
#include "Core/MapT.hpp"
#include "Core/ArrayT.hpp"
#include "Core/RouletteT.hpp"
#include "Core/Bool.hpp"
#include "Core/Char.hpp"
#include "Core/Int.hpp"
#include "Core/Long.hpp"
#include "Core/UInt.hpp"
#include "Core/ULong.hpp"
#include "Core/Double.hpp"
#include "Core/String.hpp"
#include "Core/ContactsGen.hpp"

// Generic framework
#include "Core/Component.hpp"
#include "Core/ExecutionContext.hpp"
#include "Core/Factory.hpp"
#include "Core/Logger.hpp"
#include "Core/LoggerMulti.hpp"
#include "Core/Parameters.hpp"
#include "Core/Plugins.hpp"
#include "Core/Randomizer.hpp"
#include "Core/RandomizerMulti.hpp"
#include "Core/System.hpp"
#include "Core/TypingManager.hpp"

// Primitives
#include "Core/Primitive.hpp"
#include "Core/PrimitiveTree.hpp"

#endif // Core_hpp
