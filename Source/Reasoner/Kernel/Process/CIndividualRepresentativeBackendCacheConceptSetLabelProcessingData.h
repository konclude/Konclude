/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREPRESENTATIVEBACKENDCACHECONCEPTSETLABELPROCESSINGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREPRESENTATIVEBACKENDCACHECONCEPTSETLABELPROCESSINGDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData {
					// public methods
					public:
						//! Constructor
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData();

						CXLinker<CIndividualProcessNode*>* getInitializedNodeLinker();
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* appendInitializedNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker);


						cint64 getOnlyConceptSetProcessedCount();
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* incOnlyConceptSetProcessedCount(cint64 count = 1);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* decOnlyConceptSetProcessedCount(cint64 count = 1);


						CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* getRootQueuingNodeLinker();
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* appendRootQueuingNodeLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeLinker);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* setRootQueuingNodeLinker(CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* indiNodeLinker);



						CXLinker<CIndividualProcessNode*>* getBranchQueuingNodeLinker();
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* appendBranchQueuingNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* setBranchQueuingNodeLinker(CXLinker<CIndividualProcessNode*>* indiNodeLinker);



						cint64 getQueuedNodeInitializingCount();
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* incQueuedNodeInitializingCount(cint64 count = 1);
						CIndividualRepresentativeBackendCacheConceptSetLabelProcessingData* decQueuedNodeInitializingCount(cint64 count = 1);

						//cint64 mLastInitScheduledIndiNodeId = 0;

					// protected methods
					protected:

					// protected variables
					protected:
						cint64 mOnlyConceptSetProcessedCount;
						cint64 mQueuedNodeInitializingCount;
						CXLinker<CIndividualProcessNode*>* mInitializedNodeLinker;
						CIndividualRepresentativeBackendCacheConceptSetLabelNodeQueuingLinker* mRootQueuingNodeLinker;

						CXLinker<CIndividualProcessNode*>* mBranchQueuingNodeLinker;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALREPRESENTATIVEBACKENDCACHECONCEPTSETLABELPROCESSINGDATA_H
