/*
 * Primitive.h
 *
 *  Created on: 2009-02-03
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

#ifndef core_Primitive_hpp
#define core_Primitive_hpp

#include <limits.h>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/AbstractAllocT.hpp"
#include "SCHNAPS/Core/PointerT.hpp"
#include "SCHNAPS/Core/ContainerT.hpp"
#include "SCHNAPS/Core/AnyType.hpp"

namespace core {

// Forward declaration
class ExecutionContext;

/*!
 *  \class Primitive Simulator/include/Primitive.hpp "Simulator/include/Primitive.hpp"
 *  \brief Primitive class, the implementation of a primitive as Object.
 */
class Primitive: public Object {
public:
	//! Specify the constants for specific kind of primitive arguments.
	enum ArgumentKind {
		eTerminal = 0, eAny = UINT_MAX
	};

	//! Primitive allocator type.
	typedef AbstractAllocT<Primitive, Object::Alloc> Alloc;
	//! Primitive handle type.
	typedef PointerT<Primitive, Object::Handle> Handle;
	//! Primitive bag type.
	typedef ContainerT<Primitive, Object::Bag> Bag;

	Primitive(const Primitive& inOriginal);
	explicit Primitive(unsigned int inNumberArguments = Primitive::eAny);
	virtual ~Primitive() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("Primitive");
			return lName;
		schnaps_StackTraceEndM("const std::string& Primitive::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& inStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;

	Primitive& operator=(const Primitive& inOriginal);

	//! Execute the primitive.
	virtual AnyType::Handle execute(unsigned int inIndex, ExecutionContext& ioContext) const;
	//! Return the nth argument requested return type (for STGP).
	virtual const std::string& getArgType(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;
	//! Return the primitive return type (for STGP).
	virtual const std::string& getReturnType(unsigned int inIndex, ExecutionContext& ioContext) const;
	//! Validate primitive and children recursively.
	bool isValid(unsigned int inIndex, ExecutionContext& ioContext) const;

	/*!
	 * \brief Get number of arguments of primitive.
	 * \return Number of arguments.
	 */
	unsigned int getNumberArguments() const {
		return mNumberArguments;
	}

	/*!
	 * \brief Set number of arguments of primitive.
	 * \param inNumberArguments Number of arguments.
	 */
	void setNumberArguments(unsigned int inNumberArguments) {
		mNumberArguments = inNumberArguments;
	}

protected:
	//! Get index in the primitive tree of the Nth argument to actual primitive.
	unsigned int getArgumentIndex(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;
	//! Get the value of the nth argument.
	AnyType::Handle getArgument(unsigned int inIndex, unsigned int inN, ExecutionContext& ioContext) const;

private:
	unsigned int mNumberArguments; //!< Number of arguments of the primitive.
};
} // end of core namespace

#endif /* core_Primitive_hpp */
