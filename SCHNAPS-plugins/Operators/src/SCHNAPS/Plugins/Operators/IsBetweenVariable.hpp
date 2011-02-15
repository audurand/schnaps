/*
 * IsBetweenVariable.hpp
 *
 *  Created on: 2010-12-01
 *  Updated on: 2010-12-01
 *      Author: Audrey Durand
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

#ifndef SCHNAPS_Plugins_Operators_IsBetweenVariable_hpp
#define SCHNAPS_Plugins_Operators_IsBetweenVariable_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Operators {

/*!
 *  \class IsBetweenVariable EasierPlugin/include/IsBetweenVariable.hpp "EasierPlugin/include/IsBetweenVariable.hpp"
 *  \brief Check if variable is between these values (lower value <= variable <= upper value).
 */
class IsBetweenVariable: public Core::Primitive {
public:
	//! IsBetweenVariable allocator type.
	typedef Core::AllocatorT<IsBetweenVariable, Core::Primitive::Alloc> Alloc;
	//! IsBetweenVariable handle type.
	typedef Core::PointerT<IsBetweenVariable, Core::Primitive::Handle> Handle;
	//! IsBetweenVariable bag type.
	typedef Core::ContainerT<IsBetweenVariable, Core::Primitive::Bag> Bag;

	IsBetweenVariable();
	IsBetweenVariable(const IsBetweenVariable& inOriginal);
	virtual ~IsBetweenVariable() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Operators_IsBetweenVariable");
			return lName;
		schnaps_StackTraceEndM("const std::string& IsBetweenVariable::getName() const");
	}

	virtual	void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	std::string mLabel; 				//!< label of concerned variable
	std::string mLowerValue_Ref; 		//!< lower value (reference)
	Core::Number::Handle mLowerValue;	//!< lower value
	std::string mUpperValue_Ref; 		//!< upper value (reference)
	Core::Number::Handle mUpperValue;	//!< upper value
};
} // end of Operators namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Operators_IsBetweenVariable_hpp */
