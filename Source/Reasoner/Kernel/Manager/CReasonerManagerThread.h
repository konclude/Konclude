/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGERTHREAD_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGERTHREAD_H

// Qt includes
#include <QLinkedList>
#include <QSet>

// Namespace includes
#include "CReasonerManager.h"
#include "CPrecomputationManager.h"
#include "CPreprocessingManager.h"
#include "COntologyRequirementPair.h"
#include "CRequirementPreparingData.h"
#include "CReasoningTaskData.h"
#include "CRealizationManager.h"


// Other includes
#include "Reasoner/Kernel/Calculation/CCalculationManager.h"
#include "Reasoner/Kernel/Calculation/CConfigDependedCalculationEnvironmentFactory.h"
#include "Reasoner/Kernel/Calculation/CTaskHandleAlgorithmBuilder.h"
#include "Reasoner/Kernel/Calculation/CConfigDependedCalculationFactory.h"

#include "Reasoner/Kernel/Algorithm/CCalculationTableauCompletionTaskHandleAlgorithm.h"
#include "Reasoner/Kernel/Algorithm/CCalculationTableauSaturationTaskHandleAlgorithm.h"
#include "Reasoner/Kernel/Algorithm/CCalculationTableauPilingSaturationTaskHandleAlgorithm.h"
#include "Reasoner/Kernel/Algorithm/CCalculationTableauApproximationSaturationTaskHandleAlgorithm.h"
#include "Reasoner/Kernel/Algorithm/CCalculationChooseTaskHandleAlgorithm.h"
#include "Reasoner/Kernel/Algorithm/CUnsatisfiableCacheHandler.h"
#include "Reasoner/Kernel/Algorithm/CSatisfiableExpanderCacheHandler.h"
#include "Reasoner/Kernel/Algorithm/CReuseCompletionGraphCacheHandler.h"
#include "Reasoner/Kernel/Algorithm/CSaturationNodeExpansionCacheHandler.h"

#include "Reasoner/Ontology/COntologyProcessingRequirementExpander.h"
#include "Reasoner/Ontology/COntologyProcessingStepRequirement.h"
#include "Reasoner/Ontology/COntologyProcessingStepVector.h"
#include "Reasoner/Ontology/COntologyProcessingConceptSaturationRequirement.h"

#include "Reasoner/Query/CJobSatisfiableCallbackContextData.h"
#include "Reasoner/Query/CJobProvidedQuery.h"
#include "Reasoner/Query/CRealizationPremisingQuery.h"
#include "Reasoner/Query/CTaxonomyPremisingQuery.h"
#include "Reasoner/Query/CConsistencePremisingQuery.h"
#include "Reasoner/Query/CQueryInconsitentOntologyError.h"
#include "Reasoner/Query/CQueryUnspecifiedStringError.h"

#include "Reasoner/Kernel/Manager/Events/CJobCalculatedSatisfiableCallbackEvent.h"
#include "Reasoner/Kernel/Manager/Events/CReasoningSatisfiableCalculationJobEvent.h"
#include "Reasoner/Kernel/Manager/Events/CCalcQueryEvent.h"
#include "Reasoner/Kernel/Manager/Events/CCalcedQueryEvent.h"
#include "Reasoner/Kernel/Manager/Events/CQueriedTaxonomyComputedCallbackEvent.h"
#include "Reasoner/Kernel/Manager/Events/CQueryJobCalculatedSatisfiableCallbackEvent.h"
#include "Reasoner/Kernel/Manager/Events/CRequirementProcessedCallbackEvent.h"
#include "Reasoner/Kernel/Manager/Events/CPrepareOntologyEvent.h"


#include "Concurrent/CIntervalThread.h"
#include "Concurrent/Callback/CBlockingCallbackData.h"

#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCache.h"
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheReader.h"
#include "Reasoner/Kernel/Cache/COccurrenceUnsatisfiableCacheWriter.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCache.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheWriter.h"
#include "Reasoner/Kernel/Cache/CSignatureSatisfiableExpanderCacheReader.h"
#include "Reasoner/Kernel/Cache/CReuseCompletionGraphCache.h"
#include "Reasoner/Kernel/Cache/CSaturationNodeAssociatedExpansionCache.h"
#include "Reasoner/Kernel/Cache/CComputedConsequencesCache.h"

#include "Reasoner/Classifier/CClassificationManager.h"
#include "Reasoner/Classifier/CClassifiedCallbackDataContext.h"

#include "Config/CGlobalConfigurationProvider.h"
#include "Config/CConfigDataReader.h"

#include "Control/Command/CConfigManagerReader.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	using namespace Concurrent;
	using namespace Callback;
	using namespace Scheduler;

	using namespace Control::Command;

	namespace Reasoner {

		using namespace Query;
		using namespace Classifier;
		using namespace Ontology;
		using namespace Preprocess;
		using namespace Consistiser;

		namespace Kernel {

			using namespace Cache;
			using namespace Task;
			using namespace Calculation;
			using namespace Algorithm;

			namespace Manager {

				using namespace Events;

				/*! 
				 *
				 *		\class		CReasonerManagerThread
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CReasonerManagerThread : public CReasonerManager, public CIntervalThread, public CTaskHandleAlgorithmBuilder {
					// public methods
					public:
						//! Constructor
						CReasonerManagerThread(CWatchDog *threadWatchDog = 0);

						//! Destructor
						virtual ~CReasonerManagerThread();


						virtual CReasonerManager* reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob, CCallbackData* callback);
						virtual CJobSatisfiableResult* reasoningSatisfiableCalcualtionJob(CSatisfiableCalculationJob* satCalcJob);

						virtual CReasonerManager* reasoningQuery(CQuery *query, CCallbackData *callback);
						virtual CReasonerManager* reasoningQuery(CQuery *query);


						virtual CReasonerManager* prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList, CCallbackData *callback);
						virtual CReasonerManager* prepareOntology(CConcreteOntology* ontology, const QList<COntologyProcessingRequirement*>& reqList);




						virtual CUnsatisfiableCache *getUnsatisfiableCache();

						virtual CCompletionGraphCache *getCompletionGraphCache();

						virtual CReasonerManager *initializeManager(CConfigurationProvider *configurationProvider);

						virtual CTaskHandleAlgorithm *createTaskHandleAlgorithm();


						virtual CCalculationManager *getCalculationManager();
						virtual CPrecomputationManager* getPrecomputationManager();
						virtual CPreprocessingManager* getPreprocessingManager();
						virtual CRealizationManager* getRealizationManager();




					// protected methods
					protected:
						virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

						virtual void threadStarted();
						virtual void threadStopped();

						virtual void logQueryProgressUpdates();

						virtual void readConfig(CConfigurationBase *config);


						virtual bool processTimer(qint64 timerID);


						virtual const QList<COntologyRequirementPair> getRequirementsForQuery(CQuery* query);

						virtual void prepareOntologyReasoning(CPrepareOntologyEvent* poe);
						virtual void prepareQueryReasoning(CCalcQueryEvent *cqe);
						virtual void initiateQueryReasoning(CQuery* query, CCallbackData* callbackData, CReasoningTaskData* reasoningData, const QList<COntologyRequirementPair>& failedRequirementList);
						virtual void continueRequirementProcessing(CRequirementPreparingData* reqData, CConcreteOntology* ontology);
						virtual void finishQueryReasoning(CCalcedQueryEvent *cqe);


						virtual void prepareSatisfiableBoxReasoning(CReasoningSatisfiableCalculationJobEvent *cbse);
						virtual void finishSatisfiableBoxReasoning(CJobCalculatedSatisfiableCallbackEvent *cbsce);
						virtual void finishQuerySatisfiableBoxReasoning(CQueryJobCalculatedSatisfiableCallbackEvent *cbsce);

						virtual void loggingCalculationStatistics();
						virtual QString getLoggingCalculationStatisticsTimeSizeString(double milliseconds);
						virtual QString getLoggingCalculationStatisticsMemorySizeString(cint64 memSize);


					// protected variables
					protected:


						CWatchDog *mWatchDog;

						QHash<cint64,CReasoningTaskData*> mReasoningTaskDataHash;
						QHash<CQuery*,CCallbackData*> mQueryCallbackHash;
						QHash<CSatisfiableCalculationJob*,CCallbackData*> mJobCallbackHash;

						QSet<CRequirementPreparingData*> mProcessingRequirementsSet;

						QSet<CQuery*> mCalculatingQuerySet;
						QSet<CSatisfiableCalculationJob*> mCalcJobSet;

						QSet< QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE> > mProcessingStartMessageSet;
						QSet< QPair<CConcreteOntology*,COntologyProcessingStep::PROCESSORTYPE> > mProcessingEndMessageSet;


						QHash<cint64,QTime> mCalcQueryJobStartTimeHash;

						COccurrenceUnsatisfiableCache *unsatCache;
						CSignatureSatisfiableExpanderCache* mSatExpCache;
						CReuseCompletionGraphCache* mReuseCompGraphCache;
						CSaturationNodeAssociatedExpansionCache* mSatNodeExpCache;
						CComputedConsequencesCache* mCompConsCache;


						static const qint64 PROGRESSQUERYTIMER = 1;

						CConfigurationProvider* configProvider;
						CClassificationManager* classificationMan;

						CCalculationManager* mCalculationManager;
						CPrecomputationManager* mPrecomputationManager;
						CPreprocessingManager* mPreprocessingManager;
						CRealizationManager* mRealizationManager;

						COntologyProcessingRequirementExpander* mRequirementExpander;

						cint64 mWorkControllerCount;

					// private methods
					private:
						bool updateBeginingCalculationStatistics(CReasoningTaskData* reaTaskData);
						bool updateFinishingCalculationStatistics(CReasoningTaskData* reaTaskData, CQueryStatistics* queryStat);

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CREASONERMANAGERTHREAD_H
