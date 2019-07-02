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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSTATISTICSCSVCOMPARISONANALYSER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSTATISTICSCSVCOMPARISONANALYSER_H

// Libraries includes
#include <QMap>
#include <QSet>
#include <QFile>
#include <QDir>
#include <qglobal.h>
#include <qmath.h>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGNUPlotPlotter.h"
#include "CReasonerEvaluationHighchartPlotter.h"
#include "CReasonerEvaluationAnalyseContext.h"
#include "CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"
#include "CReasonerEvaluationDataValueGroupCollectionReasonerComparison.h"
#include "CReasonerEvaluationDoubleDataValue.h"
#include "CReasonerEvaluationTableMultiFormatOutputWriter.h"
#include "CReasonerEvaluationFiltering.h"
#include "CReasonerEvaluationStringListValue.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {



			class CRequestStatisticsData {
				public:
					CRequestStatisticsData() {
						mInitialized = false;
						mCalculated = false;
						mMinValue = DOUBLE_MAX;
						mMaxValue = DOUBLE_MIN;
						mSumValue = 0.;
						mMeanValue = 0.;
						mValueCount = 0.;
						mStandardDerivationValue = 0.;
						mMedianValue = 0.;
					}

					CRequestStatisticsData* setDataName(const QString& dataName) {
						mDataName = dataName;
						return this;
					}

					CRequestStatisticsData* addData(double value, const QString& requestString) {
						mDataRequestStringMap.insertMulti(value,requestString);
						return this;
					}

					bool calculateStatistics() {
						if (!mCalculated) {
							mCalculated = true;
							for (QMap<double,QString>::const_iterator it = mDataRequestStringMap.constBegin(), itEnd = mDataRequestStringMap.constEnd(); it != itEnd; ++it) {
								double value(it.key());
								mMinValue = qMin(mMinValue,value);
								mMaxValue = qMax(mMaxValue,value);
								mSumValue += value;
								mValueCount += 1;
							}
							if (mValueCount > 0) {
								mMeanValue = mSumValue/mValueCount;
								cint64 medianCounter = mValueCount/2.;
								for (QMap<double,QString>::const_iterator it = mDataRequestStringMap.constBegin(), itEnd = mDataRequestStringMap.constEnd(); it != itEnd; ++it) {
									double value(it.key());	
									mStandardDerivationValue += (value-mMeanValue)*(value-mMeanValue);									
									if (medianCounter >= 0) {
										mMedianValue = value;
										medianCounter = medianCounter-1.;
									}
								}
								mStandardDerivationValue = qSqrt(mStandardDerivationValue/(mValueCount+1.0));
							}
						}
						return mCalculated;
					}

				public:
					QMap<double,QString> mDataRequestStringMap;
					QString mDataName;
					bool mInitialized;
					bool mCalculated;
					double mMinValue;
					double mMaxValue;
					double mMeanValue;
					double mStandardDerivationValue;
					double mSumValue;
					double mMedianValue;
					double mValueCount;
			};


			/*! 
			 *
			 *		\class		CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser : public CReasonerEvaluationGroupRequestReasonerComparisonAnalyser {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser(CReasonerEvaluationFiltering* filtering);

					//! Destructor
					virtual ~CReasonerEvaluationGroupRequestStatisticsCSVComparisonAnalyser();



					QString getPrettyTitleFromOutputFile(const QString& outputFile);


				// protected methods
				protected:
					virtual bool analyseGroupedEvaluationData(const QStringList& groubList, const QStringList& requestList, const QStringList& reasonerList, const QString& outputDirectory, CReasonerEvaluationAnalyseContext* context, CReasonerEvaluationGroupRequestSelector* selector);

				// protected variables
				protected:
					CReasonerEvaluationFiltering* mFiltering;
					QString mOutputFileName;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONGROUPREQUESTSTATISTICSCSVCOMPARISONANALYSER_H
