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

#include "CReasonerEvaluationGroupRequestReasonerComparisonAnalyser.h"


namespace Konclude {

	namespace Test {

		namespace Evaluation {



			CReasonerEvaluationGroupRequestReasonerComparisonAnalyser::CReasonerEvaluationGroupRequestReasonerComparisonAnalyser() {
			}


			CReasonerEvaluationGroupRequestReasonerComparisonAnalyser::~CReasonerEvaluationGroupRequestReasonerComparisonAnalyser() {
			}

			CReasonerEvaluationAnalyser* CReasonerEvaluationGroupRequestReasonerComparisonAnalyser::analyseEvaluationData(CReasonerEvaluationDataValueGroupCollectionReasonerComparison* evaluationData, CReasonerEvaluationAnalyseContext* context, const QString& outputDirectory, CReasonerEvaluationGroupRequestSelector* selectors) {

				QStringList reasonerList;
				QStringList groubList;
				QStringList requestList;

				QSet<QString> groupStringSet;
				QSet<QString> requestStringSet;

				mGroupRequestSetHash.clear();

				mReasonerCollectionMap = evaluationData->getReasonerCollectionMap();
				for (QMap<QString,CReasonerEvaluationDataValueGroupCollection*>::const_iterator it1 = mReasonerCollectionMap->constBegin(), it1End = mReasonerCollectionMap->constEnd(); it1 != it1End; ++it1) {
					QString reasonerString(it1.key());
					reasonerList.append(reasonerString);


					CReasonerEvaluationDataValueGroupCollection* groupCollection(it1.value());
					QMap<QString,CReasonerEvaluationDataValueGroup*>* nameGroupMap = groupCollection->getNameGroupMap();
					for (QMap<QString,CReasonerEvaluationDataValueGroup*>::const_iterator it2 = nameGroupMap->constBegin(), it2End = nameGroupMap->constEnd(); it2 != it2End; ++it2) {
						QString groupString(it2.key());
						groupStringSet.insert(groupString);

						QSet<QString>* groupRequestSet = mGroupRequestSetHash.value(groupString);
						if (!groupRequestSet) {
							groupRequestSet = new QSet<QString>();
							mRequestSetContainer.append(groupRequestSet);
							mGroupRequestSetHash.insert(groupString,groupRequestSet);
						}

						CReasonerEvaluationDataValueGroup* group(it2.value());
						QMap<QString,CReasonerEvaluationDataValue*>* requestDataValueMap = group->getRequestDataValueMap();
						for (QMap<QString,CReasonerEvaluationDataValue*>::const_iterator it3 = requestDataValueMap->constBegin(), it3End = requestDataValueMap->constEnd(); it3 != it3End; ++it3) {
							QString requestString(it3.key());
							requestStringSet.insert(requestString);
							groupRequestSet->insert(requestString);
						}
					}
				}

				for (QSet<QString>::const_iterator it1 = groupStringSet.constBegin(), it1End = groupStringSet.constEnd(); it1 != it1End; ++it1) {
					groubList.append(*it1);
				}
				for (QSet<QString>::const_iterator it1 = requestStringSet.constBegin(), it1End = requestStringSet.constEnd(); it1 != it1End; ++it1) {
					requestList.append(*it1);
				}

				for (CReasonerEvaluationGroupRequestSelector* selectorsIt = selectors; selectorsIt; selectorsIt = selectorsIt->getNext()) {
					analyseGroupedEvaluationData(groubList,requestList,reasonerList,outputDirectory,context,selectorsIt);
				}

				qDeleteAll(mRequestSetContainer);

				return this;
			}


			QSet<QString>* CReasonerEvaluationGroupRequestReasonerComparisonAnalyser::getGroupRequests(const QString& groupName) {
				return mGroupRequestSetHash.value(groupName);
			}



			CReasonerEvaluationDataValue* CReasonerEvaluationGroupRequestReasonerComparisonAnalyser::getGroupRequestReasonerEvaluationData(const QString& groupName, const QString& requstName, const QString& reasonerName) {
				CReasonerEvaluationDataValue* evaluationData = nullptr;
				CReasonerEvaluationDataValueGroupCollection* groupCollection = mReasonerCollectionMap->value(reasonerName);
				if (groupCollection) {
					QMap<QString,CReasonerEvaluationDataValueGroup*>* nameGroupMap = groupCollection->getNameGroupMap();
					if (nameGroupMap) {
						CReasonerEvaluationDataValueGroup* group = nameGroupMap->value(groupName);
						if (group) {
							QMap<QString,CReasonerEvaluationDataValue*>* requestDataValueMap = group->getRequestDataValueMap();
							if (requestDataValueMap) {
								evaluationData = requestDataValueMap->value(requstName);
							}
						}
					}
				}
				return evaluationData;
			}


		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude
