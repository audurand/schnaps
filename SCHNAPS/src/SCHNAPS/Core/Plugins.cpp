/*
 * Plugins.cpp
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
 * \brief Construct object plugins component.
 */
Plugins::Plugins() :
	Component("Plugins")
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Plugins::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
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

		// add allocators to the factory
		lAllocatorsList.clear();
		lPlugin->listFactories(lAllocatorsList);
		for (unsigned int i = 0; i < lAllocatorsList.size(); ++i) {
			lFactory.insertAllocator(lAllocatorsList[i], lPlugin->getAllocator(lAllocatorsList[i]));
		}

		insertPlugin(lPlugin->getLibName(), lPlugin);
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Plugins::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
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
 * \brief Insert new plugin.
 * \param inLabel A const reference to the label of plugin.
 * \param inPlugin A handle to the plugin.
 * \throw SCHNAPS::Core::RunTimeException if the plugin already exists.
 */
void Plugins::insertPlugin(const std::string& inLabel, Plugin::Handle inPlugin) {
	schnaps_StackTraceBeginM();
	Plugins::PluginMap::const_iterator lIterPluginMap = mPluginMap.find(inLabel);
	if(lIterPluginMap != mPluginMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The plugin '" << inLabel;
		lOSS << "' is already present in the map of plugins; ";
		lOSS << "could not add it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mPluginMap[inLabel] = inPlugin;
	schnaps_StackTraceEndM("void SCHNAPS::Core::Plugins::insertPlugin(const std::string&, SCHNAPS::Core::Plugin::Handle)");
}
