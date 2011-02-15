/*
 * Token.hpp
 *
 *  Created on: 2009-02-19
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

#ifndef SCHNAPS_Plugins_Data_Token_hpp
#define SCHNAPS_Plugins_Data_Token_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"

namespace SCHNAPS {
namespace Plugins {
namespace Data {

/*!
 *  \class Token SCHNAPS-plugins/Data/Token.hpp "SCHNAPS-plugins/Data/Token.hpp"
 *  \brief Simple token terminal primitive class.
 */
class Token: public Core::Primitive {
public:
	//! Token allocator type.
	typedef Core::AllocatorT<Token, Core::Primitive::Alloc> Alloc;
	//! Token handle type.
	typedef Core::PointerT<Token, Core::Primitive::Handle> Handle;
	//! Token bag type.
	typedef Core::ContainerT<Token, Core::Primitive::Bag> Bag;

	Token();
	Token(const Token& inOriginal);
	virtual ~Token() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Data_Token");
			return lName;
		schnaps_StackTraceEndM("const std::string& Token::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Token& operator=(const Token& inOriginal);

	virtual Core::AnyType::Handle execute(unsigned int inIndex, Core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, Core::ExecutionContext& ioContext) const;

private:
	Core::Atom::Handle mTokenHandle; //!< handle to the value contained by token
};
} // end of Data namespace
} // end of Plugins namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Plugins_Data_Token_hpp */
