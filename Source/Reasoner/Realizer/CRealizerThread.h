/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_Realizer_CREALIZERTHREAD_H
#define KONCLUDE_REASONER_Realizer_CREALIZERTHREAD_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizer.h"
#include "CRealizingContext.h"
#include "COntologyRealizingItem.h"
#include "CRealizingCallbackDataContext.h"
#include "CRealizingTestingItem.h"
#include "CRealizationMessageObserver.h"

// Other includes
#include "Reasoner/Realizer/Events/CCallbackRealizedOntologyEvent.h"
#include "Reasoner/Realizer/Events/CRealizeOntologyEvent.h"
#include "Reasoner/Realizer/Events/CRealizingCalculatedCallbackEvent.h"
#include "Reasoner/Realizer/Events/CRealizationMessageEvent.h"

#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/COntologyProcessingRequirement.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Concurrent/CThread.h"

#include "Concurrent/Callback/CBlockingCallbackData.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogDomain.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent;
	using namespace Concurrent::Callback;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Kernel::Manager;
		using namespace Kernel::Calculation;

		namespace Realizer {

			using namespace Events;


			/*! 
			 *
			 *		\class		CRealizerThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizerThread : public CRealizer, public CThread, public CRealizationMessageObserver {
				// public methods
				public:
					//! Constructor
					CRealizerThread(CReasonerManager* reasonerManager);

					//! Destructor
					virtual ~CRealizerThread();

					virtual bool realize(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback);
					virtual bool realize(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList);
					virtual bool callbackRealized(CConcreteOntology* ontology, CCallbackData* callback);

					virtual CRealizationMessageObserver* tellRealizationMessage(CConcreteOntology *ontology, CRealizationMessageData* messageData, CMemoryPool* memoryPool);

				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					virtual bool createNextTest() = 0;

					virtual bool canProcessMoreTests();
					virtual bool doNextPendingTests();

					virtual COntologyRealizingItem* initializeOntologyRealizingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					virtual bool addOntologyRealizingCallback(COntologyRealizingItem* item, CCallbackData* callback);
					virtual bool addOntologyRealizingRequirements(COntologyRealizingItem* item, COntologyProcessingRequirement* requirement);

					CRealizerThread* processCalculationJob(CSatisfiableCalculationJob* job, COntologyRealizingItem* preCompItem, CRealizingTestingItem* preTestItem);

					virtual bool realizingTested(COntologyRealizingItem* ontPreCompItem, CRealizingTestingItem* preTestItem, CRealizingCalculatedCallbackEvent* pcce) = 0;

					virtual bool processRealizationMessage(COntologyRealizingItem* ontRealItem, CRealizationMessageData* messageData, CMemoryPool* memoryPools) = 0;


				// protected variables
				protected:
					qint64 mConfMaxTestParallelCount;

					qint64 mCurrRunningTestParallelCount;

					QSet<COntologyRealizingItem*> mActiveOntItemSet;
					QSet<COntologyRealizingItem*> mInactiveOntItemSet;
					QList<COntologyRealizingItem*> mProcessingOntItemList;
					QHash<CConcreteOntology*,COntologyRealizingItem*> mOntItemHash;
					cint64 mRealizingCount;

					CReasonerManager* mReasoner;
					CCalculationManager* mCalculationManager;

					cint64 mStatCalculatingJobs;
					CRealizingContext mContext;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_Realizer_CREALIZERTHREAD_H
