/*
 * Number.cpp
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
 * \brief Copy operator.
 * \return A reference to the current number.
 */
Number& Number::operator=(const Number& inOriginal) {
	schnaps_StackTraceBeginM();
	return *this;
	schnaps_StackTraceEndM("SCHNAPS::Core::Number& SCHNAPS::Core::Number::operator=(const SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the absolute value.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::abs() {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("abs", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::abs()");
}

/*!
 * \brief  Compute the addition a number.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("add", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::add(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the division by a number.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("div", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::div(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the base-e exponential function, which is the e number raised to the power x.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::exp() {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("exp", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::exp(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the modulo by a number.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("mod", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::mod(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the multiplication by a number.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("mult", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::mult(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute Raise the current number to the power exponent.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("pow", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::pow(SCHNAPS::Core::Number&)");
}

/*!
 * \brief  Compute the subtraction by a number.
 * \return A handle to the resulting number.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::Handle Number::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("sub", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::Handle SCHNAPS::Core::Number::sub(SCHNAPS::Core::Number&)");
}

/*!
 * \brief Casting operator to double.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator Double() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator Double", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator Double() const");
}

/*!
 * \brief Casting operator to float.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator Float() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator Float", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator Float() const");
}

/*!
 * \brief Casting operator to integer.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator Int() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator Int", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator Int() const");
}

/*!
 * \brief Casting operator to long.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator Long() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator Long", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator Long() const");
}

/*!
 * \brief Casting operator to unsigned integer.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator UInt() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator UInt", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator UInt() const");
}
/*!
 * \brief Casting operator to unsigned long.
 * \throw  SCHNAPS::Core::InternalException if the method is not overdefined in a subclass.
 */
Number::operator ULong() const {
	schnaps_StackTraceBeginM();
	throw schnaps_UndefinedMethodInternalExceptionM("operator ULong", "Number", getName());
	schnaps_StackTraceEndM("SCHNAPS::Core::Number::operator ULong() const");
}
