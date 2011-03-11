/*
 * System.hpp
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

#ifndef SCHNAPS_Core_System_hpp
#define SCHNAPS_Core_System_hpp

#include "SCHNAPS/Core/Component.hpp"
#include "SCHNAPS/Core/Factory.hpp"
#include "SCHNAPS/Core/LoggerMulti.hpp"
#include "SCHNAPS/Core/Parameters.hpp"
#include "SCHNAPS/Core/Plugins.hpp"
#include "SCHNAPS/Core/RandomizerMulti.hpp"
#include "SCHNAPS/Core/TypingManager.hpp"

#if defined(SCHNAPS_HAVE_STD_HASHMAP) | defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
#include <hash_map>
#include "SCHNAPS/Core/HashString.hpp"
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
#include <ext/hash_map>
#include "SCHNAPS/Core/HashString.hpp"
#else
#include <map>
#endif

namespace SCHNAPS {
namespace Core {

/*!
 *  \class System SCHNAPS/Core/System.hpp "SCHNAPS/Core/System.hpp"
 *  \brief System class.
 *
 *  A system is a container for the following basic components:
 *   - Factory
 *   - Parameters
 *   - Randomizer
 *   - Logger
 *   - Typing manager
 *   - Plugins
 */
#if defined(SCHNAPS_HAVE_STD_HASHMAP)
	class System: public Object, public std::hash_map<std::string, Component::Handle, HashString>
#elif defined(SCHNAPS_HAVE_GNUCXX_HASHMAP)
	class System: public Object, public __gnu_cxx::hash_map<std::string, Component::Handle, HashString>
#elif defined(SCHNAPS_HAVE_STDEXT_HASHMAP)
	class System: public Object, public stdext::hash_map<std::string, Component::Handle, HashString>
#else // no hash_map found
	class System: public Object, public std::map<std::string, Component::Handle>
#endif
{
public:
	//! System allocator type.
	typedef AllocatorT<System, Object::Alloc> Alloc;
	//! System handle type.
	typedef PointerT<System, Object::Handle> Handle;
	//! System bag type.
	typedef ContainerT<System, Object::Bag> Bag;

	System();
	virtual ~System() {}

	/*!
	 * \brief  Return a const reference to the name of object.
	 * \return A const reference to the name of object.
	 */
	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
		const static std::string lName("System");
		return lName;
		schnaps_StackTraceEndM("const std::string& SCHNAPS::Core::System::getName() const");
	}

	//! Read object from XML.
	virtual void read(PACC::XML::ConstIterator inIter);
	//! Write content of object to XML.
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	//! Test if object is equal to another.
	virtual bool isEqual(const Object& inRightObj) const;
	//! Test if object is less than another.
	virtual bool isLess(const Object& inRightObj) const;

	//! Add a new component to the system.
	void addComponent(Component::Handle inComponent);
	//! Initialize the components of this system.
	void initComponents();

	/*!
	 * \brief Return a const reference to the factory.
	 * \return A const reference to the factory.
	 * \throw SCHNAPS::Core::AssertException if factory is not instanciated. 
	 */
	const Factory& getFactory() const {
		schnaps_NonNullPointerAssertM(mFactory);
		return *mFactory;
	}

	/*!
	 * \brief Return a reference to the factory.
	 * \return A reference to the factory.
	 * \throw SCHNAPS::Core::AssertException if factory is not instanciated. 
	 */
	inline Factory& getFactory() {
		schnaps_NonNullPointerAssertM(mFactory);
		return *mFactory;
	}

	/*!
	 * \brief Return a const reference to the parameters.
	 * \return A const reference to the parameters.
	 * \throw SCHNAPS::Core::AssertException if parameters are not instanciated. 
	 */
	const Parameters& getParameters() const {
		schnaps_NonNullPointerAssertM(mParameters);
		return *mParameters;
	}

	/*!
	 * \brief Return a reference to the parameters.
	 * \return A reference to the parameters.
	 * \throw SCHNAPS::Core::AssertException if parameters are not instanciated. 
	 */
	Parameters& getParameters() {
		schnaps_NonNullPointerAssertM(mParameters);
		return *mParameters;
	}

	/*!
	 * \brief Return a const reference to the randomizer associated to a thread.
	 * \param inThreadNb The thread number of randomizer to return.
	 * \return A const reference to the randomizer.
	 * \throw SCHNAPS::Core::AssertException if randomizers are not instanciated. 
	 */
	const Randomizer& getRandomizer(unsigned int inThreadNb) const {
		schnaps_NonNullPointerAssertM(mRandomizers);
		return *((*mRandomizers)[inThreadNb]);
	}

	/*!
	 * \brief Return a reference to the randomizer.
	 * \param inThreadNb The thread number of randomizer to return.
	 * \return A reference to the randomizer.
	 * \throw SCHNAPS::Core::AssertException if randomizers are not instanciated. 
	 */
	Randomizer& getRandomizer(unsigned int inThreadNb) {
		schnaps_NonNullPointerAssertM(mRandomizers);
		return *((*mRandomizers)[inThreadNb]);
	}

	/*!
	 * \brief Return a const reference to the logger.
	 * \param inThreadNb The thread number of logger to return.
	 * \return A const reference to the logger.
	 * \throw SCHNAPS::Core::AssertException if loggers are not instanciated.
	 */
	const Logger& getLogger(unsigned int inThreadNb) const {
		schnaps_NonNullPointerAssertM(mLoggers);
		return *((*mLoggers)[inThreadNb]);
	}

	/*!
	 * \brief Return a reference to the logger.
	 * \param inThreadNb The thread number of logger to return.
	 * \return A reference to the logger.
	 * \throw SCHNAPS::Core::AssertException if loggers are not instanciated.
	 */
	Logger& getLogger(unsigned int inThreadNb) {
		schnaps_NonNullPointerAssertM(mLoggers);
		return *((*mLoggers)[inThreadNb]);
	}

	/*!
	 * \brief Return a const reference to the typing manager.
	 * \return A const reference to the typing manager.
	 * \throw SCHNAPS::Core::AssertException if typing manager is not instanciated. 
	 */
	const TypingManager& getTypingManager() const {
		schnaps_NonNullPointerAssertM(mTypingManager);
		return *mTypingManager;
	}

	/*!
	 * \brief Return a const reference to the plugins.
	 * \return A const reference to the plugins.
	 * \throw SCHNAPS::Core::AssertException if plugins are not instanciate.
	 */
	const Plugins& getPlugins() const {
		schnaps_NonNullPointerAssertM(mPlugins);
		return *mPlugins;
	}

	/*!
	 * \brief Return a const handle to a specific component.
	 * \param inName The name of the component to return.
	 * \return A const handle to the specific component.
	 * \throw SCHNAPS::Core::AssertException if specific component does not exist.
	 */
	const Component::Handle getComponentHandle(std::string inName) const {
		schnaps_AssertM(this->find(inName) != this->end());
		return this->find(inName)->second;
	}

	/*!
	 * \brief Return a const reference to a specific component.
	 * \param inName The name of the component to return.
	 * \return A const reference to the specific component.
	 * \throw SCHNAPS::Core::AssertException if specific component does not exist.
	 */
	const Component& getComponent(std::string inName) const {
		schnaps_AssertM(this->find(inName) != this->end());
		return *(this->find(inName)->second);
	}

	/*!
	 * \brief Return a reference to a specific component.
	 * \param inName The name of the component to return.
	 * \return A reference to the specific component.
	 * \throw SCHNAPS::Core::AssertException if specific component does not exist.
	 */
	Component& getComponent(std::string inName) {
		schnaps_AssertM(this->find(inName) != this->end());
		return *(this->find(inName)->second);
	}

private:
	Factory::Handle mFactory;				//!< Factory for allocating different objects.
	Parameters::Handle mParameters;			//!< Parameters of system.

	RandomizerMulti::Handle mRandomizers;	//!< Random number generators (one per thread).
	LoggerMulti::Handle mLoggers;			//!< Loggers (one per thread).

	TypingManager::Handle mTypingManager;	//!< Typing manager for types verification.
	Plugins::Handle mPlugins;				//!< Plugins (dynamically loaded at runtime.
};
} // end of Core namespace
} // end of SCHNAPS namespace

#endif // SCHNAPS_Core_System_hpp
