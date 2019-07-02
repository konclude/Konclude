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

#include "COntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			COntologyClassificationItem::COntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) {
				statistics = new CClassifierStatistics(nextClassificationStatistics);
				config = new CCalculationConfigurationExtension(configuration,0);

				mClassificationStatCollStrings = new CClassificationStatisticsCollectionStrings();

				mConfCollectProcessStatistics = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.CollectProcessStatistics",true);;
				
				mClassifyProcessingStepData = nullptr;

				mIndiDepTrackingCollector = nullptr;

				mInitTime.start();
			}


			COntologyClassificationItem::~COntologyClassificationItem() {
				delete statistics;
				delete config;
				delete mIndiDepTrackingCollector;
			}




			CConcreteOntology *COntologyClassificationItem::getOntology() {
				return onto;
			}

			
			CCalculationConfigurationExtension *COntologyClassificationItem::getCalculationConfiguration() {
				return config;
			}



			COntologyClassificationItem *COntologyClassificationItem::incCurrentCalculatingCount(qint64 incCount) {
				currentCalculatingCount += incCount;
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::decCurrentCalculatingCount(qint64 decCount) {
				currentCalculatingCount -= decCount;
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::setHasRemainingTests(bool moreTests) {
				remainingTests = moreTests;
				return this;
			}
			
			bool COntologyClassificationItem::hasRemainingTests() {
				return remainingTests || currentCalculatingCount > 0;
			}

			qint64 COntologyClassificationItem::getCurrentCalculatingCount() {
				return currentCalculatingCount;
			}


			COntologyClassificationItem *COntologyClassificationItem::doClassifiedCallback() {
				classifiedCallbackList.doCallback();
				return this;
			}

			COntologyClassificationItem *COntologyClassificationItem::addClassifiedCallback(CCallbackData *callback) {
				callback->setCallbackDataContext(new CClassifiedCallbackDataContext(onto));
				classifiedCallbackList.addCallbackData(callback);
				if (!hasRemainingTests()) {
					doClassifiedCallback();
				}
				return this;
			}

			qint64 COntologyClassificationItem::getCalcedSatisfiableCount() {
				return statistics->getTestedSatisfiableCount();
			}

			qint64 COntologyClassificationItem::getCalcedSubsumptionCount() {
				return statistics->getTestedSubsumptionCount();
			}

			CClassifierStatistics *COntologyClassificationItem::getClassifierStatistics() {
				return statistics;
			}


			CClassificationStatisticsCollectionStrings* COntologyClassificationItem::getClassificationStatisticsCollectionStrings() {
				return mClassificationStatCollStrings;
			}


			CClassificationCalculationStatisticsCollection* COntologyClassificationItem::getCalculationStatisticsCollection() {
				CClassificationCalculationStatisticsCollection* statColl = nullptr;
				if (!mReuseStatCollList.isEmpty()) {
					statColl = mReuseStatCollList.takeFirst();
				}
				if (!statColl && mClassificationStatCollStrings) {
					statColl = mClassificationStatCollStrings->createCalculationStatisticsCollection();
				}
				return statColl;
			}

			COntologyClassificationItem* COntologyClassificationItem::reuseCalculationStatisticsCollection(CClassificationCalculationStatisticsCollection* statColl) {
				mReuseStatCollList.append(statColl);
				return this;
			}



			bool COntologyClassificationItem::isCollectProcessStatisticsActivated() {
				return mConfCollectProcessStatistics;
			}

			QList<COntologyProcessingRequirement*> COntologyClassificationItem::getOntologyProcessingRequirementList() {
				return mRequirementList;
			}

			COntologyClassificationItem* COntologyClassificationItem::addOntologyProcessingRequirements(const QList<COntologyProcessingRequirement*>& reqList) {
				mRequirementList.append(reqList);
				return this;
			}



			CIndividualDependenceTrackingCollector* COntologyClassificationItem::getIndividualDependenceTrackingCollector() {
				return mIndiDepTrackingCollector;
			}


			COntologyClassificationItem* COntologyClassificationItem::setIndividualDependenceTrackingCollector(CIndividualDependenceTrackingCollector* indiDepTrackColl) {
				mIndiDepTrackingCollector = indiDepTrackColl;
				return this;
			}

			COntologyProcessingStepData* COntologyClassificationItem::getClassificationProcessingStep() {
				return mClassifyProcessingStepData;
			}


			QTime* COntologyClassificationItem::getInitializationTime() {
				return &mInitTime;
			}

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
