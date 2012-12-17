/*
 * Factory.cpp
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
 
#include "SCHNAPS/Core.hpp"

#include <stdexcept>

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Default constructor.
 */
Factory::Factory() :
	Component("Factory")
{
	// any types
	insertAllocator("Vector", new Vector::Alloc());

	// atoms
	insertAllocator("Bool", new Bool::Alloc());
	insertAllocator("Char", new Char::Alloc());
	insertAllocator("String", new String::Alloc());

	// numbers
	insertAllocator("Double", new Double::Alloc());
	insertAllocator("Int", new Int::Alloc());
	insertAllocator("Long", new Long::Alloc());
	insertAllocator("UInt", new UInt::Alloc());
	insertAllocator("ULong", new ULong::Alloc());

	// primitives
	insertAllocator("PrimitiveTree", new PrimitiveTree::Alloc());
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Factory::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	// TODO: write current state?
	schnaps_StackTraceEndM("void SCHNAPS::Core::Factory::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 * \brief Alias a type name to an already existing one in object factory.
 * \param inTypeName Original name of type.
 * \param inAlias Alias name of that type.
 * \throw SCHNAPS::Core::RunTimeException if the original type name is not in the factory
 *    or if the alias is already in the factory.
 */
void Factory::aliasAllocator(const std::string& inTypeName, const std::string& inAlias) {
	schnaps_StackTraceBeginM();
	Factory::AllocatorMap::const_iterator lIterType = mAllocatorMap.find(inTypeName);
	if(lIterType == mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inTypeName;
		lOSS << "' is not present in the factory's allocator map; ";
		lOSS << "could not alias it";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	Factory::AllocatorMap::const_iterator lIterAlias = mAllocatorMap.find(inAlias);
	if(lIterAlias != mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inAlias;
		lOSS << "' is already present in the factory's allocator map; ";
		lOSS << "could not make an alias using its name";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mAllocatorMap[inAlias] = lIterType->second;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Factory::aliasAllocator(const std::string&, const std::string&)");
}

/*!
 * \brief Insert new allocator in object factory.
 * \param inTypeName Name of type inserted.
 * \param inAllocator Allocator associated to type.
 * \throw SCHNAPS::Core::RunTimeException if the type name is already in the factory.
 */
void Factory::insertAllocator(const std::string& inTypeName, Allocator::Handle inAllocator) {
	schnaps_StackTraceBeginM();
	Factory::AllocatorMap::const_iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
	if(lIterAllocMap != mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The type name '" << inTypeName;
		lOSS << "' is already present in the factory's allocator map; ";
		lOSS << "could not add it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mAllocatorMap[inTypeName] = inAllocator;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Factory::insertAllocator(const std::string&, SCHNAPS::Core::Allocator::Handle)");
}

/*!
 * \brief Remove allocator from the factory.
 * \param inTypeName Type name of the allocator to remove.
 * \return Allocator associated to the removed type name.
 * \throw SCHNAPS::Core::RunTimeException if the type name does not exist in the factory.
 */
Allocator::Handle Factory::removeAllocator(const std::string& inTypeName) {
	schnaps_StackTraceBeginM();
	Factory::AllocatorMap::iterator lIterAllocMap = mAllocatorMap.find(inTypeName);
	if(lIterAllocMap == mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The allocator of type '" << inTypeName;
		lOSS << "' does not exist; ";
		lOSS << "could not remove it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	
	Allocator::Handle lAlloc = lIterAllocMap->second;
	mAllocatorMap.erase(lIterAllocMap);
	return lAlloc;
	schnaps_StackTraceEndM("SCHNAPS::Core::Allocator::Handle SCHNAPS::Core::Factory::removeAllocator(const std::string&)");
}
