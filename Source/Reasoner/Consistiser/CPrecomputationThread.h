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

#ifndef KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTHREAD_H
#define KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTHREAD_H

// Libraries includes


// Namespace includes
#include "ConsistiserSettings.h"
#include "CPrecomputator.h"
#include "CConsistiser.h"
#include "CPrecomputationContext.h"
#include "COntologyPrecomputationItem.h"
#include "CPrecomputationCallbackDataContext.h"
#include "CPrecomputationTestingItem.h"

// Other includes
#include "Reasoner/Consistiser/Events/CCallbackPrecomputedOntologyEvent.h"
#include "Reasoner/Consistiser/Events/CPrecomputeOntologyEvent.h"
#include "Reasoner/Consistiser/Events/CPrecomputationCalculatedCallbackEvent.h"
#include "Reasoner/Consistiser/Events/CSaturationPrecomputationCalculatedCallbackEvent.h"
#include "Reasoner/Consistiser/Events/CRetrievedPrecomputationIndividualsCallbackEvent.h"
#include "Reasoner/Consistiser/Events/CRescheduleJobCreationEvent.h"


#include "Reasoner/Ontology/CConcept.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Concurrent/CThread.h"

#include "Concurrent/Callback/CBlockingCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Concurrent;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel::Manager;
		using namespace Kernel::Calculation;

		namespace Consistiser {

			using namespace Events;


			/*! 
			 *
			 *		\class		CPrecomputationThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPrecomputationThread : public CPrecomputator, public CConsistiser, public CThread {
				// public methods
				public:
					//! Constructor
					CPrecomputationThread(CReasonerManager* reasonerManager);

					//! Destructor
					virtual ~CPrecomputationThread();

					virtual bool precompute(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback);
					virtual bool precompute(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList);
					virtual bool callbackPrecomputed(CConcreteOntology* ontology, CCallbackData* callback);


				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					virtual bool createNextTest() = 0;

					virtual bool canProcessMoreTests();
					virtual bool doNextPendingTests();

					virtual COntologyPrecomputationItem* initializeOntologyPrecomputionItem(CConcreteOntology* ontology, CConfigurationBase* config);
					virtual bool addOntologyPrecomputionCallback(COntologyPrecomputationItem* item, CCallbackData* callback);
					virtual bool addOntologyPrecomputionRequirements(COntologyPrecomputationItem* item, COntologyProcessingRequirement* requirement);

					CPrecomputationThread* processCalculationJob(CSatisfiableCalculationJob* job, COntologyPrecomputationItem* preCompItem, CPrecomputationTestingItem* preTestItem);
					CPrecomputationThread* processCalculationJob(CApproximatedSaturationCalculationJob* job, COntologyPrecomputationItem* preCompItem, CPrecomputationTestingItem* preTestItem);

					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CPrecomputationCalculatedCallbackEvent* pcce) = 0;
					virtual bool precomputationTested(COntologyPrecomputationItem* ontPreCompItem, CPrecomputationTestingItem* preTestItem, CSaturationPrecomputationCalculatedCallbackEvent* pcce) = 0;


					virtual bool precomputationIndividualsRetrieved(COntologyPrecomputationItem* ontPreCompItem, CRetrievedPrecomputationIndividualsCallbackEvent* pcce) = 0;


				// protected variables
				protected:
					qint64 mConfMaxTestParallelCount;
					qint64 mConfMaxTestBatchCreationCount;

					qint64 mCurrRunningTestParallelCount;

					QSet<COntologyPrecomputationItem*> mActiveOntItemSet;
					QSet<COntologyPrecomputationItem*> mInactiveOntItemSet;
					QList<COntologyPrecomputationItem*> mProcessingOntItemList;
					QHash<CConcreteOntology*,COntologyPrecomputationItem*> mOntItemHash;
					cint64 mPrecomputationCount;

					CReasonerManager* mReasoner;
					CCalculationManager* mCalculationManager;


					cint64 mStatCalculatingJobs;
					CPrecomputationContext mContext;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CONSISTISER_CPRECOMPUTATIONTHREAD_H
