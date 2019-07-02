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

#include "CClassificationManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationManager::CClassificationManager() {
			}


			CClassificationManager::~CClassificationManager() {
				if (classifierFac) {
					delete classifierFac;
				}
			}


			cint64 CClassificationManager::getActiveClassifierCount() {
				readWriteLock.lockForRead();
				cint64 activeCount = 0;
				foreach (CSubsumptionClassifier* classifier, ontoClassifierSet) {
					CSubsumptionClassifierThread* threadClassifier = dynamic_cast<CSubsumptionClassifierThread*>(classifier);
					if (threadClassifier && threadClassifier->isClassifierActive()) {
						++activeCount;
					}
				}
				readWriteLock.unlock();
				return activeCount;
			}


			CClassificationManager *CClassificationManager::initializeManager(CSubsumptionClassifierFactory *takeClassifierFactory, CConfigurationProvider *configurationProvider) {
				classifierFac = takeClassifierFactory;
				return this;
			}

			
			CSubsumptionClassifier *CClassificationManager::getClassifier(CConcreteOntology *ontology, CConfigurationBase *config, bool create, bool backgroundClassification) {
				readWriteLock.lockForRead();
				CSubsumptionClassifier *classifier = ontoClassifierHash.value(ontology);
				readWriteLock.unlock();
				if (!classifier) {
					readWriteLock.lockForWrite();
					classifier = ontoClassifierHash.value(ontology);
					if (!classifier) {
						classifier = classifierFac->createClassifier(ontology,config);
						ontoClassifierSet.insert(classifier);
						ontoClassifierHash.insert(ontology,classifier);
					}
					readWriteLock.unlock();
				}
				return classifier;
			}

			CClassifierStatistics *CClassificationManager::collectClassificationStatistics(CClassifierStatistics *statistics) {
				statistics->resetValues();
				readWriteLock.lockForRead();
				foreach (CSubsumptionClassifier *classifier, ontoClassifierSet) {
					CClassifierStatistics *classifierStatistics = classifier->getClassificationStatistics();
					statistics->appendStatistics(classifierStatistics);
				}
				readWriteLock.unlock();
				return statistics;
			}

			QList<CSubsumptionClassifier *> CClassificationManager::getClassifierList() {
				readWriteLock.lockForRead();
				QList<CSubsumptionClassifier *> list(ontoClassifierSet.values());
				readWriteLock.unlock();
				return list;
			}

			CClassificationProgress* CClassificationManager::getClassificationProgress() {
				CClassificationProgress newClassProg;
				readWriteLock.lockForRead();
				double percentAvg = 0;
				cint64 percentCount = 0;
				foreach (CSubsumptionClassifier *classifier, ontoClassifierSet) {
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


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
