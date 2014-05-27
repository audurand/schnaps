/*
 * Clock.hpp
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

#ifndef SCHNAPS_Simulation_Clock_hpp
#define SCHNAPS_Simulation_Clock_hpp

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Core/Bool.hpp"
#include "SCHNAPS/Core/PrimitiveTree.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class Clock SCHNAPS/Simulation/Clock.hpp "SCHNAPS/Simulation/Clock.hpp"
 *  \brief The clock that drives the simulation.
 */
class Clock: public Core::Object {
public:
	//! Clock allocator type.
	typedef Core::AllocatorT<Clock, Core::Object::Alloc> Alloc;
	//! Clock handle type.
	typedef Core::PointerT<Clock, Core::Object::Handle> Handle;
	//! Clock bag type.
	typedef Core::ContainerT<Clock, Core::Object::Bag> Bag;
	
	//! The units of each clock tick (left = less specific, right = more specific).
	enum Units {eOther, eYear, eMonth, eDay, eWeek};

	Clock();
	Clock(const Clock& inOriginal);
	virtual ~Clock() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("Clock");
		return lName;
		schnaps_StackTraceEndM("const std::string& Clock::getName() const");
	}

	//! Read object from XML using system.
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, Core::System& ioSystem);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	/*!
	 * \brief Reset the value of clock to zero.
	 */
	void reset() {
		schnaps_StackTraceBeginM();
		mValue = 0;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::Clock::reset()");
	}

	/*!
	 * \brief  Have the clock stepping of one time unit.
	 * \param  ioContext A reference to the execution context.
	 * \return True if the simulation continues, false if it the stop condition was encountered.
	 */
	bool step(Core::ExecutionContext& ioContext) {
		schnaps_StackTraceBeginM();
		schnaps_NonNullPointerAssertM(mStop);
		mValue++;
		return !Core::castHandleT<Core::Bool>(mStop->interpret(ioContext))->getValue();
		schnaps_StackTraceEndM("bool SCHNAPS::Simulation::Clock::step(SCHNAPS::Core::ExecutionContext&)");
	}

	/*!
	 * \brief  Return the current value in specific units.
	 * \param  inUnits The specific units in which to return the clock value.
	 * \return The current value in specific units.
	 * \throw  SCHNAPS::Core::RunTimeException if these units are mroe specific than the simulation units
	 */
	unsigned long getValue(Units inUnits = eOther) const {
		schnaps_StackTraceBeginM();
		switch (inUnits) {
			case eYear:
				switch (mUnits) {
					case eYear:
						return mValue;
					case eMonth:
						return mValue / 12;
					case eWeek:
						return mValue / 52;
					case eDay:
						return mValue / 365;
					default:
						throw schnaps_RunTimeExceptionM("Cannot get value: these units are more specific than the simulation units!");
				}
				break;
			case eMonth:
				switch (mUnits) {
					case eMonth:
						return mValue;
					case eDay: {
						unsigned int lDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
						unsigned int lYear = mValue / 365;
						unsigned int lYearDay = mValue % 365;
						unsigned int lSumDays = 0;
						unsigned int lMonth;
						for (lMonth = 0; lMonth < 12; lMonth++) {
							lSumDays += lDays[lMonth];
							if (lSumDays > lYearDay) {
								break;
							}
						}
						return lYear * 12 + lMonth;}
					default:
						throw schnaps_RunTimeExceptionM("Cannot get value: these units are more specific than the simulation units!");
				}
				break;
			case eWeek:
                                switch (mUnits) {
                                        case eDay:
                                                return mValue;
					case eWeek:
						return mValue / 7;
                                        default:
                                                throw schnaps_RunTimeExceptionM("Cannot get value: these units are more specific than the simulation units!");
                                }
                                break;
			case eDay:
				switch (mUnits) {
					case eDay:
						return mValue;
					default:
						throw schnaps_RunTimeExceptionM("Cannot get value: these units are more specific than the simulation units!");
				}
				break;
			default:
				//other
				return mValue;
		}
		schnaps_StackTraceEndM("unsigned long SCHNAPS::Simulation::Clock::getValue(SCHNAPS::Simulation::Clock::Units) const");
	}
	
	/*!
	 * \brief  Get the clock unit corresponding to the beginning of next year.
	 * \param  inFromValue The clock value from which to compute the beginning of next year.
	 * \return The clock unit corresponding to the beginning of next year.
	 * \throw  SCHNAPS::Core::RunTimeException if these units are mroe specific than the simulation units
	 */
	unsigned long getNextYear(unsigned long inFromValue) const {
		schnaps_StackTraceBeginM();
		switch (mUnits) {
			case eYear:
				return inFromValue + 1;
			case eMonth:
				return (inFromValue/12 + 1) * 12;
			case eWeek:
				return (inFromValue/52 + 1) * 52;
			case eDay:
				return (inFromValue/365 + 1) * 365;
			default:
				throw schnaps_RunTimeExceptionM("Cannot get next year: these units are more specific than the simulation units!");
		}
		schnaps_StackTraceEndM("unsigned long SCHNAPS::Simulation::Clock::getNextYear() const");
	}
	
	/*!
	 * \brief  Get the clock unit corresponding to the beginning of next month.
	 * \param  inFromValue The clock value from which to compute the beginning of next month.
	 * \return The clock unit corresponding to the beginning of next month.
	 * \throw  SCHNAPS::Core::RunTimeException if these units are mroe specific than the simulation units
	 */
	unsigned long getNextMonth(unsigned long inFromValue) const {
		schnaps_StackTraceBeginM();
		switch (mUnits) {
			case eMonth:
				return inFromValue + 1;
			case eDay: {
				unsigned int lDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
				unsigned int lSumDays = 0;
				unsigned int lYearDay = inFromValue % 365;
				for (unsigned int i = 0; i < 12; i++) {
					if (lSumDays > lYearDay) {
						break;
					}
					lSumDays += lDays[i];
				}
				return lSumDays - lYearDay;}
			default:
				throw schnaps_RunTimeExceptionM("Cannot get next month: these units are more specific than the simulation units!");
		}
		schnaps_StackTraceEndM("unsigned long SCHNAPS::Simulation::Clock::getNextMonth() const");
	}
	
	/*!
	 * \brief  Return the clock tick (in clock units) corresponding to a value in specific units.
	 * \param  inValue The value (in specified units).
	 * \return The clock tick (in clock units) corresponding to a value in specific units.
	 * \throw  SCHNAPS::Core::RunTimeException if trying to get clock tick in units more specific than the simulation units.
	 */
	unsigned long getTick(unsigned int inValue, Clock::Units inUnits) const {
		schnaps_StackTraceBeginM();
		switch (inUnits) {
			case eYear:
				switch (mUnits) {
					case eYear:
						return inValue;
					case eMonth:
						return inValue * 12;
					case eWeek:
                                                return inValue * 52;
					case eDay:
						return inValue * 365;
					default:
						throw schnaps_RunTimeExceptionM("Cannot get clock tick in units more specific than the simulation units!");
				}
				break;
			case eMonth:
				switch (mUnits) {
					case eMonth:
						return inValue;
					case eDay: {
						unsigned int lDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
						unsigned int lYear = inValue / 12;
						unsigned int lMonth = inValue % 12;
						unsigned int lTick = lYear * 365;
						for (unsigned int i = 0; i < lMonth; i++) {							
							lTick += lDays[i];
						}
						return lTick;}
					default:
						throw schnaps_RunTimeExceptionM("Cannot get clock tick in units more specific than the simulation units!");
				}
				break;
			case eWeek:
				switch (mUnits) {
                                        case eWeek:
                                                return inValue;
					case eDay:
						return inValue * 7;
                                        default:
                                                throw schnaps_RunTimeExceptionM("Cannot get clock tick in units more specific than the simulation units!");
                                }
                                break;
			case eDay:
				switch (mUnits) {
					case eDay:
						return inValue;
					default:
						throw schnaps_RunTimeExceptionM("Cannot get clock tick in units more specific than the simulation units!");
				}
				break;
			default:
				// other
				return inValue;
		}
		schnaps_StackTraceEndM("unsigned long SCHNAPS::Simulation::Clock::getTick(unsigned int, unsigned int, SCHNAPS::Simulation::Clock::Units) const");
	}

	/*!
	 * \brief  Return a const handle to the stop condition tree.
	 * \return A const handle to the stop condition tree.
	 */
	const Core::PrimitiveTree::Handle getStop() const {
		return mStop;
	}

	/*!
	 * \brief  Return a const reference to the clock units.
	 * \return A const reference to the clock units.
	 */
	const Units& getUnits() const {
		return mUnits;
	}

private:
	unsigned long mValue; 				//!< The current clock value.
	Core::PrimitiveTree::Handle mStop;	//!< A handle to the stop condition tree.
	Units mUnits;						//!< The units of clock ticks.
	
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_Clock_hpp */
