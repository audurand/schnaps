/*
 * PopulationManager.hpp
 *
 *  Created on: 2010-04-15
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

#ifndef simulator_PopulationManager_hpp
#define simulator_PopulationManager_hpp

#include <map>

#include "SCHNAPS/Core/Object.hpp"
#include "SCHNAPS/Simulator/Generator.hpp"
#include "SCHNAPS/Simulator/Individual.hpp"

namespace simulator {

struct Source {
	std::string mProfile;
	unsigned int mSize;

	Source() {}
	explicit Source(std::string inProfile, unsigned int inSize) :
		mProfile(inProfile),
		mSize(inSize)
	{}
};

/*!
 *  \class PopulationManager PopulationManager/include/PopulationManager.hpp "PopulationManager/include/PopulationManager.hpp"
 *  \brief PopulationManager class. Maps a clock time to a source (profile and size).
 */
class PopulationManager: public core::Object, public std::multimap<unsigned long, Source> {
public:
	//! PopulationManager allocator type.
	typedef core::AllocatorT<PopulationManager, core::Object::Alloc> Alloc;
	//! PopulationManager handle type.
	typedef core::PointerT<PopulationManager, core::Object::Handle> Handle;
	//! PopulationManager bag type.
	typedef core::ContainerT<PopulationManager, core::Object::Bag> Bag;

	PopulationManager();
	explicit PopulationManager(core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment);
	virtual ~PopulationManager() {}

	virtual const std::string& getName() const {
		schnaps_StackTraceBeginM();
			const static std::string lName("PopulationManager");
			return lName;
		schnaps_StackTraceEndM("const std::string& PopulationManager::getName() const");
	}

	virtual void readWithSystem(PACC::XML::ConstIterator inIter, core::System& ioSystem);
	virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent = true) const;

	void readStr(const std::string& inStr) {
		std::stringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters(",", "");

		std::string lInput;

		while (lTokenizer.getNextToken(lInput)) {
			readEntryStr(lInput);
		}
	}

	std::string writeStr() const {
		std::stringstream lSS;
		std::string lInput;

		for (PopulationManager::const_iterator lIt = this->begin(); lIt != this->end(); lIt++) {
			// Write time
			lSS << "time" << "=" << lIt->first << ";";

			// Write profile name
			lSS << "profile" << "=" << lIt->second.mProfile.c_str() << ";";

			// Write size
			lSS << "size" << "=" << lIt->second.mSize << ",";
		}

		lInput = lSS.str();
		lInput.erase(lInput.end()-1);
		return lInput;
	}

	Individual::Bag::Handle getIndividuals();

	const std::map<std::string, Source>& getPrefixes() const {
		return mPrefixes;
	}

	std::map<std::string, Source>& getPrefixes() {
		return mPrefixes;
	}

	const Generator& getGenerator() const {
		schnaps_NonNullPointerAssertM(mGenerator);
		return *mGenerator;
	}

	Generator& getGenerator() {
		schnaps_NonNullPointerAssertM(mGenerator);
		return *mGenerator;
	}

private:
	void readEntryStr(const std::string& inStr) {
		std::stringstream lISS(inStr);
		PACC::Tokenizer lTokenizer(lISS);
		lTokenizer.setDelimiters(";", "");

		std::string lParam;
		std::string::size_type lPos;

		unsigned long lTime;
		std::string lProfile;
		unsigned int lSize;

		// Reading time
		if (lTokenizer.getNextToken(lParam)) {
			lPos = lParam.find("=");

			if (lPos == std::string::npos) {
				printf("Expected value of input parameter %s!\n", lParam.c_str());
			} else {
				lTime = core::str2uint(lParam.substr(lPos+1));

				// Reading profile name
				if (lTokenizer.getNextToken(lParam)) {
					lPos = lParam.find("=");

					if (lPos == std::string::npos) {
						printf("Expected value of input parameter %s!\n", lParam.c_str());
					} else {
						lProfile = lParam.substr(lPos+1);

						// Reading size
						if (lTokenizer.getNextToken(lParam)) {
							lPos = lParam.find("=");

							if (lPos == std::string::npos) {
								printf("Expected value of input parameter %s!\n", lParam.c_str());
							} else {
								lSize = core::str2uint(lParam.substr(lPos+1));

								this->insert(std::pair<unsigned long, Source>(lTime, Source(lProfile, lSize)));
							}
						}
					}
				}
			}
		}
	}

private:
	std::map<std::string, Source> mPrefixes;	//!< Individual IDs prefixes to sources.
	Generator::Handle mGenerator;				//!< Individual generator.
};
} // end of simulator namespace

#endif /* simulator_PopulationManager_hpp */
