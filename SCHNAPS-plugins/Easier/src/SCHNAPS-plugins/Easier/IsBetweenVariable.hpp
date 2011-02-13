/*
 * IsBetweenVariable.hpp
 *
 *  Created on: 2010-12-01
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

#ifndef easier_IsBetweenVariable_hpp
#define easier_IsBetweenVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace easier {
/*!
 *  \class IsBetweenVariable SCHNAPS-plugins/Easier/IsBetweenVariable.hpp "SCHNAPS-plugins/Easier/IsBetweenVariable.hpp"
 *  \brief Check if variable is between these values (lower value <= variable <= upper value).
 */
class IsBetweenVariable: public core::Primitive {
public:
	//! IsBetweenVariable allocator type.
	typedef core::AllocatorT<IsBetweenVariable, core::Primitive::Alloc> Alloc;
	//! IsBetweenVariable handle type.
	typedef core::PointerT<IsBetweenVariable, core::Primitive::Handle> Handle;
	//! IsBetweenVariable bag type.
	typedef core::ContainerT<IsBetweenVariable, core::Primitive::Bag> Bag;

	IsBetweenVariable();
	IsBetweenVariable(const IsBetweenVariable& inOriginal);
	virtual ~IsBetweenVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Easier_IsBetweenVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsBetweenVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 				//!< Label of concerned variable.
	std::string mLowerValue_Ref; 		//!< Lower value (reference).
	core::Number::Handle mLowerValue;	//!< Lower value.
	std::string mUpperValue_Ref; 		//!< Upper value (reference).
	core::Number::Handle mUpperValue;	//!< Upper value.
};
} // end of easier namespace

#endif /* easier_IsBetweenVariable_hpp */
