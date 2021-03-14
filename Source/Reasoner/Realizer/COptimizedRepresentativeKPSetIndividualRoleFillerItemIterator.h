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

#ifndef KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETINDIVIDUALROLEFILLERITEMITERATOR_H
#define KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETINDIVIDUALROLEFILLERITEMITERATOR_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "COptimizedKPSetRoleInstancesItem.h"
#include "COptimizedKPSetIndividualComplexRoleData.h"
#include "COptimizedKPSetIndividualInstantiatedItemMultiHash.h"


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
			 *		\class		COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator : public CRealizationIndividualInstanceItemReferenceIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetIndividualRoleFillerItemIterator(const CRealizationIndividualInstanceItemReference& indiInstItemRef, CBackendRepresentativeMemoryCacheReader* backendAssocCacheReader, COptimizedKPSetRoleInstancesItem* roleInstancesItem, bool inversed, COptimizedKPSetIndividualInstantiatedItemMultiHash* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor = true);


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
					class IndividualIdSourceData {
					public:
						IndividualIdSourceData() {
						}

						IndividualIdSourceData(bool isFromAssociationNeighbourArray, bool isFromComplexRoleDataHash, bool isKnownNeighbour, bool isPossibleNeighbour, COptimizedKPSetRoleInstancesData* complexInstancesData) {
							mIsFromAssociationNeighbourArray = isFromAssociationNeighbourArray;
							mIsFromComplexRoleDataHash = isFromComplexRoleDataHash;
							mIsKnownNeighbour = isKnownNeighbour;
							mIsPossibleNeighbour = isPossibleNeighbour;
							mComplexInstancesData = complexInstancesData;
						}

						bool mIsFromAssociationNeighbourArray = false;
						bool mIsFromComplexRoleDataHash = false;
						bool mIsKnownNeighbour = false;
						bool mIsPossibleNeighbour = false;
						COptimizedKPSetRoleInstancesData* mComplexInstancesData = nullptr;
					};


					bool isSameIndividualsMerged(const CRealizationIndividualInstanceItemReference& indiInstItemRef);

					virtual bool isValidIteratorPosition(cint64 indi, IndividualIdSourceData& sourceData);
					bool moveToValidPosition(QMap<cint64, IndividualIdSourceData>::const_iterator& neighbourIndiIdsMapIt);

				// protected variables
				protected:
					CRealizationIndividualInstanceItemReference mIndiInstItemRef;
					COptimizedKPSetIndividualItem* mIndiInstItem;
					CBackendRepresentativeMemoryCacheReader* mBackendAssocCacheReader;
					CBackendRepresentativeMemoryCacheIndividualAssociationData* mIndiAssData;
					COptimizedKPSetRoleInstancesItem* mRoleInstancesItem;
					bool mInversed;
					COptimizedKPSetIndividualInstantiatedItemMultiHash* mIndividualInstantiatedItemHash;
					CIndividualVector* mIndividualVector;
					CRealizationIndividualSorting mSorting;
					CRealizationIndividualInstanceItemReference mIndiInstItemRefCursor;
					bool mMoveOverCursor;
					COptimizedKPSetRoleNeighbourInstancesHashData* mNeighbourPossibleInstanceHashData;
					bool mAllowPossibleFillers;

					bool mIterateAscending;
					bool mConsiderSameMergedIndis;

					COptimizedKPSetIndividualComplexRoleData** mComplexRoleDataPointer;
					bool mInitializationRequired;
					bool mMapInitialized;

					QMap<cint64, IndividualIdSourceData> mNeighbourIndiIdsMap;

					QMap<cint64, IndividualIdSourceData>::const_iterator mNeighbourIndiIdsMapItBegin;
					QMap<cint64, IndividualIdSourceData>::const_iterator mNeighbourIndiIdsMapItEnd;
					QMap<cint64, IndividualIdSourceData>::const_iterator mNeighbourIndiIdsMapIt;
					cint64 mCurrentIndiId;
					bool mHasCurrentIndiId;
					IndividualIdSourceData mCurrentSourceData;
					COptimizedKPSetIndividualItem* mCurrentIndividualItem;
					CIndividual* mCurentIndividual;
					CRealizationIndividualInstanceItemReference mCurrentIndiInstItemRef;
					bool mCurrentIndiInstanceOnlyPossible;

					cint64 mMinIndiId;
					cint64 mMaxIndiId;
					cint64 mPossibleIndiCount;
					cint64 mVisitedPossibleIndiCount;
					cint64 mConfirmedPossibleIndiCount;

					cint64 mLastIndiId;
					cint64 mAverageIterationIncrementSize;
					cint64 mIterationCount;
					bool mIterationMoved;

					bool mSameMergedIndisInCache;

					const static cint64 MAX_ITERTION_BEFORE_MOVE_LIMIT = 10;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COPTIMIZEDREPRESENTATIVEKPSETINDIVIDUALROLEFILLERITEMITERATOR_H
