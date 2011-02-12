/*!
 *  \file   Core.hpp
 *  \brief  Header for including SCHNAPS object oriented foundations.
 *  \author Audrey Durand
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
#include "Core/Float.hpp"
#include "Core/Double.hpp"
#include "Core/String.hpp"

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
