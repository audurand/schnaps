/*
 * ExecutionContext.cpp
 *
 *  Created on: 2010-03-31
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

#include "SCHNAPS/Core.hpp"

using namespace core;

ExecutionContext::ExecutionContext() :
		mSystem(NULL),
		mPrimitiveTree(NULL),
		mThreadNb(0)
{}

ExecutionContext::ExecutionContext(const ExecutionContext& inExecutionContext) :
		mSystem(inExecutionContext.mSystem),
		mPrimitiveTree(inExecutionContext.mPrimitiveTree),
		mThreadNb(0)
{}

ExecutionContext::ExecutionContext(const System::Handle inSystem) :
		mSystem(inSystem),
		mPrimitiveTree(NULL),
		mThreadNb(0)
{}

ExecutionContext& ExecutionContext::operator=(const ExecutionContext& inOriginal) {
	schnaps_StackTraceBeginM();
		mSystem = inOriginal.mSystem;
		mPrimitiveTree = inOriginal.mPrimitiveTree;
		mThreadNb = inOriginal.mThreadNb;
		return *this;
	schnaps_StackTraceEndM("core::ExecutionContext& core::ExecutionContext::operator=(const core::ExecutionContext&)");
}
