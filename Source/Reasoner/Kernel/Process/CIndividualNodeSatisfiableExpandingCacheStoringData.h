/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODESATISFIABLEEXPANDINGCACHESTORINGDATA_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODESATISFIABLEEXPANDINGCACHESTORINGDATA_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptor.h"
#include "CIndividualNodeSatisfiableCacheStoringData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CIndividualNodeSatisfiableExpandingCacheStoringData
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CIndividualNodeSatisfiableExpandingCacheStoringData : public CIndividualNodeSatisfiableCacheStoringData {
					// public methods
					public:
						//! Constructor
						CIndividualNodeSatisfiableExpandingCacheStoringData();

						//! Destructor
						virtual ~CIndividualNodeSatisfiableExpandingCacheStoringData();

						CIndividualNodeSatisfiableExpandingCacheStoringData* initSatisfiableExpandingCacheRetrievalData(CIndividualNodeSatisfiableExpandingCacheStoringData* prevData);

						bool hasIndividualNodeOrSuccessorBranchedConcept();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setIndividualNodeOrSuccessorBranchedConcept(bool branched);

						bool hasPreviousCached();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setPreviousCached(bool cached);

						cint64 getLastCachedSignature();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setLastCachedSignature(cint64 signature);

						CConceptDescriptor* getLastCachedConceptDescriptor();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setLastCachedConceptDescriptor(CConceptDescriptor* conDes);

						cint64 getMinimalIndividualNodeBranchingTag();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setMinimalIndividualNodeBranchingTag(cint64 branchingTag);

						bool hasCachingError();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setCachingError(bool error);

						bool hasPreviousSatisfiableCached();
						CIndividualNodeSatisfiableExpandingCacheStoringData* setPreviousSatisfiableCached(bool cached);

					// protected methods
					protected:

					// protected variables
					protected:
						bool mNodeSuccBranched;
						bool mPrevCached;
						cint64 mLastCachedSignature;
						CConceptDescriptor* mLastCachedConDes;
						bool mCachingError;
						cint64 mMinNodeBranchingTag;
						bool mPrevSatCached;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CINDIVIDUALNODESATISFIABLEEXPANDINGCACHESTORINGDATA_H
