/*
 * Plugin.cpp
 *
 *  Created on: 2009-11-21
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


/*!
 *  \brief Class methods for the portable dynamically loadable libraries.
 *  \author Christian Gagne <cgagne@gmail.com>
 *  \author Alexandre Devert <devert@lri.fr>
 */

#include "SCHNAPS/Core.hpp"

#include <sstream>
#include <stdexcept>

using namespace core;

void Plugin::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
		if (inIter->getType() != PACC::XML::eData) {
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
		}
		if (inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		// Load plugin
		std::string lSource = inIter->getAttribute("source");
		if (lSource.empty()) {
			throw schnaps_IOExceptionNodeM(*inIter, "source attribute expected!");
		}
		load(lSource);
	schnaps_StackTraceEndM("void core::Plugin::read(PACC::XML::ConstIterator)");
}

void Plugin::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
		read(inIter);
	schnaps_StackTraceEndM("void core::Plugin::readWithSystem(PACC::XML::ConstIterator, System&)");
}

/*!
 *  \brief Write a plugin into a XML streamer.
 *  \param ioStreamer XML streamer into which the string is written.
 *  \param inIndent Whether output should be indented.
 */
void Plugin::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.openTag(getName(), inIndent);
		writeContent(ioStreamer, inIndent);
		ioStreamer.closeTag();
	schnaps_StackTraceEndM("void core::Plugin::write(PACC::XML::Streamer&, bool) const");
}

/*!
 *  \brief Write plugin content into XML streamer, without opening/closing tags.
 *  \param ioStreamer XML streamer to write the tree into.
 *  \param inIndent Whether XML output should be indented.
 */
void Plugin::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		ioStreamer.insertAttribute("source", mSource);
	schnaps_StackTraceEndM("void core::Plugin::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 *  \brief Bind to a function of a dynamically loaded library.
 *  \param inDynLib Handle to the loaded dynamic library.
 *  \param inFunctionName Name of the function to bind on.
 *  \return Opaque pointer to the desired function.
 *  \throw std::runtime_error If it is impossible to bind on the function.
 */
void* Plugin::bindDynLibFunction(void* inDynLib, std::string inFunctionName) {
	if (inDynLib == NULL) {
		std::ostringstream lOSS;
		lOSS << "Impossible to bind the \"";
		lOSS << inFunctionName << "\" function on the given dynamic library, ";
		lOSS << "the given handler pointer is NULL!";
		throw std::runtime_error(lOSS.str());
	}
#ifdef SCHNAPS_IS_WINDOWS
	void* lFunction = GetProcAddress((HMODULE)inDynLib, TEXT(inFunctionName.c_str()));
	if(lFunction == NULL) {
		std::ostringstream lOSS;
		lOSS << "It seems that it is impossible to bind function \"" << inFunctionName;
		lOSS << "\" from the plugin. ";
		DWORD lErrorCode = GetLastError();
		LPVOID lMsgBuf;
		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				lErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lMsgBuf,
				0,
				NULL
		);
		lOSS << "Windows error code " << lErrorCode << ": " << (char*)lMsgBuf;
		LocalFree(lMsgBuf);
		throw std::runtime_error(lOSS.str());
	}
	return lFunction;
#else // SCHNAPS_IS_WINDOWS
	dlerror();
	void* lFunction = dlsym(inDynLib, inFunctionName.c_str());
	char* lErrorMessage = dlerror();
	if (lErrorMessage != NULL) {
		std::ostringstream lOSS;
		lOSS << "An error occured while trying to bind to the \"";
		lOSS << inFunctionName << "\" symbol on the given dynamic library. ";
		lOSS << "Detailed error message is: \"" << lErrorMessage << "\".";
		throw std::runtime_error(lOSS.str());
	}
	return lFunction;
#endif // SCHNAPS_IS_WINDOWS
}

/*!
 *  \brief Load a dynamic library into memory.
 *  \param inFilename Filename of the library to load in memory.
 *  \return Opaque pointer loaded dynamic library.
 *  \throw std::runtime_error If it is impossible to load the library.
 *
 *  The filename used is pretty fragile depending on the platform.
 *
 *  On Unices systems, if a simple filename without slash is provided, the command will
 *  search for files in usual dynamic libraries directories. To load a library in current
 *  directory, the filename must be prefixed by "./" in order to specified to look into the
 *  current directory. For more details, see the ldopen manual ("man dlopen").
 *
 *  On Windows, user must be cautious to use backslashes instead of slashes to separate
 *  directories and binaries in the filename paths.
 */
void* Plugin::loadDynLib(std::string inSource) {
#ifdef SCHNAPS_IS_WINDOWS
	HINSTANCE lHandle = LoadLibrary(inSource.c_str());
	if(lHandle == NULL) {
		std::ostringstream lOSS;
		lOSS << "It seems that it is impossible to load filename \"" << inSource;
		lOSS << "\" as a plugin. ";
		DWORD lErrorCode = GetLastError();
		LPVOID lMsgBuf;
		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				lErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lMsgBuf,
				0,
				NULL
		);
		lOSS << "Windows error code " << lErrorCode << ": " << (char*)lMsgBuf;
		LocalFree(lMsgBuf);
		throw std::runtime_error(lOSS.str());
	}
	return lHandle;
#else // SCHNAPS_IS_WINDOWS
	void* lDynLib = dlopen(inSource.c_str(), RTLD_LAZY);
	if (lDynLib == NULL) {
		std::ostringstream lOSS;
		lOSS << "It seems that it is impossible to load filename \"" << inSource;
		lOSS << "\" as a plugin.";
		char* lErrorMessage = dlerror();
		if (lErrorMessage != NULL) {
			lOSS << " Detailed error message is: \"" << lErrorMessage << "\".";
		}
		throw std::runtime_error(lOSS.str());
	}
	return lDynLib;
#endif // SCHNAPS_IS_WINDOWS
}

/*!
 *  \brief Construct a plain plugin object without loading a library.
 */
Plugin::Plugin() :
	mDynLib(NULL)
{}

/*!
 * \brief Lists all allocators contained in plugin.
 * \param outAllocators Output for result.
 */
void Plugin::listFactories(std::vector<std::string>& outAllocators) const {
	if (mDynLib == NULL) {
		std::ostringstream lOSS;
		lOSS << "Could not list allocators, it seems that no library is loaded by the plugin!";
		throw std::runtime_error(lOSS.str());
	}
	for (AllocatorMap::const_iterator lIter = mAllocatorMap.begin(); lIter != mAllocatorMap.end(); lIter++) {
		outAllocators.push_back(lIter->first);
	}
}

/*!
 * \brief Get allocator of specified name.
 * \param inAllocName The name of allocator to retrieve.
 * \return Handle to allocator.
 * \throw std::runtime_error If no library loaded, no allocator found or NULL allocator.
 */
Allocator::Handle Plugin::getAllocator(std::string inAllocName) const {
	if (mDynLib == NULL) {
		std::ostringstream lOSS;
		lOSS << "Could not allocate an object, it seems that no library is loaded by the plugin!";
		throw std::runtime_error(lOSS.str());
	}
	AllocatorMap::const_iterator lIter = mAllocatorMap.find(inAllocName);
	if (lIter == mAllocatorMap.end()) {
		std::ostringstream lOSS;
		lOSS << "No object allocator named \"" << inAllocName << "\" found in current plugin \"";
		lOSS << mLibName << "\" (version: \"" << mVersion << "\")!";
		throw std::runtime_error(lOSS.str());
	}
	if (lIter->second == NULL) {
		std::ostringstream lOSS;
		lOSS << "NULL pointer found for allocator named \"" << inAllocName;
		lOSS << "\" in current plugin \"" << mLibName << "\" (version: \"";
		lOSS << mVersion << "\")!";
		throw std::runtime_error(lOSS.str());
	}
	return lIter->second;
}

/*!
 *  \brief Load the dynamic library given by the filename in memory.
 *  \param inFilename Name of the dynamic library to load in memory.
 *  \throw std::runtime_error If it is impossible to load the library.
 *
 *  The filename used is pretty fragile depending on the platform.
 *
 *  On Unices, if a simple filename without slash is provided, the command will
 *  search for files in usual dynamic libraries directories. To load a library in current
 *  directory, the filename must be prefixed by "./" in order to specified to look into the
 *  current directory. For more details, see the ldopen manual ("man dlopen").
 *
 *  On Windows, user must be cautious to use backslashes instead of slashes to separate
 *  directories and binaries in the filename paths.
 */
void Plugin::load(std::string inSource) {
	typedef void (*PluginSpecFctPtr)(std::string&, std::string&, AllocatorMap&);
	if (mDynLib != NULL) {
		std::ostringstream lOSS;
		lOSS << "Could not load the library, it seems that the plugin as already a library loaded!";
		throw std::runtime_error(lOSS.str());
	}
	mDynLib = loadDynLib(inSource);
	mLibName.clear();
	mVersion.clear();
	mAllocatorMap.clear();
	PluginSpecFctPtr lFunctPtr = (PluginSpecFctPtr) bindDynLibFunction(mDynLib, "__SCHNAPS_Plugin_getPluginSpecs");
	lFunctPtr(mLibName, mVersion, mAllocatorMap);
	mSource = inSource;
}
