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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERTHREAD_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERTHREAD_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "AnswererSettings.h"
#include "CAnsweringManager.h"
#include "CAnswererThread.h"
#include "CFirstQueuedAnsweringHandlerProvider.h"
#include "CAnsweringManagerQueryData.h"
#include "CConfigurationAnsweringHandlerFactory.h"
#include "CAnsweringStatisticsCollectionStrings.h"


// Other includes
#include "Reasoner/Answerer/Events/CAnsweringComplexQueryCompletedEvent.h"
#include "Reasoner/Answerer/Events/CAnswerComplexQueryEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringOntologyPreparationEvent.h"
#include "Reasoner/Answerer/Events/CAnsweringPreparedEvent.h"

#include "Reasoner/Query/CComplexAnsweringQuery.h"

#include "Reasoner/Kernel/Manager/CReasonerManager.h"


#include "Concurrent/CThread.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"





namespace Konclude {

	namespace Reasoner {

		using namespace Query;
		using namespace Kernel::Manager;

		namespace Answerer {



			/*! 
			 *
			 *		\class		CAnsweringManagerThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringManagerThread : public CAnsweringManager, public CThread {
				// public methods
				public:
					//! Constructor
					CAnsweringManagerThread(CReasonerManager* reasonerManager, CConfiguration* config);

					//! Destructor
					virtual ~CAnsweringManagerThread();

					virtual CAnsweringManager* answerComplexQuery(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback = nullptr);

					virtual CAnsweringProgress* getAnsweringProgress();


					virtual CAnsweringStatistics* getAnsweringStatistics(CConcreteOntology* ontology);

					virtual CAnsweringManager* prepareAnswering(CConcreteOntology* ontology, CCallbackData* callback = nullptr);


				// protected methods
				protected:
					CAnsweringManager* queueComplexQueryAnswering(CComplexAnsweringQuery* complexAnsweringQuery, CCallbackData* callback);
					CAnsweringManager* finishComplexQueryAnswering(CAnsweringHandler* answeringHandler, CAnsweringManagerQueryData* queryData);

					virtual CAnsweringManager* manageComplexQueryAnswering();

					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

				// protected variables
				protected:
					CConfiguration* mConfig;
					CFirstQueuedAnsweringHandlerProvider mHandlerProvider;


					CReasonerManager* mReasonerManager;


					QList<CAnswererThread*> mAnswererThreadContainer;
					QList<CAnswererThread*> mQueuedThreadList;
					QSet<CAnswererThread*> mProcessingThreadSet;
					cint64 mCurrentThreadCount;
					cint64 mConfMaxThreadCount;


					QList<CAnsweringManagerQueryData*> mQueuedQueryList;
					QSet<CAnsweringManagerQueryData*> mProcessingQuerySet;

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

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGMANAGERTHREAD_H
