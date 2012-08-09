/*
 * Generator.cpp
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
#include "SCHNAPS/Simulation.hpp"

#include <vector>

using namespace SCHNAPS;
using namespace Simulation;

/*!
 * \brief Default constructor.
 */
Generator::Generator() :
	mSystem(NULL),
	mClock(NULL),
	mEnvironment(NULL),
	mParallel(new PACC::Threading::Condition()),
	mSequential(new PACC::Threading::Semaphore(0))
{}

/*!
 * \brief Construct a generator as a copy of an original.
 * \param inOriginal A const reference to the original.
 */
Generator::Generator(const Generator& inOriginal) :
	mSystem(inOriginal.mSystem),
	mClock(inOriginal.mClock),
	mEnvironment(inOriginal.mEnvironment),
	mRandomizerInitSeed(inOriginal.mRandomizerInitSeed),
	mRandomizerInitState(inOriginal.mRandomizerInitState),
	mRandomizerCurrentSeed(inOriginal.mRandomizerCurrentSeed),
	mRandomizerCurrentState(inOriginal.mRandomizerCurrentState),
	mContext(inOriginal.mContext),
	mSubThreads(inOriginal.mSubThreads),
	mParallel(inOriginal.mParallel),
	mSequential(inOriginal.mSequential),
	mProfiles(inOriginal.mProfiles)
{}

/*!
 * \brief Construct a generator with specific system, clock and environment.
 * \param inSystem A handle to the system.
 * \param inClock A handle to the clock.
 * \param inEnvironment A handle to the environment.
 */
Generator::Generator(Core::System::Handle inSystem, Clock::Handle inClock, Environment::Handle inEnvironment) :
	mSystem(inSystem),
	mClock(inClock),
	mEnvironment(inEnvironment),
	mParallel(new PACC::Threading::Condition()),
	mSequential(new PACC::Threading::Semaphore(0))
{
	mContext.push_back(new GenerationContext(inSystem, inClock, inEnvironment));
	mContext.back()->setThreadNb(0);
}

/*!
 * \brief Destructor.
 */

Generator::~Generator()
{
	delete mParallel;
	delete mSequential;
}


/*!
 * \brief Read object from XML.
 * \param inIter XML iterator of input document.
 * \throw SCHNAPS::Core::IOException if a wrong tag is encountered.
 */
void Generator::read(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != getName()) {
		std::ostringstream lOSS;
		lOSS << "tag <" + getName() + "> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

#ifdef SCHNAPS_FULL_DEBUG
	printf("Reading generator\n");
#endif

	inIter = inIter->getFirstChild();

	refresh();

	// read and set seed for each randomizer
	readRandomizerInfo(inIter++);

	// read population profiles
	readProfiles(inIter);
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::read(PACC::XML::ConstIterator)");
}

/*!
 * \brief Write object content to XML.
 * \param ioStreamer XML streamer to output document.
 * \param inIndent Wether to indent or not.
 */
void Generator::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	// write the seeds of randomizers
	this->writeRandomizerInfo(ioStreamer, inIndent);

	// write profiles
	this->writeProfiles(ioStreamer, inIndent);
}

/*!
 * \brief  Generate a quantity of individuals from a specific profile using specific prefix and start index.
 * \param  inProfile A const reference to the profile name.
 * \param  inSize The size of sub-population to generate.
 * \param  inPrefix A const reference to the ID prefix of new individuals.
 * \param  inStartingIndex The start index of individuals.
 * \return A handle to a bag of newly generated individuals.
 * \throw  SCHNAPS::Core::RunTimeException if specific profile does not exist.
 */
Individual::Bag::Handle Generator::generate(const std::string& inProfile, unsigned int inSize, const std::string& inPrefix, unsigned int inStartingIndex) {
	schnaps_StackTraceBeginM();
	ProfileMap::iterator lIterProfiles = mProfiles.find(inProfile);
	if (lIterProfiles == mProfiles.end()) {
		std::ostringstream lOSS;
		lOSS << "The profile '" << inProfile << "' does not exist; ";
		lOSS << "could not generate individuals.";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}

	GenProfile::Handle lProfile = lIterProfiles->second;

	// compute variables to erase
	Core::StringArray::Handle lEraseVariable = new Core::StringArray();
	for (unsigned int i = 0; i < lProfile->getDemography().getVariablesSize(); i++) {
		if (lProfile->getIndividualModel().find(lProfile->getDemography().getVariable(i).mLabel) == lProfile->getIndividualModel().end()) {
			// if variable not in individual model, add to variable list for thrash
			lEraseVariable->push_back(lProfile->getDemography().getVariable(i).mLabel);
		}
	}

	// backup randomizer seeds
	Core::ULongArray lBackupSeed;
	Core::StringArray lBackupState;

	// for computing threads sub-size to generate
	unsigned int lQuotient = inSize / mContext.size();
	unsigned int lRemainder = inSize % mContext.size();
	unsigned int lSubSize;

	// create subthreads
	for (unsigned int i = 0; i < mContext.size(); i++) {
		mContext[i]->setGenProfile(Core::castHandleT<GenProfile>(lProfile->deepCopy(*mSystem)));
		
		// create subthread
		mSubThreads.push_back(new GenerationThread(mParallel, mSequential, mContext[i]));
		mSequential->wait();

		// compute sub-size
		lSubSize = lQuotient;
		if (i < lRemainder) {
			lSubSize++;
		}

		mSubThreads.back()->setGenerationInfo(lSubSize, inPrefix, inStartingIndex, lEraseVariable);
		inStartingIndex += lSubSize;

		// backup and reset randomizer info
		lBackupSeed.push_back(mSystem->getRandomizer(i).getSeed());
		lBackupState.push_back(mSystem->getRandomizer(i).getState());
		mSystem->getRandomizer(i).reset(mRandomizerCurrentSeed[i], mRandomizerCurrentState[i]);
	}

	// build individuals
#ifdef SCHNAPS_FULL_DEBUG
		std::cout << "Generating " << inSize <<  " individuals\n";
#endif
	// launch subthreads and wait
	mParallel->lock();
	mParallel->broadcast();
	mParallel->unlock();
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		mSequential->wait();
	}

	// store result for output
	Individual::Bag::Handle lIndividuals = new Individual::Bag();
	lIndividuals->reserve(inSize);
	for (unsigned int i = 0; i < mSubThreads.size(); i++) {
		lIndividuals->insert(lIndividuals->end(), mSubThreads[i]->getIndividuals().begin(), mSubThreads[i]->getIndividuals().end());

		// reset old randomizer info
		mRandomizerCurrentSeed[i] = mSystem->getRandomizer(i).getSeed();
		mRandomizerCurrentState[i] = mSystem->getRandomizer(i).getState();
		mSystem->getRandomizer(i).reset(lBackupSeed[i], lBackupState[i]);
	}
	
	// terminate and destroy threads
	mSubThreads.clear();

	return lIndividuals;
	schnaps_StackTraceEndM("SCHNAPS::Simulation::Individual::Bag::Handle SCHNAPS::Simulation::Generator::generate(const std::string&, unsigned int, const std::string&, unsigned int)");
}

/*!
 * \brief Build individuals using specific thread.
 * \param inThread A handle to the executing thread.
 */
void Generator::buildIndividuals(GenerationThread::Handle inThread) {
	schnaps_StackTraceBeginM();
	GenerationContext::Handle lContext = inThread->getContextHandle();
	std::stringstream lID;
	unsigned int lIndividualIndex = inThread->getStartingIndex();


	inThread->getIndividuals().clear();
	inThread->getIndividuals().reserve(inThread->getSize());

	for (unsigned int i = 0; i < inThread->getSize(); i++) {
		lID.str("");
		lID << inThread->getPrefix() << "/" << lIndividualIndex++;

		inThread->getIndividuals().push_back(new Individual(lID.str()));
		lContext->setIndividual(inThread->getIndividuals().back());



		// add demography variables
		do {
			lContext->getIndividual().getState().clear();
			
			for (unsigned int j = 0; j < lContext->getGenProfile().getDemography().getVariablesSize(); j++) {
				// set local variables
				for (unsigned int k = 0; k < lContext->getGenProfile().getDemography().getVariable(j).mLocalVariables.size(); k++) {
					lContext->insertLocalVariable(
						lContext->getGenProfile().getDemography().getVariable(j).mLocalVariables[k].first,
						Core::castHandleT<Core::AnyType>(lContext->getGenProfile().getDemography().getVariable(j).mLocalVariables[k].second->clone()));
				}
				
				// compute variable init value
				lContext->getIndividual().getState().insertVariable(
					lContext->getGenProfile().getDemography().getVariable(j).mLabel,
					lContext->getGenProfile().getDemography().getVariable(j).mInitTree->interpret(*lContext));
				
				// clear local variables
				lContext->clearLocalVariables();
			}
			// retry until a valid individual is created
		} while (Core::castHandleT<Core::Bool>(lContext->getGenProfile().getAcceptFunction().interpret(*lContext))->getValue() == false);

		// add simulation variables
		for (unsigned int j = 0; j < lContext->getGenProfile().getSimulationVariables().getVariablesSize(); j++) {
			// set local variables
			for (unsigned int k = 0; k < lContext->getGenProfile().getSimulationVariables().getVariable(j).mLocalVariables.size(); k++) {
				lContext->insertLocalVariable(
					lContext->getGenProfile().getSimulationVariables().getVariable(j).mLocalVariables[k].first,
					Core::castHandleT<Core::AnyType>(lContext->getGenProfile().getSimulationVariables().getVariable(j).mLocalVariables[k].second->clone()));
			}
			
			// compute variable init value
			lContext->getIndividual().getState().insertVariable(
				lContext->getGenProfile().getSimulationVariables().getVariable(j).mLabel,
				lContext->getGenProfile().getSimulationVariables().getVariable(j).mInitTree->interpret(*lContext));
			
			// clear local variables
			lContext->clearLocalVariables();
		}

		// erase non-wanted demographic variables
		for (unsigned int j = 0; j < inThread->getEraseVariables().size(); j++) {
			lContext->getIndividual().getState().removeVariable(inThread->getEraseVariables()[j]);
		}
	}
	
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::buildIndividuals(SCHNAPS::Simulation::GenerationThread::Handle)");
}

/*!
 * \brief Generate contacts for all individuals
 * \param inThread A handle to the executing thread.
 */
void Generator::generateContacts(Individual::Bag::Handle inPop){
	schnaps_StackTraceBeginM();
	Core::Vector::Handle lNbContactsVect;
	if(mSystem->getParameters().hasParameter(NBCONTACTS_VARIABLE)){
		lNbContactsVect = Core::castHandleT<Core::Vector>(mSystem->getParameters().getParameterHandle(NBCONTACTS_VARIABLE));
	}
	else{
		lNbContactsVect=NULL;
	}
	//Individual::Bag lPop=*inPop;
	unsigned int lNbIndividuals=inPop->size();
	std::vector<unsigned int> lListNbContacts(lNbIndividuals);
	std::vector<Core::Vector::Handle> lList(lNbIndividuals);
	for(unsigned int i=0; i<lNbIndividuals ; i++){ //loop over all individuals to get their number of contacts
		lList[i]=new Core::Vector();
		std::stringstream lSstm;
		unsigned int lAgeGroup;
		try{
			lAgeGroup = Core::castObjectT<const Core::UInt&>((*inPop)[i]->getState().getVariable(AGE_GROUP_VARIABLE)).getValue();
			//There are two ways to specify the number of contacts for each age group. The first one is in a vector parameter and the second one is to have one parameter for each age group.
			//Vectors are cleaner but both ways are supported in this code
			if(lNbContactsVect != NULL){
				if(lNbContactsVect->size() <= lAgeGroup){
					throw schnaps_RunTimeExceptionM("Age group error");
				}
				lListNbContacts[i]=Core::castHandleT<Core::UInt>((*lNbContactsVect)[lAgeGroup])->getValue();
			}
			else
			{
				lSstm << NBCONTACTS_VARIABLE << lAgeGroup;
				lListNbContacts[i]=Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter(lSstm.str())).getValue();
			}
		}
		catch(Core::RunTimeException) {
			lSstm << " no number of contacts for this age group : " << lAgeGroup << ". Make sure you also have a variable " << AGE_GROUP_VARIABLE;
			throw schnaps_RunTimeExceptionM(lSstm.str());
		}
		if(lListNbContacts[i] >= lNbIndividuals){
			throw schnaps_RunTimeExceptionM("Number of contacts must be lower than the number of individuals!");
		}
	}
	unsigned long lBackupSeed=mSystem->getRandomizer(0).getSeed();
	std::string lBackupState=mSystem->getRandomizer(0).getState();
	mSystem->getRandomizer(0).reset(mRandomizerCurrentSeed[0], mRandomizerCurrentState[0]);
	for (unsigned int i=0; i<lNbIndividuals ; i++) { //loop over all individuals to generate their contacts
		unsigned int lExtra = i+1<lNbIndividuals ? 0 : 1; //It is not always possible to arrive to the good number of contacts for each individual. We will sometimes need to tolerate an extra
		for (unsigned int j = lList[i]->size(); j < lListNbContacts[i]; j++){ //loop over all contacts to be generated
			unsigned int lIndividual = mSystem->getRandomizer(0).rollInteger(lExtra==0 ? i+1 : 0,lNbIndividuals-1);
			for(unsigned int lCount=1;;lCount++){ //loop until a contact is found
				if(lList[lIndividual]->size() < lListNbContacts[lIndividual]+lExtra){
					if(i!=lIndividual){
						bool lValid = true;
						for(unsigned int k=0;k<j;k++){ //loop over all contacts already generated to check if they are already in the list
							if(Core::castHandleT<Core::UInt>((*(lList[i]))[k])->getValue() == lIndividual){
								lValid = false; //Can't have the same contact twice
								break;
							}
						}
						if(lValid){
							//valid contact found
							break;
						}
					}
				}
				if(lCount>=lNbIndividuals){
					lCount=0;
					//We tried all individuals and none were available
					lExtra++; //We will tolerate an extra contact
					//We redraw to be fair
					lIndividual = mSystem->getRandomizer(0).rollInteger(0,lNbIndividuals-1);
				}
				else{
					//We take the next individual, making sure we don't overflow.
					if(++lIndividual>=lNbIndividuals){
						lIndividual=0;
					}
				}
			}
			//Add to both contact lists
			lList[i]->push_back(new Core::UInt(lIndividual));
			lList[lIndividual]->push_back(new Core::UInt(i));
		}
	}
	mRandomizerCurrentSeed[0] = mSystem->getRandomizer(0).getSeed();
	mRandomizerCurrentState[0] = mSystem->getRandomizer(0).getState();
	mSystem->getRandomizer(0).reset(lBackupSeed, lBackupState);
	for (unsigned int i=0; i<lNbIndividuals ; i++) { //loop over all individuals to finally add their contact lists to the simulation variables
		(*inPop)[i]->getState().insertVariable(CONTACTS_LIST_VARIABLE,lList[i]);
#ifdef SCHNAPS_DEBUG_CONTACTS

		std::cout << "individual " << i << " list of " << lList[i]->size() << "/" << lListNbContacts[i] << " contacts " << lList[i]->writeStr() << std::endl;
#endif
	}

	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::generateContacts(SCHNAPS::Simulation::GenerationThread::Handle)");
}

/*!
 * \brief Refresh simulator structure with up-to-date parameters.
 */
void Generator::refresh() {
	schnaps_StackTraceBeginM();
	// create one context per thread + sub threads
	unsigned int lNbThreads_new = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.generator")).getValue();
	if (lNbThreads_new < 1) {
		std::ostringstream lOSS;
		lOSS << "The number of generation threads must be higher or equal to 1 (current new value = " << lNbThreads_new << ");";
		lOSS << "the number of generation threads could not be set.\n";
		throw schnaps_RunTimeExceptionM(lOSS.str());
	}
	
	unsigned int lNbThreads_old = mContext.size();

	if (lNbThreads_old < lNbThreads_new) {
		for (unsigned int i = lNbThreads_old; i < lNbThreads_new; i++) {
			// create new contexts
			mContext.push_back(new GenerationContext(mSystem, mClock, mEnvironment));
			mContext.back()->setThreadNb(i);
			
			// add simulator randomizers information
			mRandomizerInitSeed.push_back(0);
			mRandomizerInitState.push_back("");
		}
	} else if (lNbThreads_new < lNbThreads_old) {
		for (unsigned int i = lNbThreads_old; i > lNbThreads_new; i--) {
			// remove unused contexts
			mContext.pop_back();
			
			// remove unused randomizers information
			mRandomizerInitSeed.pop_back();
			mRandomizerInitState.pop_back();
		}
	}
	
	// update generator randomizers information
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::refresh()");
}

void Generator::clearRandomizer() {
	schnaps_StackTraceBeginM();
	for (unsigned int i = 0; i < mRandomizerCurrentSeed.size(); i++) {
		mRandomizerCurrentSeed[i] = 0;
		mRandomizerCurrentState[i] = "";
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::clearRandomizer()");
}

void Generator::resetRandomizer() {
	schnaps_StackTraceBeginM();
	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::resetRandomizer()");
}

// private functions

void Generator::readRandomizerInfo(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "RandomizerInfo") {
		std::ostringstream lOSS;
		lOSS << "tag <RandomizerInfo> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

#ifdef SCHNAPS_FULL_DEBUG
printf("Reading randomizer info\n");
#endif

	unsigned int lThreadsGenerator = Core::castObjectT<const Core::UInt&>(mSystem->getParameters().getParameter("threads.generator")).getValue();
	mRandomizerInitSeed.clear();
	mRandomizerInitState.clear();
	mRandomizerInitSeed.resize(lThreadsGenerator, 0);
	mRandomizerInitState.resize(lThreadsGenerator, "");

	if ((inIter->getChildCount() != 0) && (inIter->getChildCount() < lThreadsGenerator)) {
		throw schnaps_IOExceptionNodeM(*inIter, "expected at least a randomizer per thread!");
	}

	unsigned int lThreadNumber = 0;
	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lThreadNumber<lThreadsGenerator; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			if (lChild->getValue() != "Randomizer") {
				std::ostringstream lOSS;
				lOSS << "tag <Randomizer> expected, but ";
				lOSS << "got tag <" << lChild->getValue() << "> instead!";
				throw schnaps_IOExceptionNodeM(*lChild, lOSS.str());
			}
			mRandomizerInitSeed[lThreadNumber] = static_cast<unsigned long>(SCHNAPS::str2ulonglong(lChild->getAttribute("seed")));
			mRandomizerInitState[lThreadNumber] = lChild->getAttribute("state");
			lThreadNumber++;
		}
	}

	mRandomizerCurrentSeed = mRandomizerInitSeed;
	mRandomizerCurrentState = mRandomizerInitState;
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::readRandomizerInfo(PACC::XML::ConstIterator)");
}

void Generator::readProfiles(PACC::XML::ConstIterator inIter) {
	schnaps_StackTraceBeginM();
	schnaps_NonNullPointerAssertM(mSystem);
	if (inIter->getType() != PACC::XML::eData) {
		throw schnaps_IOExceptionNodeM(*inIter, "tag expected!");
	}
	if (inIter->getValue() != "Profiles") {
		std::ostringstream lOSS;
		lOSS << "tag <Profiles> expected, but ";
		lOSS << "got tag <" << inIter->getValue() << "> instead!";
		throw schnaps_IOExceptionNodeM(*inIter, lOSS.str());
	}

#ifdef SCHNAPS_FULL_DEBUG
printf("Reading profiles\n");
#endif

	for (PACC::XML::ConstIterator lChild = inIter->getFirstChild(); lChild; lChild++) {
		if (lChild->getType() == PACC::XML::eData) {
			mProfiles.insert(std::pair<std::string, GenProfile::Handle>(lChild->getAttribute("label"), new GenProfile()));
			mProfiles[lChild->getAttribute("label")]->readWithSystem(lChild, *mSystem);
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::readProfile(PACC::XML::ConstIterator, SCHNAPS::Simulation::Profile&)");
}

void Generator::writeRandomizerInfo(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("RandomizerInfo");
	for (unsigned int i = 0; i < mRandomizerInitState.size(); i++) {
		ioStreamer.openTag("Randomizer");
		ioStreamer.insertAttribute("seed", mRandomizerInitSeed[i]);
		ioStreamer.insertAttribute("state", mRandomizerInitState[i]);
		ioStreamer.closeTag();
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::writeRandomizerInfo(PACC::XML::Streamer&, bool)");
}

void Generator::writeProfiles(PACC::XML::Streamer& ioStreamer, bool inIndent) const {
	schnaps_StackTraceBeginM();
	ioStreamer.openTag("Profiles");
	for (ProfileMap::const_iterator lIt = mProfiles.begin(); lIt != mProfiles.end(); lIt++) {
		lIt->second->write(ioStreamer, inIndent);
	}
	ioStreamer.closeTag();
	schnaps_StackTraceEndM("void SCHNAPS::Simulation::Generator::writeProfiles(PACC::XML::Streamer&, bool)");
}
