/*
 * ExecutionContext.hpp
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

#ifndef SCHNAPS_Simulation_ExecutionContext_hpp
#define SCHNAPS_Simulation_ExecutionContext_hpp

#include "SCHNAPS/Core/ExecutionContext.hpp"
#include "SCHNAPS/Simulation/Clock.hpp"
#include "SCHNAPS/Simulation/Environment.hpp"
#include "SCHNAPS/Simulation/Individual.hpp"

namespace SCHNAPS {
namespace Simulation {

/*!
 *  \class ExecutionContext SCHNAPS/Simulation/ExecutionContext.hpp "SCHNAPS/Simulation/ExecutionContext.hpp"
 *  \brief ExecutionContext class, execution context for simulator.
 */
class ExecutionContext: public Core::ExecutionContext {
public:
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	typedef std::hash_map<std::string, Core::AnyType::Handle, Core::HashString> LocalVariablesMap;
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	typedef __gnu_cxx::hash_map<std::string, Core::AnyType::Handle, Core::HashString> LocalVariablesMap;
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	typedef stdext::hash_map<std::string, Core::AnyType::Handle, Core::HashString> LocalVariablesMap;
#else // no hash_map found
	typedef std::map<std::string, Core::AnyType::Handle> LocalVariablesMap;
#endif

	//! ExecutionContext allocator type.
	typedef Core::AllocatorT<ExecutionContext, Core::ExecutionContext::Alloc> Alloc;
	//! ExecutionContext handle type.
	typedef Core::PointerT<ExecutionContext, Core::ExecutionContext::Handle> Handle;
	//! ExecutionContext bag type.
	typedef Core::ContainerT<ExecutionContext, Core::ExecutionContext::Bag> Bag;

	ExecutionContext();
	ExecutionContext(const ExecutionContext& inOriginal);
	explicit ExecutionContext(const Core::System::Handle inSystem, const Clock::Handle inClock, const Environment::Handle inEnvironment);
	virtual ~ExecutionContext() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("ExecutionContext");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Simulation::ExecutionContext::getName() const");
	}

	/*!
	 * \brief Set the clock.
	 * \param inClock A handle to the clock.
	 */
	void setClock(Clock::Handle inClock) {
		mClock = inClock;
	}

	/*!
	 * \brief Set the environment.
	 * \param inEnvironment A handle to the environment.
	 */
	void setEnvironment(Environment::Handle inEnvironment) {
		mEnvironment = inEnvironment;
	}

	/*!
	 * \brief Set the current individual.
	 * \param inIndividual A handle to the current individual.
	 */
	void setIndividual(Individual::Handle inIndividual) {
		mIndividual = inIndividual;
	}

	/*!
	 * \brief Set the current individual by index.
	 * \param inIndex The index of the current individual.
	 */
	void setIndividualByIndex(unsigned int inIndex) {
		mIndividual = mEnvironment->getPopulation()[inIndex];
	}

	/*!
	 * \brief  Return a const reference to the clock.
	 * \throw  SCHNAPS::AssertException if the clock is null.
	 * \return A const reference to the clock.
	 */
	const Clock& getClock() const {
		schnaps_NonNullPointerAssertM(mClock);
		return *mClock;
	}

	/*!
	 * \brief  Return a const reference to the environment.
	 * \throw  SCHNAPS::AssertException if the environment is null.
	 * \return A const reference to the environment.
	 */
	const Environment& getEnvironment() const {
		schnaps_NonNullPointerAssertM(mEnvironment);
		return *mEnvironment;
	}

	/*!
	 * \brief  Return a const handle to the environment.
	 * \return A const handle to the environment.
	 */
	const Environment::Handle getEnvironmentHandle() const {
		return mEnvironment;
	}

	/*!
	 * \brief  Return a const reference to the current individual.
	 * \throw  SCHNAPS::AssertException if the current individual is null.
	 * \return A const reference to the current individual.
	 */
	const Individual& getIndividual() const {
		schnaps_NonNullPointerAssertM(mIndividual);
		return *mIndividual;
	}

	/*!
	 * \brief  Return a reference to the current individual.
	 * \throw  SCHNAPS::AssertException if the current individual is null.
	 * \return A reference to the current individual.
	 */
	Individual& getIndividual() {
		schnaps_NonNullPointerAssertM(mIndividual);
		return *mIndividual;
	}
	
	/*!
	 * \brief  Return a const handle to the current individual.
	 * \return A const handle to the current individual.
	 */
	const Individual::Handle getIndividualHandle() const {
		return mIndividual;
	}
	
	//! Erase all local variables.
	void clearLocalVariables();
	//! Insert a local variable.
	void insertLocalVariable(const std::string& inLabel, Core::AnyType::Handle inValue);
	
	/*!
	 * \brief Set a local variable.
	 * \param inLabel A const reference to the label of the local variable.
	 * \param inValue A handle to the new value of the local variable.
	 * \throw SCHNAPS::Core::RunTimeException if the local variable does not exist.
	 */
	void setLocalVariable(const std::string& inLabel, Core::AnyType::Handle inValue) {
		schnaps_StackTraceBeginM();
		LocalVariablesMap::iterator lIter = mLocalVariables.find(inLabel);
		if (lIter == mLocalVariables.end()) {
			std::ostringstream lOSS;
			lOSS << "The local variable '" << inLabel << "' does not exists; ";
			lOSS << "could not set it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		lIter->second = inValue;
		schnaps_StackTraceEndM("void SCHNAPS::Simulation::ExecutionContext::setLocalVariable(const std::string&, SCHNAPS::Core::Atom::Handle) const");
	}
	
	/*!
	 * \brief  Return a copy of the local variables container.
	 * \return A copy of the local variables container.
	 */
	LocalVariablesMap getLocalVariables() const {
		schnaps_StackTraceBeginM();
		return mLocalVariables;
		schnaps_StackTraceEndM("LocalVariablesMap SCHNAPS::Simulation::ExecutionContext::getLocalVariables() const");
	}
	
	/*!
	 * \brief  Return a const reference to the value of a local variable.
	 * \param  inLabel A const reference to the label of the local variable.
	 * \return A const reference to the value of the local variable.
	 * \throw  SCHNAPS::Core::RunTimeException if the local variable does not exist.
	 * \throw  SCHNAPS::Core::AssertException if the local variable is NULL.
	 */
	const Core::AnyType& getLocalVariable(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		LocalVariablesMap::const_iterator lIterVariables = mLocalVariables.find(inLabel);
		if (lIterVariables == mLocalVariables.end()) {
			std::ostringstream lOSS;
			lOSS << "The local variable '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		schnaps_NonNullPointerAssertM(lIterVariables->second);
		return *lIterVariables->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::AnyType& SCHNAPS::Simulation::ExecutionContext::getLocalVariable(const std::string&) const");
	}
	
	/*!
	 * \brief  Return a const handle to the value of a local variable.
	 * \param  inLabel A const referenceto the label of the local variable.
	 * \return A const handle to the value of the local variable.
	 * \throw  SCHNAPS::Core::RunTimeException if the local variable does not exist.
	 */
	const Core::AnyType::Handle getLocalVariableHandle(const std::string& inLabel) const {
		schnaps_StackTraceBeginM();
		LocalVariablesMap::const_iterator lIterVariables = mLocalVariables.find(inLabel);
		if (lIterVariables == mLocalVariables.end()) {
			std::ostringstream lOSS;
			lOSS << "The local variable '" << inLabel << "' does not exist; ";
			lOSS << "could not get it.";
			throw schnaps_RunTimeExceptionM(lOSS.str());
		}
		return lIterVariables->second;
		schnaps_StackTraceEndM("const SCHNAPS::Core::AnyType::Handle SCHNAPS::Simulation::ExecutionContext::getVariableHandle(const std::string&) const");
	}

protected:
	// reference structures
	Clock::Handle mClock;				//!< A handle to the clock used for simulation.
	Environment::Handle mEnvironment;	//!< A handle to the environment.

	// current structures
	Individual::Handle mIndividual;		//!< A handle to the current individual (or environment) processing.
	LocalVariablesMap mLocalVariables;	//!< Current local variables in the executing process.
};
} // end of Simulation namespace
} // end of SCHNAPS namespace

#endif /* SCHNAPS_Simulation_ExecutionContext_hpp */
