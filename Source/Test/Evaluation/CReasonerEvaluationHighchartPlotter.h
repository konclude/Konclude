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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHIGHCHARTPLOTTER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHIGHCHARTPLOTTER_H

// Libraries includes
#include <QProcess>

// Namespace includes
#include "EvaluationSettings.h"


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
			 *		\class		CReasonerEvaluationHighchartPlotter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationHighchartPlotter {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationHighchartPlotter(CReasonerEvaluationAnalyseContext* context);

					//! Destructor
					virtual ~CReasonerEvaluationHighchartPlotter();


					CReasonerEvaluationHighchartPlotter* createReasonerFasterPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerComparisonPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerDifferencePlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerErrorPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerAccumulatedPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerOverviewPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerAccumulatedCountPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);

					CReasonerEvaluationHighchartPlotter* createRequestStatisticsPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);

					CReasonerEvaluationHighchartPlotter* createReasonerWinnerCountPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerStackedBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);
					CReasonerEvaluationHighchartPlotter* createReasonerCumulativeBucketPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& selectionString, const QString& outputFile);


					CReasonerEvaluationHighchartPlotter* createReasonerComparisonSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QString& selectionString);
					CReasonerEvaluationHighchartPlotter* createReasonerFasterSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QString& selectionString);


				// protected methods
				protected:
					QMap<QString,cint64> loadReasonerStringIndexMap(const QString& dataFileString);
					QString loadTemplateFromFile(const QString& fileString);

					bool hasValidColorsForAllReasoenrs(const QStringList& reasonerList);

					bool loadReasonerComparisonData(const QString& dataFileString, QStringList& labelList, QMap<QString,QStringList>& valueListMap);

					QString getColorString(const QString& reasonerString, QSet<QString>* usedReasonerColorSet = nullptr);

					QString getFileNameWithoutDirectory(const QString& fileNameString);

					CReasonerEvaluationHighchartPlotter* createReasonerPlot(const QString& dataFileString, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QHash<QString,QString>& reasTemplateStringHash, const QString& selectionString, const QString& curveString = "type: 'column',");


					CReasonerEvaluationHighchartPlotter* sortAndCreateReasonerSplitPlot(const QStringList& dataFileStringList, const QStringList& plotTitleList, const QStringList& reasonerNameList, const QHash<QString,QString>& reasTemplateOverStringHash, const QHash<QString,QString>& reasTemplateContStringHash, const QString& selectionString);
					CReasonerEvaluationHighchartPlotter* createReasonerSplitPlot(const QStringList& dataFileStringList, const QString& plotTitle, const QStringList& reasonerNameList, const QString& outputFile, const QHash<QString,QString>& reasTemplateOverStringHash, const QHash<QString,QString>& reasTemplateContStringHash, const QString& selectionString);
					
					bool loadReasonerColorData(const QString& dataFileString, QHash<QString,QString>& reasColorHash);

				// protected variables
				protected:
					CReasonerEvaluationAnalyseContext* mContext;

					QString mReasCompTemplateString;
					QString mReasOverTemplateString;

					QHash<QString,QString> mReasCompTemplateStringHash;
					QHash<QString,QString> mReasFasterTemplateStringHash;
					QHash<QString,QString> mReasDiffTemplateStringHash;
					QHash<QString,QString> mReasErrorTemplateStringHash;
					QHash<QString,QString> mReasAccumTemplateStringHash;
					QHash<QString,QString> mReasOverviewTemplateStringHash;
					QHash<QString,QString> mReasWinnerTemplateStringHash;
					QHash<QString,QString> mReasStackedBucketTemplateStringHash;
					QHash<QString,QString> mReasBucketTemplateStringHash;
					QHash<QString,QString> mReasCumulativeBucketTemplateStringHash;
					QHash<QString,QString> mReasAccumCountTemplateStringHash;
					QHash<QString,QString> mRequestStatisticsTemplateStringHash;


					QHash<QString,QString> mReasCompSplitOverTemplateStringHash;
					QHash<QString,QString> mReasCompSplitContentTemplateStringHash;


					QHash<QString,QString> mReasFasterSplitOverTemplateStringHash;
					QHash<QString,QString> mReasFasterSplitContentTemplateStringHash;


					QHash<QString,QString> mReasColorHash;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHIGHCHARTPLOTTER_H
