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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERSTATISTICVALUECOLLECTINGCSVCOMPARISONANALYSER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERSTATISTICVALUECOLLECTINGCSVCOMPARISONANALYSER_H

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


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser : public CReasonerEvaluationGroupRequestReasonerComparisonAnalyser {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser();

					//! Destructor
					virtual ~CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser();


					CReasonerEvaluationGroupRequestReasonerStatisticValueCollectingCSVComparisonAnalyser* addNextStatisticName(const QString& statName);

					QString getPrettyTitleFromOutputFile(const QString& outputFile);


				// protected methods
				protected:
					virtual bool analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector);

				// protected variables
				protected:
					QString mOutputFileName;


					QHash< CReasonerEvaluationGroupRequestSelector*, QHash<QPair<QString,QString>,double>* > mSelectorReasonerRequestValueHash;
					QStringList mStatNameList;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERSTATISTICVALUECOLLECTINGCSVCOMPARISONANALYSER_H
