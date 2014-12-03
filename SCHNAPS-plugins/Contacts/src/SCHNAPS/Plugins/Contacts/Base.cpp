/*
 * Base.cpp
 *
 * SCHNAPS
 * Copyright (C) 2012 by Xavier Douville
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

#include "SCHNAPS/Plugins/Contacts/Contacts.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Contacts;


/*!
 * \brief  Generate contacts.
 * \param  inPop Handle to the individuals to generate contacts for
 * \param  inSystem Handle to the system
 * \param  ioList List of contacts
 */
void Base::generate(Simulation::Individual::Bag::Handle inPop,Core::System::Handle inSystem,std::vector<Core::Vector::Handle>& ioList) const {
	schnaps_StackTraceBeginM();
	Core::Vector::Handle lNbContactsVect;
	if (inSystem->getParameters().hasParameter(NBCONTACTS_VARIABLE)) {
		lNbContactsVect = Core::castHandleT<Core::Vector>(inSystem->getParameters().getParameterHandle(NBCONTACTS_VARIABLE));
	} else {
		lNbContactsVect = NULL;
	}
	unsigned int lNbIndividuals=inPop->size();
	std::vector<unsigned int> lListNbContacts(lNbIndividuals);
	for (unsigned int i = 0; i < lNbIndividuals; i++) { //loop over all individuals to get their number of contacts
		ioList[i]=new Core::Vector();
		std::stringstream lSstm;
		unsigned int lAgeGroup;
		try {
			lAgeGroup = Core::castObjectT<const Core::UInt&>((*inPop)[i]->getState().getVariable(AGE_GROUP_VARIABLE)).getValue();
			// there are two ways to specify the number of contacts for each age group
			// 1) in a vector parameter
			// 2) have one parameter for each age group
			// vectors are cleaner but both ways are supported in this code
			if (lNbContactsVect != NULL) {
				if (lNbContactsVect->size() <= lAgeGroup) {
					throw schnaps_RunTimeExceptionM("Age group error");
				}
				lListNbContacts[i] = Core::castHandleT<Core::UInt>((*lNbContactsVect)[lAgeGroup])->getValue();
			} else {
				lSstm << NBCONTACTS_VARIABLE << lAgeGroup;
				lListNbContacts[i] = Core::castObjectT<const Core::UInt&>(inSystem->getParameters().getParameter(lSstm.str())).getValue();
			}
		} catch (Core::RunTimeException) {
			lSstm << " no number of contacts for this age group : " << lAgeGroup << ". Make sure you also have a variable " << AGE_GROUP_VARIABLE;
			throw schnaps_RunTimeExceptionM(lSstm.str());
		}
		if (lListNbContacts[i] >= lNbIndividuals) {
			throw schnaps_RunTimeExceptionM("Number of contacts must be lower than the number of individuals!");
		}
	}
	for (unsigned int i = 0; i < lNbIndividuals; i++) { //loop over all individuals to generate their contacts
		unsigned int lExtra = i+1 < lNbIndividuals ? 0 : 1;
		// it is not always possible to arrive to the good number of contacts for each individual
		// we will sometimes need to tolerate an extra
		for (unsigned int j = ioList[i]->size(); j < lListNbContacts[i]; j++){ //loop over all contacts to be generated
			unsigned int lIndividual = inSystem->getRandomizer(0).rollInteger(lExtra == 0 ? i+1 : 0, lNbIndividuals-1);
			for (unsigned int lCount=1;;lCount++){ //loop until a contact is found
				if (ioList[lIndividual]->size() < lListNbContacts[lIndividual]+lExtra) {
					if (i != lIndividual) {
						bool lValid = true;
						for (unsigned int k = 0; k < j; k++) { //loop over all contacts already generated to check if they are already in the list
							if (Core::castHandleT<Core::UInt>((*(ioList[i]))[k])->getValue() == lIndividual) {
								lValid = false; //Can't have the same contact twice
								break;
							}
						}
						if (lValid) {
							//valid contact found
							break;
						}
					}
				}
				
				if (lCount >= lNbIndividuals) {
					lCount = 0;
					// we tried all individuals and none were available
					lExtra++; // we will tolerate an extra contact
					// we redraw to be fair
					lIndividual = inSystem->getRandomizer(0).rollInteger(0, lNbIndividuals-1);
				} else {
					// we take the next individual, making sure we don't overflow
					if (++lIndividual >= lNbIndividuals) {
						lIndividual = 0;
					}
				}
			}
			// add to both contact lists
			ioList[i]->push_back(new Core::UInt(lIndividual));
			ioList[lIndividual]->push_back(new Core::UInt(i));
		}
	}
	schnaps_StackTraceEndM("void SCHNAPS::Plugins::Contacts::Base::generate(Simulation::Individual::Bag::Handle inPop) const");
}


