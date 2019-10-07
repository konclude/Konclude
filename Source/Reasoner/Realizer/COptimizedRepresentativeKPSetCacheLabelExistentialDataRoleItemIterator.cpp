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

#include "COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator::COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator(const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemKnownInstancesList, const QList<CBackendRepresentativeMemoryLabelCacheItem*>& labelCacheItemPossibleInstancesList,
					QHash<cint64, COptimizedKPSetIndividualItem*>* individualInstantiatedItemHash, CIndividualVector* individualVector, const CRealizationIndividualSorting& sorting, const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor, bool moveOverCursor)
				
					: COptimizedRepresentativeKPSetCacheLabelItemIterator(labelCacheItemKnownInstancesList, labelCacheItemPossibleInstancesList, individualInstantiatedItemHash, individualVector, sorting, indiInstItemRefCursor, moveOverCursor) {

				init();

				mPreventSameIndisMultipleOccurences = true;
			}


			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator::getCopy() {
				COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator* iteratorCopy = new COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator(QList<CBackendRepresentativeMemoryLabelCacheItem*>(), QList<CBackendRepresentativeMemoryLabelCacheItem*>(), mIndividualInstantiatedItemHash, mIndividualVector, mSorting, mIndiInstItemRefCursor, mMoveOverCursor);
				this->initCopy(iteratorCopy);
				return iteratorCopy;
			}

			CRealizationIndividualInstanceItemReferenceIterator* COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator::initCopy(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
				COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator* cacheLabelItemIterator = (COptimizedRepresentativeKPSetCacheLabelExistentialDataRoleItemIterator*)iterator;
				COptimizedRepresentativeKPSetCacheLabelItemIterator::initCopy(cacheLabelItemIterator);
				return cacheLabelItemIterator;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
