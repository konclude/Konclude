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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETUNSORTEDINDIVIDUALROLEFILLERITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETUNSORTEDINDIVIDUALROLEFILLERITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedKPSetIndividualComplexRoleData.h"


// Other includes
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReferenceIterator.h"

#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryCacheReader.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Cache;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator : public CRealizationIndividualInstanceItemReferenceIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetUnsortedIndividualRoleFillerItemIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, bool considerSameMergedIndis);


					virtual bool requiresInitialization();
					virtual bool begin();


					virtual bool atEnd();
					virtual CRealizationRemainingInstancesEstimation remainingInstancesEstimation();

					virtual bool moveNext();
					virtual bool moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver = true);

					virtual CRealizationIndividualInstanceItemReference currentIndividualInstanceItemReference();
					virtual bool isCurrentInstancePossible();
					virtual bool isCurrentInstanceCertain();

					virtual CRealizationIndividualInstanceItemReferenceIterator* getCopy();

					virtual CRealizationIndividualSorting getSorting();



					CRealizationIndividualInstanceItemReference getQueryingIndividualInstanceItemReference();
					COptimizedKPSetRoleInstancesItem* getQueryingRoleInstancesItem();
					bool getQueryingRoleInstancesItemInversion();


				// protected methods
				protected:
					bool moveToNextPossiblePosition(bool forceNextPosition = false);

					bool isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef);

				// protected variables
				protected:
					CRealizationIndividualInstanceItemReference mIndiInstItemRef;
					CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;
					COptimizedKPSetRoleInstancesItem* mRoleInstancesItem;
					bool mInversed;

					QHash<cint64, COptimizedKPSetIndividualItem*>* mIndividualInstantiatedItemHash;
					CIndividualVector* mIndividualVector;


					bool mNeighbourPossibleInstanceDataIteratorsInitialized;
					bool mNeighbourPossibleInstanceDataIteratorsFinished;
					COptimizedKPSetRoleInstancesHash::const_iterator mNeighbourPossibleInstanceDataIt;
					COptimizedKPSetRoleInstancesHash::const_iterator mNeighbourPossibleInstanceDataItEnd;


					bool mNeighbourArrayIteratorsInitialized;
					bool mNeighbourArrayIteratorsFinished;
					CBackendRepresentativeMemoryCacheIndividualAssociationData* mIndiAssData;
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>::const_iterator mSingleNeighLabelDetDataHashIt;
					QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>::const_iterator mSingleNeighLabelDetDataHashItEnd;
					//QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>::const_iterator mSingleNeighLabelNonDetDataHashIt;
					//QHash<CBackendRepresentativeMemoryLabelCacheItem*, COptimizedKPSetRoleInstancesSingleNeighbourRoleSetCacheLabelData*>::const_iterator mSingleNeighLabelNonDetDataHashItEnd;
					cint64 mCurrentNeighbourArrayVisitingPositionCursor;


					bool mNeighbourComplexDataIteratorsInitialized;
					bool mNeighbourComplexDataIteratorsFinished;
					COptimizedKPSetRoleInstancesHash::const_iterator mNeighbourComplexInstanceDataIt;
					COptimizedKPSetRoleInstancesHash::const_iterator mNeighbourComplexInstanceDataItEnd;


					bool mAllowPossibleFillers;


					COptimizedKPSetIndividualComplexRoleData** mComplexRoleDataPointer;
					bool mInitializationRequired;

					cint64 mCurrentIndiId;
					bool mHasCurrentIndiId;
					CRealizationIndividualInstanceItemReference mCurrentIndiInstItemRef;
					bool mCurrentIndiInstanceOnlyPossible;

					bool mConsiderSameMergedIndis;
					bool mSameMergedIndisInCache;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETUNSORTEDINDIVIDUALROLEFILLERITEMITERATOR_H
