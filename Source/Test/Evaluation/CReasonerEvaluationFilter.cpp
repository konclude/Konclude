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

#include "CReasonerEvaluationFilter.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationFilter::CReasonerEvaluationFilter() {
			}


			bool CReasonerEvaluationFilter::loadFilterConditionFile(const QString& filterNameString) {
				QFile filterFile(filterNameString);
				if (filterFile.open(QIODevice::ReadOnly)) {
					while (!filterFile.atEnd()) {
						QByteArray filterLine(filterFile.readLine());
						QStringList filterDataList = QString(filterLine).trimmed().split("\t");
						if (filterDataList.count() >= 4) {
							CReasonerEvaluationFilterCondition* statExtCon = nullptr;
							QString filterTypeString = filterDataList.takeFirst().toUpper();
							QString filterNameString = filterDataList.takeFirst();
							QString filterComperatorString = filterDataList.takeFirst();
							QString filterCompareValueString = filterDataList.takeFirst();
							if (filterTypeString == "INTEGER") {
								statExtCon = new CReasonerEvaluationFilterIntegerCondition(filterNameString,filterComperatorString,filterCompareValueString);
							} else if (filterTypeString == "STRING") {
								statExtCon = new CReasonerEvaluationFilterStringCondition(filterNameString,filterComperatorString,filterCompareValueString);
							}
							if (statExtCon) {
								mCondList.append(statExtCon);
							}
						}
					}
					filterFile.close();
					return true;
				}
				return false;
			}




			bool CReasonerEvaluationFilter::testSatisfiesFilterConditions(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList) {
				QStringList extStatisticList;
				extStatisticList.append(QString("Path:\t")+testFilePathString+subDirPathString);
				extStatisticList.append(QString("RelativeTestPath:\t")+subDirPathString);
				extStatisticList.append(QString("BaseTestPath:\t")+testFilePathString);
				extStatisticList.append(QString("FileName:\t")+testFileNameString);
				extStatisticList.append(QString("PathFileName:\t")+testFilePathString+subDirPathString+testFileNameString);
				extStatisticList.append(statisticList);
				return testSatisfiesFilterConditions(extStatisticList);
			}


			bool CReasonerEvaluationFilter::testSatisfiesFilterConditions(const QString& testFileNameString, const QStringList& statisticList) {
				QStringList extStatisticList;
				QString fileNameString = testFileNameString;
				QString pathString;
				cint64 lastSlash = testFileNameString.lastIndexOf("/");
				if (!lastSlash) {
					pathString = testFileNameString.mid(0,lastSlash);
					fileNameString = testFileNameString.mid(lastSlash+1);
				}
				extStatisticList.append(QString("Path:\t")+pathString);
				extStatisticList.append(QString("FileName:\t")+fileNameString);
				extStatisticList.append(QString("PathFileName:\t")+testFileNameString);
				extStatisticList.append(statisticList);
				return testSatisfiesFilterConditions(extStatisticList);
			}


			bool CReasonerEvaluationFilter::testSatisfiesFilterConditions(const QStringList& statisticList) {
				foreach (CReasonerEvaluationFilterCondition* extrCond, mCondList) {
					if (!extrCond->isStatisticConditionSatisfied(statisticList)) {
						return false;
					}
				}
				return true;
			}



		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
