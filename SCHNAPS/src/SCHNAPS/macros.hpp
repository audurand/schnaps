/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \brief Miscellaneous utility classes and functions, part of the Object Oriented Foundations.
 */

#ifndef macros_hpp
#define macros_hpp

#include "SCHNAPS/config.hpp"

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef SCHNAPS_HAVE_NUMERIC_LIMITS
#include <limits>
#endif // SCHNAPS_HAVE_NUMERIC_LIMITS

/*!
 *  \def    schnaps_MinM(X1,X2)
 *  \brief  Calculate the minimum between two values.
 *  \param  X1 First value to compare.
 *  \param  X2 Second value to compare.
 *  \return Minimum of the two values.
 *  \deprecated Use inline function minOf instead.
 */
#define schnaps_MinM(X1,X2) ( ((X1)<(X2)) ? (X1) : (X2) )

/*!
 *  \def    schnaps_MaxM(X1,X2)
 *  \brief  Calculate the maximum between two values.
 *  \param  X1 First value to compare.
 *  \param  X2 Second value to compare.
 *  \return Maximum of the two values.
 *  \deprecated Use inline function maxOf instead.
 */
#define schnaps_MaxM(X1,X2) ( ((X1)>(X2)) ? (X1) : (X2) )

/*!
 *  \def    schnaps_Pow2M(X1,X2)
 *  \brief  Calculate the square of a value.
 *  \param  X Value to compute the square.
 *  \return Square of value.
 *  \deprecated Use inline function pow2Of instead.
 */
#define schnaps_Pow2M(X) ( (X)*(X) )

namespace SCHNAPS {
	/*!
	 *  \brief Test if a floating-point number is in a not-a-number state.
	 *  \param inValue Value to evaluate not-a-number state.
	 *  \return True if the floating-point value is not-a-number, otherwise false.
	 */
	template<class T>
	inline bool isNaN(const T& inValue) {
		return ((inValue == inValue) == false);
	}

	/*!
	 *  \brief Test if a floating-point number is infinite.
	 *  \param inValue Value to evaluate finiteness.
	 *  \return True if the floating-point value is equal to infinity, otherwise false.
	 */
	template<class T>
	inline bool isInfinity(const T& inValue) {
#ifdef SCHNAPS_HAVE_NUMERIC_LIMITS
		if (std::numeric_limits<T>::has_infinity) {
			return ((inValue == std::numeric_limits<T>::infinity()) || (inValue
					== -std::numeric_limits<T>::infinity()));
		}
#endif // SCHNAPS_HAVE_NUMERIC_LIMITS
		T lZero(0.0);
		T lInfinity(1.0 / lZero);
		return ((inValue == lInfinity) || (inValue == -lInfinity));
	}

	/*!
	 *  \brief Test if a floating-point number is finite, that is not NaN or infinite.
	 *  \param inValue Value to evaluate finiteness.
	 *  \return True if the floating-point value is finite, otherwise false.
	 */
	template<class T>
	inline bool isFinite(const T& inValue) {
		return ((isNaN<T> (inValue) == false) && (isInfinity(inValue) == false));
	}

	/*!
	 *  \brief Evaluate the maximum of two values.
	 *  \param inValue1 First value from which we want the maximum.
	 *  \param inValue2 Second value from which we want the maximum.
	 *  \return Maximum value between inValue1 and inValue2.
	 */
	template<class T>
	inline T maxOf(const T& inValue1, const T& inValue2) {
		if (isNaN(inValue1))
			return inValue2;
		else if (isNaN(inValue2))
			return inValue1;
		return ((inValue2 < inValue1) ? inValue1 : inValue2);
	}

	/*!
	 *  \brief Evaluate the minimum of two values.
	 *  \param inValue1 First value from which we want the minimum.
	 *  \param inValue2 Second value from which we want the minimum.
	 *  \return Minimum value between inValue1 and inValue2.
	 */
	template<class T>
	inline T minOf(const T& inValue1, const T& inValue2) {
		if (isNaN(inValue1))
			return inValue2;
		else if (isNaN(inValue2))
			return inValue1;
		return ((inValue1 < inValue2) ? inValue1 : inValue2);
	}

	/*!
	 *  \brief Evaluate square of a number (X^2).
	 *  \param inValue Value to evaluate the square.
	 *  \return Square of the value.
	 */
	template<class T>
	inline T pow2Of(const T& inValue) {
		return (inValue * inValue);
	}

	/*!
	 *  \brief Evaluate absolute value of a number.
	 *  \param inValue Value to evaluate the absolute value.
	 *  \return Absolute value of the input.
	 */
	template<class T>
	inline T absolute(const T& inValue) {
		return (inValue < 0) ? -inValue : inValue;
	}

	/*!
	 *  \brief Evaluate absolute value of a double.
	 *  \param inValue Double to evaluate the absolute value.
	 *  \return Absolute value of the input.
	 */
	template<>
	inline double absolute(const double& inValue) {
		return std::fabs(inValue);
	}

	/*!
	 *  \brief Evaluate absolute value of a float.
	 *  \param inValue Float to evaluate the absolute value.
	 *  \return Absolute value of the input.
	 */
	template<>
	inline float absolute(const float& inValue) {
		return std::fabs(inValue);
	}

	/*!
	 *  \brief Evaluate absolute value of a long integer.
	 *  \param inValue Long integer to evaluate the absolute value.
	 *  \return Absolute value of the input.
	 */
	template<>
	inline long absolute(const long& inValue) {
		return std::labs(inValue);
	}

	/*!
	 *  \brief Evaluate absolute value of a int.
	 *  \param inValue Integer to evaluate the absolute value.
	 *  \return Absolute value of the input.
	 */
	template<>
	inline int absolute(const int& inValue) {
		return std::abs(inValue);
	}

	/*!
	 *  \brief Round double to the nearest integer, rounding half-way cases away from 0.
	 *  \param inValue Value to round
	 *  \return Rounded values to nearest integer.
	 */
	inline double round(double inValue) {
		return (inValue < 0.0) ? std::ceil(inValue - 0.5) : std::floor(inValue
				+ 0.5);
	}

	/*!
	 *  \brief  Convert an integer into a string.
	 *  \param  inInteger Integer to convert into a string.
	 *  \return string containing conversion of the integer.
	 */
	inline std::string int2str(long inInteger) {
		std::ostringstream lStringOS;
		lStringOS << inInteger;
		const std::string lStr = lStringOS.str();
		return lStr;
	}

	/*!
	 *  \brief  Convert a string into a integer.
	 *  \param  inString String to convert into an integer.
	 *  \return Long containing the conversion of the string.
	 */
	inline long str2int(const std::string& inString) {
		std::string lStr(inString);
		std::istringstream lStringIS(lStr);
		long lInteger;
		lStringIS >> lInteger;
		return lInteger;
	}

	/*!
	 *  \brief  Convert an unsigned integer into a string.
	 *  \param  inInteger Integer to convert into a string.
	 *  \return string containing conversion of the integer.
	 */
	inline std::string uint2str(unsigned long inInteger) {
		std::ostringstream lStringOS;
		lStringOS << inInteger;
		const std::string lStr = lStringOS.str();
		return lStr;
	}

	/*!
	 *  \brief  Convert a string into an unsigned integer.
	 *  \param  inString String to convert into an integer.
	 *  \return Unsigned long containing the conversion of the string.
	 */
	inline unsigned long str2uint(const std::string& inString) {
		std::istringstream lStringIS(inString);
		unsigned long lInteger;
		lStringIS >> lInteger;
		return lInteger;
	}

	/*!
	 *  \brief  Convert a double into a string.
	 *  \param  inDouble Double to convert into a string.
	 *  \param  inPrecision Precision of the conversion.
	 *  \return String containing conversion of the double.
	 */
	inline std::string dbl2str(double inDouble, unsigned int inPrecision = 12) {
		std::ostringstream lStringOS;
		lStringOS.precision(inPrecision);
		if (isNaN(inDouble))
			lStringOS << "nan";
		else if (isInfinity(inDouble)) {
			if (inDouble < 0.)
				lStringOS << "-inf";
			else
				lStringOS << "inf";
		} else
			lStringOS << inDouble;
		const std::string lStr = lStringOS.str();
		return lStr;
	}

	/*!
	 *  \brief  Convert a string into a double.
	 *  \param  inString String to convert into a double.
	 *  \return Double containing the conversion of the string.
	 */
	inline double str2dbl(const std::string& inString) {
		if (inString == "nan") {
			double lZero(0.);
			return (lZero / lZero);
		} else if (inString == "inf") {
#ifdef SCHNAPS_HAVE_NUMERIC_LIMITS
			if (std::numeric_limits<double>::has_infinity) {
				return std::numeric_limits<double>::infinity();
			}
#endif // SCHNAPS_HAVE_NUMERIC_LIMITS
			double lZero(0.);
			return (1. / lZero);
		} else if (inString == "-inf") {
#ifdef SCHNAPS_HAVE_NUMERIC_LIMITS
			if (std::numeric_limits<double>::has_infinity) {
				return -std::numeric_limits<double>::infinity();
			}
#endif // SCHNAPS_HAVE_NUMERIC_LIMITS
			double lZero(0.);
			return (-1. / lZero);
		}
		std::istringstream lStringIS(inString);
		double lDouble;
		lStringIS >> lDouble;
		return lDouble;
	}

	/*!
	 *  \brief Convert a integer to its ordinal form , i.e. 1 -> 1st, 22 -> 22nd.
	 *  \param inNumber Integer to convert.
	 *  \return Ordinal form of the number.
	 *  \author Matthew Walker
	 *  \author Christian Gagne
	 */
	inline std::string uint2ordinal(unsigned int inNumber) {
		// write number to string
		std::ostringstream outStream;
		outStream << inNumber;
		std::string lString = outStream.str();
		// insert commas for easier reading
		if (inNumber > 9999) {
			unsigned int lCount = 0;
			for (unsigned int i = lString.length(); i > 1; --i) {
				++lCount;
				if (lCount == 3) {
					lString.insert(i - 1, ",");
					lCount = 0;
				}
			}
		}
		// choose appropriate suffix
		std::string lSuffix = "th";
		switch (inNumber % 10) {
		case 1: {
			lSuffix = "st";
			break;
		}
		case 2: {
			lSuffix = "nd";
			break;
		}
		case 3: {
			lSuffix = "rd";
			break;
		}
		}
		switch (inNumber % 100) {
		case 11:
		case 12:
		case 13: {
			lSuffix = "th";
			break;
		}
		}
		lString.append(lSuffix);
		return lString;
	}

	/*!
	 *  \brief Wrap a string to fit a given line witdh.
	 *  \param ioString String to wrap.
	 *  \param inLineWidth Line width of the converted string.
	 */
	inline void wrapString(std::string& ioString, unsigned int inLineWidth = 80) {
		// can't do a line width of zero (setting to zero equates to turning off this algorithm)
		if (inLineWidth == 0)
			return;

		bool lBreakPointValid = false;
		bool lNewLine = true;
		unsigned int lBestBreakPoint = 0;
		unsigned int lFixed = 0;

		// loop through all the characters in the string
		for (unsigned int i = 0; i < ioString.size(); ++i) {

			// find next character if this is a new line
			if (lNewLine) {
				// find the next non-whitespace character to start the line.
				std::string::size_type lNextChar = ioString.find_first_not_of(
						" \t", i);
				if (lNextChar == std::string::npos) {
					// the rest of the string is just whitespace. Dump it.
					ioString.erase(i - 1);
					return;
				}
				// swallow one newline.
				if (ioString[lNextChar] == '\n') {
					++lNextChar;
				}
				// remove the whitespace.
				if (lNextChar > i) {
					ioString.erase(i, lNextChar - i);
				}
				lNewLine = false;
			}

			// check if i has gone over line width (this shouldn't happen)
			if (i > lFixed + inLineWidth)
				return;

			// check if i is at line width
			if (i == lFixed + inLineWidth) {
				if (!lBreakPointValid) {
					// breaking in the middle of a word
					ioString.insert(i, "\n");
					lFixed = i + 1;
					i = lFixed - 1; // because at the continue i will be incremented
					lBreakPointValid = false;
					lNewLine = true;
					continue;
				} else {
					// breakpoint valid
					if (ioString[lBestBreakPoint] == ' '
							|| ioString[lBestBreakPoint] == '\t') {
						ioString[lBestBreakPoint] = '\n';
						lFixed = lBestBreakPoint + 1;
						i = lFixed - 1;
						lBreakPointValid = false;
						lNewLine = true;
						continue;
					}
				}
			}

			// check if current char is a newline
			const char lCurrentChar = ioString[i];
			if (lCurrentChar == '\n') {
				lFixed = i + 1;
				lBreakPointValid = false;
				lNewLine = true;
				continue;
			}

			// look for whitespace to set breakpoints
			if (lCurrentChar == ' ' || lCurrentChar == '\t') {
				lBestBreakPoint = i;
				lBreakPointValid = true;
			}
		}
	}
} // end of SCHNAPS namespace

#endif // macros_hpp
