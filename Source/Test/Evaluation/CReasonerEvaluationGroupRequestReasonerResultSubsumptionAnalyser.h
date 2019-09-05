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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTSUBSUMPTIONANALYSER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTSUBSUMPTIONANALYSER_H

// Libraries includes
#include <QMap>
#include <QSet>
#include <QFile>
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGNUPlotPlotter.h"
#include "CReasonerEvaluationHighchartPlotter.h"
#include "CReasonerEvaluationAnalyseContext.h"
#include "CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"
#include "CReasonerEvaluationDataValueGroupCollectionReasonerComparison.h"
#include "CReasonerEvaluationDoubleDataValue.h"
#include "CReasonerEvaluationTableMultiFormatOutputWriter.h"
#include "CReasonerEvaluationStringListValue.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Parser/COWLlinkQtXMLResultParser.h"
#include "Parser/CSPARQLQtXMLResultParser.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Parser;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser : public CReasonerEvaluationGroupRequestReasonerComparisonAnalyser {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser();

					//! Destructor
					virtual ~CReasonerEvaluationGroupRequestReasonerResultSubsumptionAnalyser();




				// protected methods
				protected:
					virtual bool analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector);

					QString getPrettyTitleFromOutputFile(const QString& outputFile);

					cint64 compareResultsSubsumptionWithOtherReasoners(const QString& groupString, const QString& requestString, const QString& reasonerString, CReasonerEvaluationStringListValue* stringListDataValue, const QStringList& reasonerList, const QString& detailedReportingOutputDirectory);

					bool isVariableBindingResultSubsuming(CVariableBindingsAnswersSetResult* varBindingResult, cint64 nodeNumber, const QString& otherResponseFileString, QHash<QString, cint64>* variableNameIndexHash);

				// protected variables
				protected:
					QString mOutputFileName;
					CSPARQLQtXMLResultParser mSPARQLResultParser;
					QHash<cint64, QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>* > mNumberCompareDiffCountHashHash;
					QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64> mReasonerCompareResultSubsumptionCountHash;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTSUBSUMPTIONANALYSER_H
