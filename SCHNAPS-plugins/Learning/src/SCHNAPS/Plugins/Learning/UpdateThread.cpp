/*
 * UpdateThread.cpp
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

#include "SCHNAPS/Plugins/Learning/Learning.hpp"

using namespace SCHNAPS;
using namespace Plugins;
using namespace Learning;

UpdateThread::UpdateThread(PACC::Threading::Condition* inParallel,
							PACC::Threading::Semaphore* inSequential,
							const std::string& inFileName) :
	mParallel(inParallel),
	mSequential(inSequential),
	mFileName(inFileName.c_str())
{
	run();
}

UpdateThread::~UpdateThread() {	
	wait();
}

void UpdateThread::main() {
	bool lDone = false;

	mParallel->lock();
	mSequential->post();
	mParallel->wait();
	mParallel->unlock();

	do {
		switch (mPosition) {
		case eREADANDUPDATE:
		{
			std::ifstream* mIFS = new std::ifstream(mFileName.c_str(), std::ifstream::in);
			PACC::Tokenizer lTokenizer(*mIFS);
			lTokenizer.setDelimiters(",\n", "");
			
			std::string lDecisionNode, lState, lActionIndex, lIndividualIndex;
			unsigned int lActionID, lIndividualID;
			
			double lReward;
			
			while (lTokenizer.getNextToken(lDecisionNode)) {
				lTokenizer.getNextToken(lState);
				lTokenizer.getNextToken(lActionIndex);
				lTokenizer.getNextToken(lIndividualIndex);
				
				lActionID = SCHNAPS::str2uint(lActionIndex);
				lIndividualID = SCHNAPS::str2uint(lIndividualIndex);
				
				lReward = mDecisionMaker->computeReward(lDecisionNode, lState, lActionID, lIndividualID);
				
				// store data in log list
				mLogList.push_back(Log(lDecisionNode, lState, lActionID, lReward));
				
				// update the current decision maker
				mDecisionMaker->update(lDecisionNode, lState, lActionID, lReward);
			}
			
			mIFS->close();
			delete mIFS;
			
			mPosition = eUPDATE;

			mParallel->lock();
			mSequential->post();
			mParallel->wait();
			mParallel->unlock();
			break;
		}
		case eUPDATE:
		{
			for (std::list<Log>::const_iterator lIt = mLogList.begin(); lIt != mLogList.end(); lIt++) {
				// update the current decision maker
				mDecisionMaker->update(lIt->mDecisionNode, lIt->mState, lIt->mActionID, lIt->mReward);
			}

			mParallel->lock();
			mSequential->post();
			mParallel->wait();
			mParallel->unlock();
			break;
		}
		default: // eEND
			lDone = true;
			break;
		}
	} while (!lDone);
}
