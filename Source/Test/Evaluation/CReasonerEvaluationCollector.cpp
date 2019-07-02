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

#include "CReasonerEvaluationCollector.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationCollector::CReasonerEvaluationCollector(CReasonerEvaluationExtractor* extractor, CReasonerEvaluationSummarizer* summarizer, CReasonerEvaluationFiltering* filtering) : CLogIdentifier("::Konclude::Test::Evaluation::Collector",this) {
				mExtractor = extractor;
				mSummarizer = summarizer;
				mFiltering = filtering;
			}


			CReasonerEvaluationCollector::~CReasonerEvaluationCollector() {
			}


			bool CReasonerEvaluationCollector::collectReasonerEvaluationDataValues(CReasonerEvaluationDataValueGroupCollectionReasonerComparison *reasonerComp, const QString& reasonerName, const QString& reasonerPath) {
				CReasonerEvaluationDataValueGroupCollection* valueGroupCollection = collectEvaluationDataValues(reasonerPath);
				reasonerComp->addReasonerCollection(reasonerName,valueGroupCollection);
				return true;
			}

			CReasonerEvaluationDataValueGroupCollection* CReasonerEvaluationCollector::collectEvaluationDataValues(const QString& baseDirectory) {
				CReasonerEvaluationDataValueGroupCollection* groupCollection = new CReasonerEvaluationDataValueGroupCollection();
				collectEvaluationDatas(baseDirectory,"",baseDirectory,groupCollection);
				return groupCollection;
			}

			bool CReasonerEvaluationCollector::collectEvaluationDatas(const QString& directory, const QString& subDirectory, const QString& baseDirectory, CReasonerEvaluationDataValueGroupCollection* groupCollection) {
				QDir dir(directory);
				CReasonerEvaluationDataValueGroup* group = new CReasonerEvaluationDataValueGroup();
				QStringList dirList = dir.entryList(QDir::Dirs,QDir::Name|QDir::IgnoreCase);
				foreach (QString dirString, dirList) {
					if (dirString != "." && dirString != "..") {
						QStringList statisticList;
						if (mFiltering) {
							statisticList = mFiltering->loadStatistics(dirString,subDirectory);
						}
						if (!mFiltering || mFiltering->testSatisfiesFilter(dirString,baseDirectory,subDirectory,statisticList)) {
							LOG(INFO,getLogDomain(),logTr("Extracting data for '%1'.").arg(dirString),this);
							collectGroupEvaluationDatas(directory+dirString+"/",dirString,group);
						} else {
							LOG(INFO,getLogDomain(),logTr("Data extracting for '%1' filtered out.").arg(dirString),this);
						}
					}
				}
				if (!group->isEmpty()) {
					QString groupName = directory;
					groupName.remove(0,baseDirectory.length());
					group->setGroupName(groupName);
					groupCollection->addNameGroupEvaluationData(groupName,group);
				} else {
					delete group;
				}
				foreach (QString dirString, dirList) {
					if (dirString != "." && dirString != "..") {
						collectEvaluationDatas(directory+dirString+"/",subDirectory+dirString+"/",baseDirectory,groupCollection);
					}
				}
				return true;
			}


			bool CReasonerEvaluationCollector::collectGroupEvaluationDatas(const QString& directory, const QString& requestFileString, CReasonerEvaluationDataValueGroup* group) {
				QDir dir(directory);
				QStringList fileList = dir.entryList(QDir::Files,QDir::Name|QDir::IgnoreCase);
				CReasonerEvaluationDataValue* extractedEvalDataValue = nullptr;
				foreach (QString fileString, fileList) {
					QString totalFileString = directory+fileString;
					CReasonerEvaluationDataValue* tmpEvalDataValue = mExtractor->extractEvaluationData(totalFileString);
					if (tmpEvalDataValue) {
						extractedEvalDataValue = tmpEvalDataValue->append(extractedEvalDataValue);
					}
				}
				CReasonerEvaluationDataValue* evalDataValue = nullptr;
				if (extractedEvalDataValue) {
					evalDataValue = mSummarizer->summarizeEvaluationData(extractedEvalDataValue);
				}
				if (evalDataValue) {
					group->addRequestDataValue(requestFileString,evalDataValue);
					return true;
				}
				return false;
			}

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
