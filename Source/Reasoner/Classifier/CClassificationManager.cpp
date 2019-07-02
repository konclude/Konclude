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

#include "CClassificationManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationManager::CClassificationManager() {
			}


			CClassificationManager::~CClassificationManager() {
				if (mClassifierFac) {
					delete mClassifierFac;
				}
			}


			cint64 CClassificationManager::getActiveClassifierCount() {
				readWriteLock.lockForRead();
				cint64 activeCount = 0;
				foreach (CSubsumptionClassifier* classifier, mOntoClassifierSet) {
					CSubsumptionClassifierThread* threadClassifier = dynamic_cast<CSubsumptionClassifierThread*>(classifier);
					if (threadClassifier && threadClassifier->isClassifierActive()) {
						++activeCount;
					}
				}
				readWriteLock.unlock();
				return activeCount;
			}


			CClassificationManager *CClassificationManager::initializeManager(CSubsumptionClassifierFactory *takeClassifierFactory, CConfigurationProvider *configurationProvider) {
				mClassifierFac = takeClassifierFactory;
				return this;
			}

			
			CSubsumptionClassifier *CClassificationManager::getClassClassifier(CConcreteOntology *ontology, CConfigurationBase *config, bool create, bool backgroundClassification) {
				readWriteLock.lockForRead();
				CSubsumptionClassifier *classifier = mOntoClassifierHash.value(ontology);
				readWriteLock.unlock();
				if (!classifier) {
					readWriteLock.lockForWrite();
					classifier = mOntoClassifierHash.value(ontology);
					if (!classifier) {
						classifier = mClassifierFac->createClassifier(ontology,config);
						mOntoClassifierSet.insert(classifier);
						mOntoClassifierHash.insert(ontology,classifier);
					}
					readWriteLock.unlock();
				}
				return classifier;
			}

			CClassifierStatistics *CClassificationManager::collectClassificationStatistics(CClassifierStatistics *statistics) {
				statistics->resetValues();
				readWriteLock.lockForRead();
				foreach (CSubsumptionClassifier *classifier, mOntoClassifierSet) {
					CClassifierStatistics *classifierStatistics = classifier->getClassificationStatistics();
					statistics->appendStatistics(classifierStatistics);
				}
				readWriteLock.unlock();
				return statistics;
			}

			QList<CSubsumptionClassifier *> CClassificationManager::getClassifierList() {
				readWriteLock.lockForRead();
				QList<CSubsumptionClassifier *> list(mOntoClassifierSet.values());
				readWriteLock.unlock();
				return list;
			}

			CClassificationProgress* CClassificationManager::getClassificationProgress() {
				CClassificationProgress newClassProg;
				readWriteLock.lockForRead();
				double percentAvg = 0;
				cint64 percentCount = 0;
				foreach (CSubsumptionClassifier *classifier, mOntoClassifierSet) {
					CClassificationProgress* classificationProgress = classifier->getClassificationProgress();
					if (classificationProgress) {
						newClassProg.setTotalClasses(newClassProg.getTotalClasses()+classificationProgress->getTotalClasses());
						newClassProg.setClassificationCount(newClassProg.getClassificationCount()+classificationProgress->getClassificationCount());
						newClassProg.setTestedClasses(newClassProg.getTestedClasses()+classificationProgress->getTestedClasses());
						newClassProg.setTestedSubsumptions(newClassProg.getTestedSubsumptions()+classificationProgress->getTestedSubsumptions());
						newClassProg.setTotalSubsumptions(newClassProg.getTotalSubsumptions()+classificationProgress->getTotalSubsumptions());
						newClassProg.setRemainingMilliSeconds(qMax(newClassProg.getRemainingMilliSeconds(),classificationProgress->getRemainingMilliSeconds()));
						percentAvg += classificationProgress->getProgessPercent();
						++percentCount;
					}
				}
				readWriteLock.unlock();
				if (percentCount != 0) {
					percentAvg /= (double)percentCount;
				}
				newClassProg.setProgessPercent(percentAvg);
				mClassificationProgress = newClassProg;
				return &mClassificationProgress;
			}



			CSubsumptionClassifier* CClassificationManager::getDataPropertyClassifier(CConcreteOntology *ontology, CConfigurationBase *config) {
				readWriteLock.lockForRead();
				CSubsumptionClassifier *classifier = mOntoDataPropertyClassifierHash.value(ontology);
				readWriteLock.unlock();
				if (!classifier) {
					readWriteLock.lockForWrite();
					classifier = mOntoDataPropertyClassifierHash.value(ontology);
					if (!classifier) {
						classifier = mClassifierFac->getDataPropertyClassifier(ontology,config);
						mOntoClassifierSet.insert(classifier);
						mOntoDataPropertyClassifierHash.insert(ontology,classifier);
					}
					readWriteLock.unlock();
				}
				return classifier;
			}

			CSubsumptionClassifier* CClassificationManager::getObjectPropertyClassifier(CConcreteOntology *ontology, CConfigurationBase *config) {
				readWriteLock.lockForRead();
				CSubsumptionClassifier *classifier = mOntoObjectPropertyClassifierHash.value(ontology);
				readWriteLock.unlock();
				if (!classifier) {
					readWriteLock.lockForWrite();
					classifier = mOntoObjectPropertyClassifierHash.value(ontology);
					if (!classifier) {
						classifier = mClassifierFac->getObjectPropertyClassifier(ontology,config);
						mOntoClassifierSet.insert(classifier);
						mOntoObjectPropertyClassifierHash.insert(ontology,classifier);
					}
					readWriteLock.unlock();
				}
				return classifier;
			}




		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
