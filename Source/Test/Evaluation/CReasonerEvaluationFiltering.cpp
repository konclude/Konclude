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

#include "CReasonerEvaluationFiltering.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationFiltering::CReasonerEvaluationFiltering(const QString& statisticsBasePath) : CLogIdentifier("::Konclude::Test::Evaluation::Filtering",this) {
				mStatisticsBasePath = statisticsBasePath;
			}

			bool CReasonerEvaluationFiltering::loadFilterFromFile(const QString& filterNameString) {
				CReasonerEvaluationFilter* filter = new CReasonerEvaluationFilter();
				LOG(INFO,getLogDomain(),logTr("Loading filter from file '%1'.").arg(filterNameString),this);
				if (filter->loadFilterConditionFile(filterNameString)) {
					mFilterList.append(filter);
					LOG(INFO,getLogDomain(),logTr("Successfully loaded filter from file '%1'.").arg(filterNameString),this);
					return true;
				} else {
					delete filter;
					LOG(ERROR,getLogDomain(),logTr("Loading filter from file '%1' failed.").arg(filterNameString),this);
					return false;
				}
			}


			bool CReasonerEvaluationFiltering::loadEmptyFilter() {
				CReasonerEvaluationFilter* filter = new CReasonerEvaluationFilter();
				mFilterList.append(filter);
				return true;
			}


			bool CReasonerEvaluationFiltering::loadFiltersFromFile(const QStringList& filterNameStringList) {
				bool loadedOne = false;
				foreach (const QString& fileName, filterNameStringList) {
					loadedOne |= loadFilterFromFile(fileName);
				}
				return loadedOne;
			}


			bool CReasonerEvaluationFiltering::testSatisfiesFilter(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList) {
				foreach (CReasonerEvaluationFilter* filter, mFilterList) {
					if (filter->testSatisfiesFilterConditions(testFileNameString,testFilePathString,subDirPathString,statisticList)) {
						return true;
					}
				}
				return false;
			}


			bool CReasonerEvaluationFiltering::testSatisfiesFilter(const QStringList& statisticList) {
				foreach (CReasonerEvaluationFilter* filter, mFilterList) {
					if (filter->testSatisfiesFilterConditions(statisticList)) {
						return true;
					}
				}
				return false;
			}



			bool CReasonerEvaluationFiltering::testSatisfiesFilter(const QString& testFileNameString, const QStringList& statisticList) {
				foreach (CReasonerEvaluationFilter* filter, mFilterList) {
					if (filter->testSatisfiesFilterConditions(testFileNameString,statisticList)) {
						return true;
					}
				}
				return false;
			}



			QStringList CReasonerEvaluationFiltering::loadStatistics(const QString& testFileNameString, const QString& subDirPathString) {
				return loadStatistics(subDirPathString+testFileNameString);
			}

			QStringList CReasonerEvaluationFiltering::loadStatistics(const QString& subDirPathTestFileNameString) {
				QString useStatisticsBasePath = mStatisticsBasePath;
				QStringList statisticList;
				QString statisticFileString = useStatisticsBasePath+subDirPathTestFileNameString+".txt";
				QFile statisticFile(statisticFileString);
				if (statisticFile.open(QIODevice::ReadOnly)) {
					while (!statisticFile.atEnd()) {
						QByteArray statDataLine(statisticFile.readLine());
						statisticList.append(statDataLine.trimmed());
					}
					statisticFile.close();
				} else {
					//LOG(INFO,getLogDomain(),logTr("Could not open statistics file '%1'.").arg(statisticFileString),this);
				}
				return statisticList;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
