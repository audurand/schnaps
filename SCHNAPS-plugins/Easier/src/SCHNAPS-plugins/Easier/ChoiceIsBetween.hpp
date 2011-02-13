/*
 * ChoiceIsBetween.hpp
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

#ifndef easier_ChoiceIsBetween_hpp
#define easier_ChoiceIsBetween_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class ChoiceIsBetween Simulator/Core/include/ChoiceIsBetween.hpp "Simulator/Core/include/ChoiceIsBetween.hpp"
 *  \brief Primitive class that choses over ranges of values according to an individual variable.
 *  Lower boundaries are given and branches are executing according to LowerBound <= Variable < HigherBound.
 */
class ChoiceIsBetween: public core::Primitive {
public:
	//! ChoiceIsBetween allocator type.
	typedef core::AllocatorT<ChoiceIsBetween, core::Primitive::Alloc> Alloc;
	//! ChoiceIsBetween handle type.
	typedef core::PointerT<ChoiceIsBetween, core::Primitive::Handle> Handle;
	//! ChoiceIsBetween bag type.
	typedef core::ContainerT<ChoiceIsBetween, core::Primitive::Bag> Bag;

	ChoiceIsBetween();
	ChoiceIsBetween(const ChoiceIsBetween& inOriginal);
	virtual ~ChoiceIsBetween() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_ChoiceIsBetween");
			return lName;
		schnaps_StackTraceEndM("const std::string& ChoiceIsBetween::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	ChoiceIsBetween& operator=(const ChoiceIsBetween& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mChoiceVariableLabel;	//!< Current individual variable label to switch on.
	std::string mChoices_Ref;			//!< Reference to choice values.
	core::Vector::Handle mChoices;		//!< Vector of choices (between clusters).
};
} // end of easier namespace

#endif /* easier_ChoiceIsBetween_hpp */
