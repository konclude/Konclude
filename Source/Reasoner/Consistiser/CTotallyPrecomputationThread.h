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

#ifndef KONCLUDE_REASONER_CONSISTISER_CTOTALLYPRECOMPUTATIONTHREAD_H
#define KONCLUDE_REASONER_CONSISTISER_CTOTALLYPRECOMPUTATIONTHREAD_H

// Libraries includes
#include <QStack>
#include <QListIterator>


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputationThread.h"
#include "CPrecomputationTestingItem.h"
#include "CConsistencePrecomputationTestingItem.h"
#include "CConceptCyclePrecomputationTestingItem.h"
#include "CSaturationPrecomputationTestingItem.h"
#include "CTotallyOntologyPrecomputationItem.h"
#include "CSaturationCommonDisjunctConceptsExtractor.h"

// Other includes
#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheWriter.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCacheEntryExpandWriteData.h"

#include "Reasoner/Kernel/Task/CCalculationConfigurationExtension.h"

#include "Reasoner/Generator/CSatisfiableCalculationJobGenerator.h"
#include "Reasoner/Generator/CApproximatedSaturationCalculationJobGenerator.h"

#include "Utilities/Memory/CTempMemoryPoolContainerAllocationManager.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		using namespace Kernel::Task;
		using namespace Kernel::Manager;
		using namespace Kernel::Cache;
		using namespace Generator;

		namespace Consistiser {

			using namespace Events;

			class CConceptNegationSaturationItem : public QPair<CSaturationConceptDataItem*,QPair<CConcept*,bool> > {
				public:
					CConceptNegationSaturationItem(CConcept* concept, bool negation, CSaturationConceptDataItem* ontConSatDataItem) : QPair<CSaturationConceptDataItem*,QPair<CConcept*,bool> >(ontConSatDataItem,QPair<CConcept*,bool>(concept,negation)) {
					}
					CSaturationConceptDataItem* getSaturationConceptDataItem() {
						return first;
					}
					CConcept* getConcept() {
						return second.first;
					}
					bool getNegation() {
						return second.second;
					}
			};


			class CSaturationSaturationListItem {
				public: 
					CSaturationSaturationListItem() {
						mSubsumerOrdered = false;
						mLastSubsumerSaturationItem = nullptr;
						mSaturationItem = nullptr;
					}
					CSaturationSaturationListItem(CSaturationConceptDataItem* saturationItem) : mSaturationItem(saturationItem) {
						mSubsumerOrdered = false;
						mLastSubsumerSaturationItem = nullptr;
					}
					CSaturationConceptDataItem* mSaturationItem;
					CSaturationConceptDataItem* mLastSubsumerSaturationItem;
					QList<CSaturationConceptDataItem*> mSaturationItemList;
					bool mSubsumerOrdered;
			};



			/*! 
			 *
			 *		\class		CTotallyPrecomputationThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTotallyPrecomputationThread : public CPrecomputationThread, public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CTotallyPrecomputationThread(CReasonerManager* reasonerManager);

					//! Destructor
					virtual ~CTotallyPrecomputationThread();


				// protected methods
				protected:
					virtual COntologyPrecomputationItem* initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config);

					virtual bool createNextTest();

					virtual bool finishOntologyPrecomputation(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce);
					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce);

					bool createConsistencePrecomputationCheck(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					bool createConceptCyclePrecomputation(CConceptCycleData* conceptCycleData, CTotallyOntologyPrecomputationItem* totallyPreCompItem);



					void createSaturationConstructionJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					bool hasRoleRanges(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CRole* role);
					void orderItemsSaturationTesting(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>& orderedItemList, QStack<CSaturationSaturationListItem>& saturationItemStack);
					void analyseConceptSaturationSubsumerExistItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, QList<CConceptNegationSaturationItem>& processList);
					void propagateSubsumerItemFlag(CSaturationConceptDataItem* ontClassSatTestItem);
					void propagateExistInitializationFlag(CSaturationConceptDataItem* ontConSatDataItem);

					void extendDisjunctionsCandidateAlternativesItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* ontConSatDataItem, QList<CSaturationConceptDataItem*>* newDisjunctionCandidateAlternativeList);




					void createSaturationProcessingJob(CTotallyOntologyPrecomputationItem* totallyPreCompItem, bool allowAllSaturation);
					bool saturateRemainingRequiredItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					cint64 markSaturationProcessingItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem, CSaturationConceptDataItem* startMarkingItem, CConcept* startMarkingConcept, bool startMarkingConceptNegation);
					bool saturateRemainingConsistencyRequiredItems(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					bool addIdentifiedRemainingConsistencyRequiredConcepts(CTotallyOntologyPrecomputationItem* totallyPreCompItem);



					void extractCommonDisjunctConceptsFromPrecomputedSaturation(CTotallyOntologyPrecomputationItem* totallyPreCompItem);
					void logSaturationInfos(CTotallyOntologyPrecomputationItem* totallyPreCompItem);


				// protected variables
				protected:
					CReuseCompletionGraphCacheWriter* mReuseCompletionGraphCacheWriter;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CTOTALLYPRECOMPUTATIONTHREAD_H
