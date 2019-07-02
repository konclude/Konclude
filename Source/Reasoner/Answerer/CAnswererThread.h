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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERERTHREAD_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERERTHREAD_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringCalculationHandler.h"
#include "CAnswererContext.h"
#include "CAnsweringMessageData.h"
#include "CAnsweringProgress.h"


// Other includes
#include "Reasoner/Answerer/Events/CAnsweringInitializingEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringMessageEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringCalculationCompletedEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringRequirementCompletedEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringHandlerPreparationEvent.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"

#include "Reasoner/Kernel/Cache/COccurrenceStatisticsCache.h"

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
		using namespace Kernel::Cache;

		namespace Answerer {

			using namespace Events;


			/*! 
			 *
			 *		\class		CAnswererThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnswererThread : public CThread, public CAnsweringCalculationHandler {
				// public methods
				public:
					//! Constructor
					CAnswererThread(CReasonerManager *reasonerManager);

					//! Destructor
					virtual ~CAnswererThread();


					bool handleAnswering(CAnsweringHandler* answeringHandling, CComplexAnsweringQuery* query, CCallbackData* callback);


					CAnsweringCalculationHandler* postMessage(CAnsweringHandler* handler, CAnsweringMessageData* message);

					CAnsweringCalculationHandler* postCalculation(CAnsweringHandler* handler, CSatisfiableCalculationJob* satCalcJob, CAnsweringMessageData* finishedMessage, CCallbackDataContext* callbackContext);
					CAnsweringCalculationHandler* ensureRequirements(CAnsweringHandler* handler, CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& requirementList, CAnsweringMessageData* finishedMessage);

					virtual CAnsweringMessageDataObserver* sendMessage(CAnsweringHandler* answeringHandler, CAnsweringMessageData* messageData, CMemoryPool* memoryPool);


					virtual CAnswererThread* initializeAnswerer(CConfiguration* configurationProvider);
					virtual CAnsweringProgress* getAnsweringProgress();

					virtual CAnswererThread* addAnsweringStatistics(CAnsweringStatistics* stats);

					virtual COccurrenceStatisticsCacheReader* getOccurrenceStatisticsCacheReader();

					virtual CAnswererThread* prepareAnswering(CAnsweringHandler* answeringHandler, CCallbackData* callback = nullptr);

				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

					bool continueCalculationCreation();

					CAnswererThread* submitCalculationJobs();

				// protected variables
				protected:
					qint64 mConfMaxTestParallelCount;
					qint64 mConfMinTestParallelCount;
					qint64 mConfDirectSubmission;

					qint64 mCurrRunningTestParallelCount;
					qint64 mCurrRunningRequirementParallelCount;

					class CAnsweringHandlerData {
						public:
							CAnsweringHandler* mAnsweringHandler;
							CCallbackData* mCallbackData;
							bool mQueued;
							bool mProcessing;
							cint64 mJobProcessingCount;
							cint64 mRequirmentProcessingCount;

							CAnsweringHandlerData(CAnsweringHandler* answeringHandling, CCallbackData* callback) {
								mAnsweringHandler = answeringHandling;
								mCallbackData = callback;
								mQueued = false;
								mProcessing = false;
								mJobProcessingCount = 0;
								mRequirmentProcessingCount = 0;
							}
					};

					QList<CAnsweringHandlerData*> mQueuedAnsweringHandlerList;
					QSet<CAnsweringHandlerData*> mProcessingAnsweringHandlerSet;
					QSet<CAnsweringHandlerData*> mManagedAnsweringHandlerSet;

					QHash<CAnsweringHandler*, CAnsweringHandlerData*> mHandlerHandlerDataHash;

					CReasonerManager* mReasonerManager;
					CCalculationManager* mCalculationManager;
					COccurrenceStatisticsCacheReader* mOccStatsCacheReader;


					QList< QPair<CCalculationJob*, CCallbackData*> > mJobCallbackList;

					cint64 mStatCalculationJobsSubmitted;
					cint64 mStatRequirmentSubmitted;

					CAnswererContext* mAnswererContext;


					QReadWriteLock mReadWriteLock;
					CAnsweringProgress mAnsweringProgress;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERERTHREAD_H
