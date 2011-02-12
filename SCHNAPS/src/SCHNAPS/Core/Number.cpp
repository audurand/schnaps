/*
 * Number.cpp
 *
 *  Created on: 2009-02-17
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

Number& Number::operator=(const Number& inOriginal) {
	schnaps_StackTraceBeginM();
		return *this;
	schnaps_StackTraceEndM("core::Number& core::Number::operator=(const core::Number&)");
}

/*!
 *  \brief  Apply the absolute value operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::abs() {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("abs", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::abs()");
}

/*!
 *  \brief  Apply the addition operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::add(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("add", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::add(core::Number&)");
}

/*!
 *  \brief  Apply the subtract operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::sub(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("sub", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::sub(core::Number&)");
}

/*!
 *  \brief  Apply the multiply operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::mult(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("mult", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::mult(core::Number&)");
}

/*!
 *  \brief  Apply the divide operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::div(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("div", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::div(core::Number&)");
}

/*!
 *  \brief  Raise the current number to the power exponent.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::pow(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("pow", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::pow(core::Number&)");
}

/*!
 *  \brief  Apply the modulo operator to a number.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::mod(Number& inRightNumber) {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("mod", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::mod(core::Number&)");
}

/*!
 *  \brief  Compute the base-e exponential function, which is the e number raised to the power x.
 *  \return The resulting number.
 *  \throw  core::InternalException If the method is not overdefined in a subclass.
 */
Number& Number::exp() {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("exp", "Number", getName());
	schnaps_StackTraceEndM("core::Number& core::Number::exp(core::Number&)");
}

Number::operator Double() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator Double", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator Double() const");
}

Number::operator Float() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator Float", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator Float() const");
}

Number::operator Int() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator Int", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator Int() const");
}

Number::operator Long() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator Long", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator Long() const");
}

Number::operator UInt() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator UInt", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator UInt() const");
}

Number::operator ULong() const {
	schnaps_StackTraceBeginM();
		throw schnaps_UndefinedMethodInternalExceptionM("operator ULong", "Number", getName());
	schnaps_StackTraceEndM("core::Number::operator ULong() const");
}
