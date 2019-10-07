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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetConceptInstancesItem.h"
#include "COptimizedRepresentativeKPSetConceptSetCacheLabelItemInstancesData.h"


// Other includes
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator.h"
#include "Reasoner/Kernel/Cache/CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapExtensionData.h"

#include "Reasoner/Realization/CRealizationIndividualSorting.h"
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReference.h"
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReferenceIterator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Kernel::Cache;
		using namespace Realization;

		namespace Realizer {


			/*! 
			 *
			 *		\class		COptimizedRepresentativeKPSetCacheLabelItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetCacheLabelItemIterator : public CRealizationIndividualInstanceItemReferenceIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetCacheLabelItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList, QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true);
					virtual ~COptimizedRepresentativeKPSetCacheLabelItemIterator();


					virtual bool requiresInitialization();
					virtual bool begin();



					bool atEnd();
					virtual CRealizationRemainingInstancesEstimation remainingInstancesEstimation();

					bool moveNext();
					virtual bool moveTo(const CRealizationIndividualInstanceItemReference& indiInstItemRef, bool moveOver = true);

					CRealizationIndividualInstanceItemReference currentIndividualInstanceItemReference();
					virtual bool isCurrentInstancePossible();
					virtual bool isCurrentInstanceCertain();

					virtual CRealizationIndividualInstanceItemReferenceIterator* getCopy();


					virtual CRealizationIndividualSorting getSorting();


					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getKnownInstancesLabelCacheItemList();
					QList<CBackendRepresentativeMemoryLabelCacheItem*>* getPossibleInstancesLabelCacheItemList();

					CRealizationIndividualInstanceItemReference getIndividualInstanceItemReferenceCursor();
					bool hasMoveOverCursor();


					class CAssociationMapIteratorData {
					public:
						CAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances) {
							mIterator = iterator;
							mPossibleInstances = possibleInstances;
							mCurrentIndiRealizationItem = nullptr;
							mLabelItem = labelItem;
							mCurrentInstanceOnlyPossible = false;
							mIndividual = nullptr;
							mAdditionalData = nullptr;
							mInitializationRequired = false;
							mLastCheckedInitializationRequiredId = -1;
						}

						CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator mIterator;
						CBackendRepresentativeMemoryLabelCacheItem* mLabelItem;
						bool mPossibleInstances;

						CIndividual* mIndividual;
						COptimizedKPSetIndividualItem* mCurrentIndiRealizationItem;
						bool mCurrentInstanceOnlyPossible;
						void* mAdditionalData;

						bool mInitializationRequired;
						cint64 mLastCheckedInitializationRequiredId;

					};

					class CAssociationMapIteratorKey {
					public:
						CAssociationMapIteratorKey(CAssociationMapIteratorData* iterationData) {
							mPossibleInstance = iterationData->mPossibleInstances;
							mIndiId = iterationData->mIterator.currentAssociatedIndividualId();
						}
						CAssociationMapIteratorKey(cint64 indiId, bool possibleInstance) {
							mPossibleInstance = possibleInstance;
							mIndiId = indiId;
						}

						cint64 mIndiId;
						cint64 mPossibleInstance;
					};

				// protected methods
				protected:

						

					virtual CRealizationIndividualInstanceItemReferenceIterator* init();


					virtual CRealizationIndividualInstanceItemReferenceIterator* initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator);


					virtual void clearInstanceIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData);
					virtual bool loadInstanceIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData);


					CAssociationMapIteratorData* createIndividualAssociationsIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, bool possibleInstances, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor);
					CAssociationMapIteratorData* updateIndividualAssociationsIteratorData(CAssociationMapIteratorData* associationIteratorData, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor);
					bool moveToValidPosition(CAssociationMapIteratorData* iteratorData);
					CAssociationMapIteratorData* getNextIteratorDataFromMap();


					virtual bool isValidIteratorPosition(CAssociationMapIteratorData* iteratorData, cint64 indiId);
					cint64 getDifference(cint64 number1, cint64 number2);



					virtual CAssociationMapIteratorData* createAssociationMapIteratorData(CBackendRepresentativeMemoryLabelCacheItem* labelItem, const CBackendRepresentativeMemoryLabelCacheItemIndividualAssociationMapIterator& iterator, bool possibleInstances);


				// protected variables
				protected:
					QList<CBackendRepresentativeMemoryLabelCacheItem*> mLabelCacheItemKnwonInstancesList;
					QList<CBackendRepresentativeMemoryLabelCacheItem*> mLabelCacheItemPossibleInstancesList;
					QHash<cint64, COptimizedKPSetIndividualItem*>* mIndividualInstantiatedItemHash;

					QList<CAssociationMapIteratorData*> mUninitializedList;


					CIndividualVector* mIndividualVector;

					bool mPreventSameIndisMultipleOccurences;
					cint64 mLastIndiId;

					bool mInitializationRequired;

					bool mIterateAscending;
					bool mConsiderSameMergedIndis;
					bool mAllowPossibleInstances;
					CRealizationIndividualSorting mSorting;
					bool mMoveOverCursor;
					CRealizationIndividualInstanceItemReference mIndiInstItemRefCursor;

					CAssociationMapIteratorData* mCurrentIteratorData;

					QList<CAssociationMapIteratorData*> mIteratorDataContainer;
					QMap<CAssociationMapIteratorKey, CAssociationMapIteratorData*> mCurrentIndiIdIterationDataMap;

					cint64 mFirstIteratorIndiId;
					double mAverageIterationIncrementSize;
					cint64 mIterationCount;
					cint64 mAssociatedIndiPossibleCount;
					cint64 mAssociatedIndiTotalCount;

					cint64 mAssociatedIndiPossibleVisistedCount;
					cint64 mAssociatedIndiPossibleConfirmedCount;



					cint64 mCheckingInitializationRequiredId = 1;


					static const cint64 MAX_ITERATION_WITHOUT_MOVE_PER_LABEL = 10;

				// private methods
				private:

				// private variables
				private:

			};




		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETCACHELABELITEMITERATOR_H
