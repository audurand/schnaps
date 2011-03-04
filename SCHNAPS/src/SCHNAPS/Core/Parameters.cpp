/*
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

using namespace SCHNAPS;
using namespace Core;

/*!
 *  \brief Construct object parameters component.
 */
Parameters::Parameters() :
	Component("Parameters")
{}

void Parameters::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	if ((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Parameters")) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag <Parameters> expected!");
	}

	AnyType::Handle lValue;
	Object::Alloc::Handle lAlloc;
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Entry") {
				throw schnaps_IOExceptionNodeM(*lChild, "tag <Entry> expected!");
			}
			if (lChild->getAttribute("label").empty()) {
				throw schnaps_IOExceptionNodeM(*lChild, "label attribute expected!");
			}

			lAlloc = ioSystem.getFactory().getAllocator(lChild->getFirstChild()->getValue());
			if (lAlloc == NULL) {
				throw schnaps_InternalExceptionM("Could not find allocator for object '" + lChild->getFirstChild()->getValue() + "'!");
			} else {
				lValue = castHandleT<AnyType>(lAlloc->allocate());
				lValue->readWithSystem(lChild->getFirstChild(), ioSystem);
				if (mParametersMap.find(lChild->getAttribute("label")) == mParametersMap.end()) {
					mParametersMap.insert(std::pair<std::string, AnyType::Handle>(lChild->getAttribute("label"), lValue));
				} else {
					mParametersMap[lChild->getAttribute("label")] = lValue;
				}
			}
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

void Parameters::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	for (ParametersMap::const_iterator lIt = mParametersMap.begin(); lIt != mParametersMap.end(); lIt++){
		ioStreamer.openTag("Entry", inIndent);
		ioStreamer.insertAttribute("label", lIt->first);
		lIt->second->write(ioStreamer, inIndent);
		ioStreamer.closeTag();
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::writeContent(PACC::XML::Streamer&, bool) const");
}
