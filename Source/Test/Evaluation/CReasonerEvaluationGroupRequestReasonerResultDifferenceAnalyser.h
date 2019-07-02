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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTDIFFERENCEANALYSER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTDIFFERENCEANALYSER_H

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


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Parser;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser : public CReasonerEvaluationGroupRequestReasonerComparisonAnalyser {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser();

					//! Destructor
					virtual ~CReasonerEvaluationGroupRequestReasonerResultDifferenceAnalyser();




				// protected methods
				protected:
					virtual bool analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector);

					QString getPrettyTitleFromOutputFile(const QString& outputFile);

					cint64 compareResultsSimilarityWithOtherReasoners(const QString& groupString, const QString& requestString, const QString& reasonerString, CReasonerEvaluationStringListValue* stringListDataValue, const QStringList& reasonerList);

					bool isClassHierarchyResultSimilarTo(CClassHierarchyResult* classHierResult, cint64 nodeNumber, const QString& otherResponseFileString);
					bool isBooleanResultSimilarTo(CBooleanQueryResult* booleanResult, cint64 nodeNumber, const QString& otherResponseFileString);
					bool isClassesResultSimilarTo(CClassesResult* classesResult, cint64 nodeNumber, const QString& otherResponseFileString);

				// protected variables
				protected:
					QString mOutputFileName;
					COWLlinkQtXMLResultParser mResultParser;
					QHash<cint64, QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64>* > mNumberCompareDiffCountHashHash;
					QHash<QPair<CReasonerEvaluationStringListValue*,CReasonerEvaluationStringListValue*>,cint64> mReasonerCompareDiffCountHash;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERRESULTDIFFERENCEANALYSER_H
