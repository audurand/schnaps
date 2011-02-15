/*
 * Plugins.cpp
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

#include "SCHNAPS/Core.hpp"

#include <stdexcept>

using namespace SCHNAPS;
using namespace Core;

/*!
 *  \brief Construct object plugins component.
 */
Plugins::Plugins() :
		Component("Plugins")
{ }

/*!
 *  \brief Read the system component.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 */
void Plugins::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	schnaps_StackTraceBeginM();
		if(inIter->getType() != PACC::XML::eData)
			throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");

		if(inIter->getValue() != getName()) {
			std::ostringstream lOSS;
			lOSS << "tag <" << getName() << "> expected, but ";
			lOSS << "got tag <" << inIter->getValue() << "> instead!";
			throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
		}

		Plugin::Handle lPlugin;
		SCHNAPS::Core::Factory& lFactory = ioSystem.getFactory();
		std::vector<std::string> lAllocatorsList;
		for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
			lPlugin = new Plugin();
			lPlugin->read(lChild);

			// Add allocators to the factory
			lAllocatorsList.clear();
			lPlugin->listFactories(lAllocatorsList);
			for (unsigned int i = 0; i < lAllocatorsList.size(); ++i) {
				lFactory.insertAllocator(lAllocatorsList[i], lPlugin->getAllocator(lAllocatorsList[i]));
			}

			insertPlugin(lPlugin->getLibName(), lPlugin);
		}
	schnaps_StackTraceEndM("void Plugins::readWithSystem(PACC::XML::ConstIterator, System&)");
}

/*!
 *  \brief Write plugins component to stream.
 *  \param ioStreamer Output streamer.
 *  \param inIndent Whether markup should be indented.
 */
void Plugins::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		for (PluginMap::const_iterator lIt = mPluginMap.begin(); lIt != mPluginMap.end(); lIt++) {
			ioStreamer.openTag("Plugin");
			ioStreamer.insertAttribute("source", lIt->second->getSource());
			ioStreamer.closeTag();
		}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Plugins::writeContent(PACC::XML::Streamer&, bool) const");
}

/*!
 *  \brief Insert new library plugin in object plugins.
 *  \param inLibName Name of library inserted.
 *  \param inPlugin Plugin associated to library.
 *  \throw SCHNAPS::Core::RunTimeException If the library name is already in the plugins.
 */
void Plugins::insertPlugin(const std::string& inLibName, Plugin::Handle inPlugin) {
	schnaps_StackTraceBeginM();
		Plugins::PluginMap::const_iterator lIterPluginMap = mPluginMap.find(inLibName);
		if(lIterPluginMap != mPluginMap.end()) {
			std::ostringstream lOSS;
			lOSS << "The type name '" << inLibName;
			lOSS << "' is already present in the factory's allocator map; ";
			lOSS << "could not add it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		mPluginMap[inLibName] = inPlugin;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Plugins::insertPlugin(const std::string&, SCHNAPS::Core::Plugin::Handle)");
}
