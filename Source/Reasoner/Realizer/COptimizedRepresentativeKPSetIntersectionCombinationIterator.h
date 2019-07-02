/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_REALIZER_COptimizedRepresentativeKPSetIntersectionCombinationIterator_H
#define KONCLUDE_REASONER_REALIZER_COptimizedRepresentativeKPSetIntersectionCombinationIterator_H

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
			 *		\class		COptimizedRepresentativeKPSetIntersectionCombinationIterator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedRepresentativeKPSetIntersectionCombinationIterator : public CRealizationIndividualInstanceItemReferenceIterator {
				// public methods
				public:
					//! Constructor
					COptimizedRepresentativeKPSetIntersectionCombinationIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> iteratorList);
					virtual ~COptimizedRepresentativeKPSetIntersectionCombinationIterator();


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

					QList<CRealizationIndividualInstanceItemReferenceIterator*>* getCombinedIteratorList();

				// protected methods
				protected:
					class CRealizationIteratorData {
					public:
						CRealizationIteratorData(CRealizationIndividualInstanceItemReferenceIterator* iterator) {
							mIterator = iterator;
						}

						CRealizationIndividualInstanceItemReferenceIterator* mIterator;
					};


					bool moveToValidPosition();
					bool init();


				// protected variables
				protected:
					QList<CRealizationIndividualInstanceItemReferenceIterator*> mIteratorList;
					QList<CRealizationIteratorData*> mIteratorDataList;

					CRealizationIteratorData* mIteratingIteratorData;
					QMap<cint64, CRealizationIteratorData*> mCurrentIndiIdIterationDataMap;


					cint64 mVisitedIterationCount;
					cint64 mIntersectionIterationCount;

					bool mIterateAscending;
			

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_COptimizedRepresentativeKPSetIntersectionCombinationIterator_H
