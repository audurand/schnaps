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
 * \brief Construct object parameters component.
 */
Parameters::Parameters() :
	Component("Parameters")
{}

/*!
 * \brief Read object from XML using system.
 * \param inIter XML iterator of input document.
 * \param ioSystem A reference to the system.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Parameters::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem) {
	schnaps_StackTraceBeginM();
	if ((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Parameters")) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag <Parameters> expected!");
	}
			
#ifdef SCHNAPS_FULL_DEBUG
	std::cout << "Reading parameters\n";
#endif

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
			lValue = castHandleT<AnyType>(lAlloc->allocate());
			lValue->readWithSystem(lChild->getFirstChild(), ioSystem);
			if (mParametersMap.find(lChild->getAttribute("label")) == mParametersMap.end()) {
				mParametersMap.insert(std::pair<std::string, AnyType::Handle>(lChild->getAttribute("label"), lValue));
			} else {
				mParametersMap[lChild->getAttribute("label")] = lValue;
			}
			
#ifdef SCHNAPS_FULL_DEBUG
			std::cout << "- " << lChild->getAttribute("label") << " = " << lValue->writeStr() << "\n";
#endif
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::readWithSystem(PACC::XML::ConstIterator, SCHNAPS::Core::System&)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
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

/*!
 * \brief Read data from string.
 * \param inStr A const reference to read data from.
 */
void Parameters::readStr(const std::string& inStr) {
	schnaps_StackTraceBeginM();
	std::stringstream lISS(inStr);
	PACC::Tokenizer lTokenizer(lISS);
	lTokenizer.setDelimiters(",", "");

	std::string lOption;
	std::string::size_type lPos;

	ParametersMap::iterator lIterParameters;
	while (lTokenizer.getNextToken(lOption)) {
		lPos = lOption.find("=");

		if (lPos == std::string::npos || lPos == lOption.size()-1) {
			std::cout << "Expected value for parameter " <<  lOption.substr(0, lPos) << "!\n";
		} else {
			lIterParameters = mParametersMap.find(lOption.substr(0, lPos));
			if (lIterParameters == mParametersMap.end()) {
				std::ostringstream lOSS;
				lOSS << "The parameter '" << lOption.substr(0, lPos);
				lOSS << "' does not exist; ";
				lOSS << "could not configure it.";
				throw schnaps_RunTimeExceptionM(lOSS.str());
			}
			lIterParameters->second->readStr(lOption.substr(lPos+1));
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::readStr(const std::string&)");
}

/*!
 * \brief Write data to string.
 * \return A string of parameters.
 */
std::string Parameters::writeStr() const {
	schnaps_StackTraceBeginM();
	std::stringstream lSS;
	std::string lParameters;

	for (ParametersMap::const_iterator lIt = mParametersMap.begin(); lIt != mParametersMap.end(); lIt++) {
		lSS << lIt->first.c_str() << "=" << lIt->second->writeStr().c_str() << ",";
	}
	lParameters = lSS.str();
	lParameters.erase(lParameters.end()-1);
	return lParameters;
	schnaps_StackTraceEndM("std::string SCHNAPS::Core::Parameters::writeStr() const");
}

/*!
 * \brief Insert a new parameter with specific value.
 * \param inLabel A const reference to the label of the parameter.
 * \param inValue A handle to the the parameter.
 * \throw SCHNAPS::Core::RunTimeException if the parameter already exists.
 */
void Parameters::insertParameter(const std::string& inLabel, AnyType::Handle inValue) {
	schnaps_StackTraceBeginM();
	if (mParametersMap.find(inLabel) != mParametersMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The parameter '" << inLabel;
		lOSS << "' is already present in the map of parameters; ";
		lOSS << "could not insert it.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	mParametersMap.insert(std::pair<std::string, AnyType::Handle>(inLabel.c_str(), inValue));
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::insertParameter(const std::string&, SCHNAPS::Core::AnyType::Handle)");
}

/*!
 * \brief Set parameter value.
 * \param inLabel A const reference to the label of the parameter.
 * \param inValue A const handle to the new value.
 * \throw SCHNAPS::Core::RunTimeException if the parameter does not exist.
 * \throw  SCHNAPS::Core::AssertException if the parameter is NULL.
 */
void Parameters::setParameter(const std::string& inLabel, const AnyType::Handle inValue) {
	schnaps_StackTraceBeginM();
	ParametersMap::iterator lIterParameters = mParametersMap.find(inLabel);
	if (lIterParameters == mParametersMap.end()) {
		std::ostringstream lOSS;
		lOSS << "The parameter '" << inLabel << "' does not exist; ";
		lOSS << "could not read its value.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	schnaps_NonNullPointerAssertM(lIterParameters->second);
	lIterParameters->second->readStr(inValue->writeStr());
	schnaps_StackTraceEndM("void SCHNAPS::Core::Parameters::setParameterValue(const std::string&, const AnyType::Handle)");
}
