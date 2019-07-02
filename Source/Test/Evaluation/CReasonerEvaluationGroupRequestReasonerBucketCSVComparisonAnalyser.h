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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERBUCKETCSVCOMPARISONANALYSER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERBUCKETCSVCOMPARISONANALYSER_H

// Libraries includes
#include <QMap>
#include <QSet>
#include <QFile>
#include <QDir>
#include <qglobal.h>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"
#include "CReasonerEvaluationDataValueGroupCollectionReasonerComparison.h"
#include "CReasonerEvaluationDoubleDataValue.h"
#include "CReasonerEvaluationTableMultiFormatOutputWriter.h"
#include "CReasonerEvaluationHighchartPlotter.h"


// Other includes
#include "Config/CConfiguration.h"

#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;
	using namespace Utilities;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser : public CReasonerEvaluationGroupRequestReasonerComparisonAnalyser {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestReasonerBucketCSVComparisonAnalyser(double bucketsStart, double bucketsEnd, double bucketsIncreaseFactor, double timeoutsEnd);

				// protected methods
				protected:
					virtual bool analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector);
					QString getPrettyTitleFromOutputFile(const QString& outputFile);

				// protected variables
				protected:
					QString mOutputFileName;

					double mBucketsStart;
					double mBucketsEnd;
					double mBucketsIncreaseFactor;

					double mTimeoutsEnd;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTREASONERBUCKETCSVCOMPARISONANALYSER_H
