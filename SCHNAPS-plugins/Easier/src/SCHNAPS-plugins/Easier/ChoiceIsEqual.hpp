/*
 * ChoiceIsEqual.hpp
 *
 *  Created on: 2010-11-20
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

#ifndef easier_ChoiceIsEqual_hpp
#define easier_ChoiceIsEqual_hpp

#include <map>

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class ChoiceIsEqual Simulator/Core/include/ChoiceIsEqual.hpp "Simulator/Core/include/ChoiceIsEqual.hpp"
 *  \brief Primitive class that choses over some values.
 */
class ChoiceIsEqual: public core::Primitive {
protected:
	typedef std::map<core::Atom::Handle, unsigned int, core::IsLessPointerPredicate> ChoiceMap;

public:
	//! ChoiceIsEqual allocator type.
	typedef core::AllocatorT<ChoiceIsEqual, core::Primitive::Alloc> Alloc;
	//! ChoiceIsEqual handle type.
	typedef core::PointerT<ChoiceIsEqual, core::Primitive::Handle> Handle;
	//! ChoiceIsEqual bag type.
	typedef core::ContainerT<ChoiceIsEqual, core::Primitive::Bag> Bag;

	ChoiceIsEqual();
	ChoiceIsEqual(const ChoiceIsEqual& inOriginal);
	virtual ~ChoiceIsEqual() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_ChoiceIsEqual");
			return lName;
		schnaps_StackTraceEndM("const std::string& ChoiceIsEqual::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	ChoiceIsEqual& operator=(const ChoiceIsEqual& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mChoiceVariableLabel;	//!< Current individual variable label to switch on.
	std::string mChoices_Ref;			//!< Reference to choice values.
	ChoiceMap mChoices;					//!< Map of values to children index associated.
	bool mCatchError;					//!< Catch error if variable not in choices.
};
} // end of easier namespace

#endif /* easier_ChoiceIsEqual_hpp */
