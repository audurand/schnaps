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

#ifndef basic_Token_hpp
#define basic_Token_hpp

#include "PACC/XML.hpp"
#include "SCHNAPS/SCHNAPS.hpp"


namespace basic {
/*!
 *  \class Token SCHNAPS-plugins/Basic/Token.hpp "SCHNAPS-plugins/Basic/Token.hpp"
 *  \brief Simple token terminal primitive class.
 */
class Token: public core::Primitive {
public:
	//! Token allocator type.
	typedef core::AllocatorT<Token, core::Primitive::Alloc> Alloc;
	//! Token handle type.
	typedef core::PointerT<Token, core::Primitive::Handle> Handle;
	//! Token bag type.
	typedef core::ContainerT<Token, core::Primitive::Bag> Bag;

	Token();
	Token(const Token& inOriginal);
	virtual ~Token() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Basic_Token");
			return lName;
		schnaps_StackTraceEndM("const std::string& Token::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	Token& operator=(const Token& inOriginal);

	virtual core::AnyType::Handle execute(unsigned int inIndex, core::ExecutionContext& ioContext) const;
	virtual const std::string& getReturnType(unsigned int inIndex, core::ExecutionContext& ioContext) const;

private:
	core::Atom::Handle mTokenHandle; //!< Handle to the value contained by token.
};
} // end of basic namespace

#endif /* basic_Token_hpp */
