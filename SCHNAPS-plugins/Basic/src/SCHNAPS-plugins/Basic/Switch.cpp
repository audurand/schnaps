/*
 * Switch.cpp
 *
 *  Created on: 2009-10-15
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

#include "SCHNAPS-plugins/Basic/Basic.hpp"

using namespace basic;

/*!
 *  \brief Construct a new primitive that returns the result of a Switch.
 */
Switch::Switch() :
		Primitive(),	// Unknown number of children
		mKeys_Ref(""),
		mKeyType("")
{}

Switch::Switch(const Switch& inOriginal) :
		Primitive(inOriginal.getNumberArguments()),
		mKeys_Ref(inOriginal.mKeys_Ref.c_str()),
		mKeyType(inOriginal.mKeyType.c_str())
{
	mSwitchMap.clear();
	for (SwitchMap::const_iterator lIt = inOriginal.mSwitchMap.begin(); lIt != inOriginal.mSwitchMap.end(); lIt++) {
		mSwitchMap.insert(std::pair<core::Atom::Handle, unsigned int>(core::castHandleT<core::Atom>(lIt->first->clone()), lIt->second));
	}
}

void Switch::readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem) {
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

		unsigned int lChoice;

		// Retrieve keys
		if (inIter->getAttribute("keys").empty()) {
			if (inIter->getAttribute("keys.ref").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "keys of the children expected!");
			} else { // Use referenced keys
				mKeys_Ref = inIter->getAttribute("keys.ref");
				std::stringstream lSS;
				lSS << "ref." << mKeys_Ref;

				core::Vector::Handle lKeys = core::castHandleT<core::Vector>(ioSystem.getParameters()[lSS.str().c_str()]);
				for (unsigned int i = 0; i < lKeys->size(); i++) {
					mSwitchMap.insert(std::pair<core::Atom::Handle, unsigned int>(core::castHandleT<core::Atom>((*lKeys)[i]), i));
				}
				lChoice = lKeys->size();
			}
		} else { // Use defined keys
			// Retrieve key type
			if (inIter->getAttribute("keyType").empty()) {
				throw schnaps_IOExceptionNodeM(*inIter, "type of the keys expected!");
			}
			mKeyType = inIter->getAttribute("keyType");
			core::Atom::Alloc::Handle lAlloc = core::castHandleT<core::Atom::Alloc>(ioSystem.getFactory().getAllocator(mKeyType));

			std::stringstream lSS(inIter->getAttribute("keys"));
			PACC::Tokenizer lTokenizer(lSS);
			lTokenizer.setDelimiters("|", "");

			std::string lKey;
			core::Atom::Handle lAtom;

			lChoice = 0;
			while (lTokenizer.getNextToken(lKey)) {
				// Add to map
				lAtom = core::castHandleT<core::Atom>(lAlloc->allocate());
				if (lAtom == NULL) {
					std::ostringstream lOSS;
					lOSS << "no atom named '" <<  mKeyType;
					lOSS << "' found in the factory";
					throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
				}
				lAtom->readStr(lKey);
				mSwitchMap.insert(std::pair<core::Atom::Handle, unsigned int>(lAtom, lChoice));
				lChoice++;
			}
		}

		setNumberArguments(lChoice + 2); // + 1 for the value switched on, +1 for default case
	schnaps_StackTraceEndM("void core::Switch::readWithSystem(PACC::XML::ConstIterator, core::System&)");
}

void Switch::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
		if (mKeys_Ref.empty()) {
			std::string lKeys;
			for (std::map<core::Atom::Handle, unsigned int, core::IsLessPointerPredicate>::const_iterator lIt = mSwitchMap.begin(); lIt != mSwitchMap.end(); lIt++) {
				lKeys += (*lIt).first->writeStr();
				lKeys += " ";
			}
			ioStreamer.insertAttribute("keyType", mKeyType);
			ioStreamer.insertAttribute("keys", lKeys);
		}

		ioStreamer.insertAttribute("keys.ref", mKeys_Ref);
	schnaps_StackTraceEndM("void core::Switch::writeContent(PACC::XML::Streamer&, bool) const");
}

Switch& Switch::operator=(const Switch& inOriginal) {
	schnaps_StackTraceBeginM();
		this->setNumberArguments(inOriginal.getNumberArguments());
		mKeys_Ref = inOriginal.mKeys_Ref.c_str();
		mKeyType = inOriginal.mKeyType.c_str();
		mSwitchMap.clear();
		for (SwitchMap::const_iterator lIt = inOriginal.mSwitchMap.begin(); lIt != inOriginal.mSwitchMap.end(); lIt++) {
			mSwitchMap.insert(std::pair<core::Atom::Handle, unsigned int>(core::castHandleT<core::Atom>(lIt->first->clone()), lIt->second));
		}
		return *this;
	schnaps_StackTraceEndM("core::Switch& core::Switch::operator=(const core::Switch&)");
}

core::AnyType::Handle Switch::execute(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		core::Atom::Handle lArg = core::castHandleT<core::Atom>(getArgument(inIndex, 0, ioContext));
		SwitchMap::const_iterator lIt = mSwitchMap.find(lArg);
		if (lIt != mSwitchMap.end()) {
			return getArgument(inIndex, (*lIt).second+2, ioContext); // +2 to skip switch value and default case
		}
		// Default
		return getArgument(inIndex, 1, ioContext);
	schnaps_StackTraceEndM("core::AnyType::Handle core::Switch::execute(unsigned int, core::ExecutionContext&) const");
}

const std::string& Switch::getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
	schnaps_AssertM(inN<getNumberArguments());
		if (inN == 0) {
			if (mKeys_Ref.empty()) {
				 return mKeyType;
			} else {
				std::stringstream lSS;
				lSS << "ref." << mKeys_Ref;

				core::Vector::Handle lKeys = core::castHandleT<core::Vector>(ioContext.getSystem().getParameters()[lSS.str().c_str()]);
				if (lKeys->size() < 1) {
					const static std::string lType("Void");
					return lType;
				} else {
					return (*lKeys)[1]->getType();
				}
			}
		} else if (inN == 1) {
			const static std::string lType("Any");
			return lType;
		}
		unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
		return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
	schnaps_StackTraceEndM("const std::string& core::Switch::getArgType(unsigned int, unsigned int, core::ExecutionContext&) const");
}

const std::string& Switch::getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const {
	schnaps_StackTraceBeginM();
		if (getNumberArguments() < 3) {
			unsigned int lNodeIndex = getArgumentIndex(inIndex, 1, ioContext);
			return ioContext.getPrimitiveTree()[lNodeIndex].mPrimitive->getReturnType(inIndex, ioContext);
		}

		std::string lCommonType = ioContext.getSystem().getTypingManager().commonType(getArgType(inIndex, 1, ioContext), getArgType(inIndex, 2, ioContext));

		for (unsigned int i = 3; i < getNumberArguments(); i++) {
			lCommonType = ioContext.getSystem().getTypingManager().commonType(lCommonType, getArgType(inIndex, i, ioContext));
		}

		const static std::string lCommonType_Final(lCommonType);
		return lCommonType_Final;
	schnaps_StackTraceEndM("const std::string& core::Switch::getReturnType(unsigned int, core::ExecutionContext&) const");
}
