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

#ifndef KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDINDIVIDUALRETRIEVALCOMPUTATIONUPDATECOORDINATIONHASH_H
#define KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDINDIVIDUALRETRIEVALCOMPUTATIONUPDATECOORDINATIONHASH_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "CacheSettings.h"
#include "CBackendIndividualRetrievalComputationUpdateCoordinationHashData.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Cache {


				/*! 
				 *
				 *		\class		CBackendIndividualRetrievalComputationUpdateCoordinationHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CBackendIndividualRetrievalComputationUpdateCoordinationHash : public QMap<cint64, CBackendIndividualRetrievalComputationUpdateCoordinationHashData*> {
					// public methods
					public:
						//! Constructor
						CBackendIndividualRetrievalComputationUpdateCoordinationHash();

						cint64 getApproximateRemainingIncompletelyHandledCount();


						cint64 getHashRemainingCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incHashRemainingCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decHashRemainingCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setHashRemainingCount(cint64 count);


						cint64 getHashRemovedCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incHashRemovedCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decHashRemovedCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setHashRemovedCount(cint64 count);



						cint64 getApproximateCorrectionIncompletelyHandledCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incApproximateCorrectionIncompletelyHandledCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decApproximateCorrectionIncompletelyHandledCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setApproximateCorrectionIncompletelyHandledCount(cint64 count);



						cint64 getApproximateTotalIncompletelyHandledCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incApproximateTotalIncompletelyHandledCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decApproximateTotalIncompletelyHandledCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setApproximateTotalIncompletelyHandledCount(cint64 count);


						cint64 getHashProcessedCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incHashProcessedCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decHashProcessedCount(cint64 count = 1);


						cint64 getHashComputationCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incHashComputationCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decHashComputationCount(cint64 count = 1);

						CBackendIndividualRetrievalComputationUpdateCoordinationHashData* createCoordinationData();


						cint64 getUsageCount();
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* incUsageCount(cint64 count = 1);
						CBackendIndividualRetrievalComputationUpdateCoordinationHash* decUsageCount(cint64 count = 1);


						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setBasicPrecomputationMode(bool basicMode);
						bool hasBasicPrecomputationMode();


						CBackendIndividualRetrievalComputationUpdateCoordinationHash* setBasicPrecomputationFinished(bool basicModeFinished);
						bool hasBasicPrecomputationFinished();

					// protected methods
					protected:

					// protected variables
					protected:


						cint64 mCorrectionIncompletelyHandledCount;
						cint64 mTotalIncompletelyHandledCount;
						cint64 mHashProcessedCount;
						cint64 mHashComputationCount;
						cint64 mHashRemainingCount;

						cint64 mHashRemovedCount;


						cint64 mUsageCount;


						bool mBasicPrecomputationMode;
						bool mBasicPrecomputationFinished;


					// private methods
					private:

					// private variables
					private:


				};

			}; // end namespace Cache

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_CACHE_CBACKENDINDIVIDUALRETRIEVALCOMPUTATIONUPDATECOORDINATIONHASH_H
