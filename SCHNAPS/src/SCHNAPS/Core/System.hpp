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

#ifndef core_System_hpp
#define core_System_hpp

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

namespace core {

/*!
 *  \class System SCHNAPS/Core/System.hpp "SCHNAPS/Core/System.hpp"
 *  \brief Core system.
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

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("System");
			return lName;
		schnaps_StackTraceEndM("const std::string& System::getName() const");
	}

	virtual void read(PACC::XML::ConstIterator inIter);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	virtual bool isEqual(const Object& inRightObj) const;
	virtual bool isLess(const Object& inRightObj) const;

	//! Add a new component to the system.
	void addComponent(Component::Handle inComponent);
	//! Initialize the components of this system.
	void initComponents();

	//! Return a const reference to the factory.
	inline const Factory& getFactory() const {
		schnaps_NonNullPointerAssertM(mFactory);
		return *mFactory;
	}

	//! Return a reference to the factory.
	inline Factory& getFactory() {
		schnaps_NonNullPointerAssertM(mFactory);
		return *mFactory;
	}

	//! Return a const reference to the parameters.
	inline const Parameters& getParameters() const {
		schnaps_NonNullPointerAssertM(mParameters);
		return *mParameters;
	}

	//! Return a reference to the parameters.
	inline Parameters& getParameters() {
		schnaps_NonNullPointerAssertM(mParameters);
		return *mParameters;
	}

	//! Return a const reference to the randomizer.
	inline const Randomizer& getRandomizer(unsigned int inThreadNb) const {
		schnaps_NonNullPointerAssertM(mRandomizers);
		return *((*mRandomizers)[inThreadNb]);
	}

	//! Return a reference to the randomizer.
	inline Randomizer& getRandomizer(unsigned int inThreadNb) {
		schnaps_NonNullPointerAssertM(mRandomizers);
		return *((*mRandomizers)[inThreadNb]);
	}

	//! Return a const reference to the logger.
	inline const Logger& getLogger(unsigned int inThreadNb) const {
		schnaps_NonNullPointerAssertM(mLoggers);
		return *((*mLoggers)[inThreadNb]);
	}

	//! Return a reference to the logger.
	inline Logger& getLogger(unsigned int inThreadNb) {
		schnaps_NonNullPointerAssertM(mLoggers);
		return *((*mLoggers)[inThreadNb]);
	}

	//! Return a const reference to the typing manager.
	inline const TypingManager& getTypingManager() const {
		schnaps_NonNullPointerAssertM(mTypingManager);
		return *mTypingManager;
	}

	//! Return a const reference to the plugins.
	inline const Plugins& getPlugins() const {
		schnaps_AssertM(mPlugins != NULL);
		return *mPlugins;
	}

	//! Return a const handle to the specified component.
	inline const Component::Handle getComponentHandle(std::string inName) const {
		schnaps_AssertM(this->find(inName) != this->end());
		return this->find(inName)->second;
	}

	//! Return a const reference to the specified component.
	inline const Component& getComponent(std::string inName) const {
		schnaps_AssertM(this->find(inName) != this->end());
		return *(this->find(inName)->second);
	}

	//! Return a reference to the specified component.
	inline Component& getComponent(std::string inName) {
		schnaps_AssertM(this->find(inName) != this->end());
		return *(this->find(inName)->second);
	}

private:
	Factory::Handle mFactory;				//!< Factory system component.
	Parameters::Handle mParameters;

	RandomizerMulti::Handle mRandomizers;	//!< Randomizer system component (one randomizer per thread).
	LoggerMulti::Handle mLoggers;			//!< Logger system component (one logger per thread).

	TypingManager::Handle mTypingManager;	//!< Typing manager system component.
	Plugins::Handle mPlugins;				//!< Plugins system component.
};
}

#endif // core_System_hpp
