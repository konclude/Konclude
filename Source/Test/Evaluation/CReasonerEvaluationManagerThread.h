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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONMANAGERTHREAD_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONMANAGERTHREAD_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationManager.h"
#include "CReasonerEvaluationExecutorThread.h"
#include "CReasonerEvaluationTestsuite.h"

#include "CReasonerEvaluationCollector.h"
#include "CReasonerEvaluationClassificationTimeExtractor.h"
#include "CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"
#include "CReasonerEvaluationAvaragerSummarizer.h"
#include "CReasonerEvaluationGroupRequestReasonerCSVComparisonAnalyser.h"
#include "CReasonerEvaluationGroupRequestReasonerWithMeanMinMaxCSVComparisonAnalyser.h"
#include "CReasonerEvaluationGroupReasonerWinnerCountCSVComparisonAnalyser.h"
#include "CReasonerEvaluationHighchartPlotter.h"
#include "CReasonerEvaluationPathConverter.h"
#include "CReasonerEvaluationExpressivitySelectorFactory.h"
#include "CReasonerEvaluationDirectoryStructureSelectorFactory.h"
#include "CReasonerEvaluationLoadingTimeExtractor.h"
#include "CReasonerEvaluationAnalyserChecker.h"
#include "CReasonerEvaluationResponseFileExtractor.h"
#include "CReasonerEvaluationResponseFileListSummarizer.h"
#include "CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser.h"
#include "CReasonerEvaluationResponseTimeExtractor.h"
#include "CReasonerEvaluationGroupRequestReasonerAccumulatedCSVComparisonAnalyser.h"
#include "CReasonerEvaluationHTMLNavigationOverviewSummarizer.h"
#include "CReasonerEvaluationGroupRequestReasonerErrorAnalyser.h"
#include "CReasonerEvaluationTimeoutErrorExtractor.h"
#include "CReasonerEvaluationProfileSelectorFactory.h"
#include "CReasonerEvaluationSpecifiedTimeExtractor.h"
#include "CReasonerEvaluationDataValueCacher.h"
#include "CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser.h"
#include "CReasonerEvaluationGroupRequestReasonerAccumulatedCountCSVComparisonAnalyser.h"
#include "CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser.h"

// Other includes
#include "Config/CConfiguration.h"

#include "Concurrent/CThread.h"

#include "Test/Evaluation/Events/CReasonerEvaluationProgramEvent.h"

#include "Control/Command/CConfigManagerReader.h"

#include "Control/Command/Records/CStartProcessCommandRecord.h"
#include "Control/Command/Records/CStopProcessCommandRecord.h"
#include "Control/Command/Records/CFinishProcessCommandRecord.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;
	using namespace Logger;
	using namespace Concurrent;
	using namespace Control::Command;
	using namespace Control::Command::Records;
	using namespace Control::Command::Instructions;

	namespace Test {

		namespace Evaluation {

			using namespace Evaluation::Events;


			/*! 
			 *
			 *		\class		CReasonerEvaluationManagerThread
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationManagerThread : public CThread, public CReasonerEvaluationManager, public CLogIdentifier {

				// public methods
				public:
					//! Constructor
					CReasonerEvaluationManagerThread(CConfiguration *config);

					//! Destructor
					virtual ~CReasonerEvaluationManagerThread();


					virtual CReasonerEvaluationManager* realizeEvaluationCommand(CEvaluateTestsuiteCommand* evalTestsuiteComm);


				// protected methods
				protected:
					virtual bool processCustomsEvents(QEvent::Type type, CCustomEvent* event);

					bool evaluationProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm);
					CReasonerEvaluationManager* analysingProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm);
					bool analyseTimes(const QString& analysingDirectoryString, const QString& evaluationProgramName, const QString& plattform, const QString& analyserOutputDirectoryName, const QString& requestDirectory, 
							cint64 testCountCut, cint64 timeoutCut, cint64 errorPunishmentTime, CReasonerEvaluationDataValueCacher* dataValueCacher, CReasonerEvaluationExtractor::EXTRACTORTYPE extractorType, 
							const QString& extractionName, const QString& contextSpecificationName);

					CReasonerEvaluationManager* initEvaluationProgram(CEvaluateTestsuiteCommand* evalTestsuiteComm);



					CReasonerEvaluationGroupRequestSelector* getSelectors(const QString& requestDirecroty);


					QString getAnalyserOutputDirectory(const QString& baseDirectory, const QString& evaluationProgramName, const QString& platformName, const QString& analyserName);


				// protected variables
				protected:
					CConfiguration* mConfig;

					QStringList mReasonerNameStringList;
					QStringList mReasonerOutputDirStringList;

					cint64 mTestTimeout;
					cint64 mTestCount;

					QString mSatisticBasePath;
					QStringList mFailedReasonerStringList;
					QStringList mFitlerStringList;
					CReasonerEvaluationFiltering* mFiltering;


					bool mOneReasonerFailed;
					bool mAllReasonerFailed;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONMANAGERTHREAD_H
