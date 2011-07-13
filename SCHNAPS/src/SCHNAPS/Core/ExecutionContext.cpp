/*
 * ExecutionContext.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#include "SCHNAPS/Core.hpp"

using namespace SCHNAPS;
using namespace Core;

/*!
 * \brief Default constructor.
 */
ExecutionContext::ExecutionContext() :
	mSystem(NULL),
	mPrimitiveTree(NULL),
	mThreadNb(0)
{}

/*!
 * \brief Copy constructor.
 */
ExecutionContext::ExecutionContext(const ExecutionContext& inExecutionContext) :
	mSystem(inExecutionContext.mSystem),
	mPrimitiveTree(inExecutionContext.mPrimitiveTree),
	mThreadNb(0)
{}

/*!
 * \brief Construct an execution context with specific system.
 * \param inSystem A const handle to the system to set.
 */
ExecutionContext::ExecutionContext(const System::Handle inSystem) :
	mSystem(inSystem),
	mPrimitiveTree(NULL),
	mThreadNb(0)
{}

/*!
 * \brief Copy operator.
 * \param inOriginal Source of copy.
 * \return A reference to the current object.
 */
ExecutionContext& ExecutionContext::operator=(const ExecutionContext& inOriginal) {
	schnaps_StackTraceBeginM();
	mSystem = inOriginal.mSystem;
	mPrimitiveTree = inOriginal.mPrimitiveTree;
	mThreadNb = inOriginal.mThreadNb;
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::ExecutionContext& SCHNAPS::Core::ExecutionContext::operator=(const SCHNAPS::Core::ExecutionContext&)");
}
