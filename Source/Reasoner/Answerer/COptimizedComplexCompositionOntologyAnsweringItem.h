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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONONTOLOGYANSWERINGITEM_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONONTOLOGYANSWERINGITEM_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "COntologyAnsweringItem.h"
#include "CAnsweringCalculationStatisticsCollectionStrings.h"
#include "CAnsweringStatisticsCollectionStrings.h"
#include "CAnsweringHandlingStatistics.h"
#include "CComputedItemDataNotificationLinker.h"
#include "CComplexQueryCompositionProcessingData.h"


// Other includes
#include "Reasoner/Ontology/CConceptReferenceLinking.h"
#include "Reasoner/Preprocess/COntologyQueryExtendedConfigDependedPreProcesser.h"

#include "Context/CMemoryPoolNewAllocationIncreasingContext.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Context;

	namespace Reasoner {

		using namespace Ontology;

		namespace Answerer {


			typedef QPair<CConcept*, bool> TConceptNegPair;



			/*! 
			 *
			 *		\class		COptimizedComplexConceptOntologyAnsweringItem
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexCompositionOntologyAnsweringItem : public COntologyAnsweringItem {
				// public methods
			public:
				//! Constructor
				COptimizedComplexCompositionOntologyAnsweringItem(CConcreteOntology* ontology, CConfigurationBase* configuration);

				//! Destructor
				virtual ~COptimizedComplexCompositionOntologyAnsweringItem();





				

				CAnsweringCalculationStatisticsCollectionStrings* getAnsweringStatisticsCollectionStrings();
				CAnsweringStatisticsCollectionStrings* getDifferenceStoredAnsweringStatisticsCollectionStrings();
				COptimizedComplexCompositionOntologyAnsweringItem* setDifferenceStoredAnsweringStatisticsCollectionStrings(CAnsweringStatisticsCollectionStrings* stats);


				CAnsweringCalculationStatisticsCollection* getCalculationStatisticsCollection();

				COptimizedComplexCompositionOntologyAnsweringItem* reuseCalculationStatisticsCollection(CAnsweringCalculationStatisticsCollection* statColl);

				bool isProcessStatisticsCollectionConfigured();
				bool isLogProcessingProgressStatisticsConfigured();
				bool isLogProcessingFinishedStatisticsConfigured();

				CAnsweringHandlingStatistics* getAnsweringHandlingStatistics();





				QList<CComplexQueryCompositionProcessingData*>* getScheduledQueriesProcessingDataList();
				COptimizedComplexCompositionOntologyAnsweringItem* addScheduledQueriesProcessingData(CComplexQueryCompositionProcessingData* procData);



				COptimizedComplexCompositionOntologyAnsweringItem* decCurrentlyAnsweringQueryCount();
				COptimizedComplexCompositionOntologyAnsweringItem* incCurrentlyAnsweringQueryCount();
				bool isCurrentlyAnsweringQueryCount();

				// protected methods
			protected:

				// protected variables
			protected:
				CAnsweringHandlingStatistics* mHandlingStats;
				CAnsweringStatisticsCollectionStrings* mDiffStoredStatCollStrings;
				CAnsweringCalculationStatisticsCollectionStrings* mCalcStatCollStrings;
				QList<CAnsweringCalculationStatisticsCollection*> mReuseStatCollList;

				bool mConfCollectProcessStatistics;
				bool mConfLogProcessingProgressStatistics;
				bool mConfLogProcessingFinishedStatistics;

				CAnsweringHandlingStatistics mAnsweringHandlingStatistics;

				QList<CComplexQueryCompositionProcessingData*> mScheduledQueriesProcDataList;

				cint64 mCurrentlyAnsweringQueryCount;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXCOMPOSITIONONTOLOGYANSWERINGITEM_H
