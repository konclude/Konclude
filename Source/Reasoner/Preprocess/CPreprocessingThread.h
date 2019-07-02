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

#ifndef KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTHREAD_H
#define KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTHREAD_H

// Libraries includes


// Namespace includes
#include "PreprocessSettings.h"
#include "CPreprocessor.h"
#include "CPreprocessingContext.h"
#include "COntologyPreprocessingItem.h"
#include "CPreprocessingCallbackDataContext.h"
#include "CPreprocessingTestingItem.h"

// Other includes
#include "Reasoner/Preprocess/Events/CCallbackPreprocessedOntologyEvent.h"
#include "Reasoner/Preprocess/Events/CPreprocessOntologyEvent.h"
#include "Reasoner/Preprocess/Events/CPreprocessingCalculatedCallbackEvent.h"


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

		namespace Preprocess {

			using namespace Events;


			/*! 
			 *
			 *		\class		CPreprocessingThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CPreprocessingThread : public CPreprocessor, public CThread {
				// public methods
				public:
					//! Constructor
					CPreprocessingThread(CReasonerManager* reasonerManager);

					//! Destructor
					virtual ~CPreprocessingThread();

					virtual bool preprocess(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList, CCallbackData* callback);
					virtual bool preprocess(CConcreteOntology* ontology, CConfigurationBase* config, const QList<COntologyProcessingRequirement*>& requirementList);
					virtual bool callbackPreprocessed(CConcreteOntology* ontology, CCallbackData* callback);


				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);


					virtual bool createNextTest() = 0;

					virtual bool canProcessMoreTests();
					virtual bool doNextPendingTests();

					virtual COntologyPreprocessingItem* initializeOntologyPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config);
					virtual bool addOntologyPreprocessingCallback(COntologyPreprocessingItem* item, CCallbackData* callback);
					virtual bool addOntologyPreprocessingRequirements(COntologyPreprocessingItem* item, COntologyProcessingRequirement* requirement);

					CPreprocessingThread* processCalculationJob(CSatisfiableCalculationJob* job, COntologyPreprocessingItem* preCompItem, CPreprocessingTestingItem* preTestItem);

					virtual bool preprocessingTested(COntologyPreprocessingItem* ontPreCompItem, CPreprocessingTestingItem* preTestItem, CPreprocessingCalculatedCallbackEvent* pcce) = 0;

				// protected variables
				protected:
					qint64 mConfMaxTestParallelCount;

					qint64 mCurrRunningTestParallelCount;

					QSet<COntologyPreprocessingItem*> mActiveOntItemSet;
					QSet<COntologyPreprocessingItem*> mInactiveOntItemSet;
					QList<COntologyPreprocessingItem*> mProcessingOntItemList;
					QHash<CConcreteOntology*,COntologyPreprocessingItem*> mOntItemHash;
					cint64 mPreprocessingCount;

					CReasonerManager* mReasoner;
					CCalculationManager* mCalculationManager;


					cint64 mStatCalculatingJobs;
					CPreprocessingContext mContext;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CPREPROCESSINGTHREAD_H
