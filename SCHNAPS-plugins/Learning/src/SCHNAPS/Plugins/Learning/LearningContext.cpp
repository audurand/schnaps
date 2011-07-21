/*
 * LearningContext.cpp
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

#include "SCHNAPS/Plugins/Learning/Learning.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

/*!
 * \brief Default constructor.
 */
LearningContext::LearningContext() :
	mState(""),
	mActionLabel("")
{}

/*!
 * \brief Construct an execution learning context as a copy of an original.
 * \param inOriginal A const reference to the original execution learning context.
 */
LearningContext::LearningContext(const LearningContext& inOriginal) :
	mState(inOriginal.mState.c_str()),
	mActionLabel(inOriginal.mActionLabel.c_str())
{}
