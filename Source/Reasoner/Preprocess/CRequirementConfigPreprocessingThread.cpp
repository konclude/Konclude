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

#include "CRequirementConfigPreprocessingThread.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CRequirementConfigPreprocessingThread::CRequirementConfigPreprocessingThread(CReasonerManager *reasonerManager) : CPreprocessingThread(reasonerManager),CLogDomain("::Konclude::Reasoner::Kernel::Preprocessor") {
			}


			CRequirementConfigPreprocessingThread::~CRequirementConfigPreprocessingThread() {
			}


			COntologyPreprocessingItem* CRequirementConfigPreprocessingThread::initializeOntologyPreprocessingItem(CConcreteOntology* ontology, CConfigurationBase* config) {
				CRequirementConfigPreprocessingItem* item = new CRequirementConfigPreprocessingItem();
				item->initRequirementConfigPreprocessingItem(ontology,config);
				return item;
			}








			bool CRequirementConfigPreprocessingThread::createNextTest() {


				COntologyPreprocessingItem* loopOntPreCompItem = 0;
				bool workTestCreated = false;
				while (!workTestCreated && !mProcessingOntItemList.isEmpty()) {
					COntologyPreprocessingItem* ontPreCompItem = mProcessingOntItemList.first();

					if (ontPreCompItem == loopOntPreCompItem) {
						// don't run into infinite loop without doing something
						break;
					}

					CRequirementConfigPreprocessingItem* reqConfPreCompItem = (CRequirementConfigPreprocessingItem*)ontPreCompItem;



					bool failDebug = false;
					if (!workTestCreated && reqConfPreCompItem->isActiveCountingStepRequired()) {
						if (!reqConfPreCompItem->isActiveCountingStepFinished()) {
							if (reqConfPreCompItem->areActiveCountingStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Counting active entities."),this);
								countActiveEntites(reqConfPreCompItem);
								reqConfPreCompItem->getActiveCountingProcessingStep()->setStepFinished(true);
								reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate();
							} else {
								reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getActiveCountingProcessingStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated && reqConfPreCompItem->isBuildStepRequired()) {
						if (!reqConfPreCompItem->isBuildStepFinished()) {
							if (reqConfPreCompItem->areBuildStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Build intern ontology representation."),this);
								createBuild(reqConfPreCompItem);
								if (reqConfPreCompItem->failAfterBuilt()) {
									failDebug = true;
									reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
								} else {
									reqConfPreCompItem->getBuildProcessingStep()->setStepFinished(true);
									reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate();
								}
							} else {
								reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getBuildProcessingStep()->submitRequirementsUpdate();
						}
					}


					if (!workTestCreated && reqConfPreCompItem->isPreprocessStepRequired()) {
						if (!reqConfPreCompItem->isPreprocessStepFinished()) {
							if (reqConfPreCompItem->arePreprocessStepProcessingRequirementSatisfied()) {
								LOG(INFO,getDomain(),logTr("Preprocess ontology."),this);
								createPreprocess(reqConfPreCompItem);
								if (reqConfPreCompItem->failAfterPreprocessing()) {
									failDebug = true;
									reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED);
								} else {
									reqConfPreCompItem->getPreprocessProcessingStep()->setStepFinished(true);
									reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate();
								}
							} else {
								reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate(COntologyProcessingStatus::PSFAILED | COntologyProcessingStatus::PSFAILEDREQUIREMENT);
							}
						} else {
							reqConfPreCompItem->getPreprocessProcessingStep()->submitRequirementsUpdate();
						}
					}

					if (!workTestCreated) {
						if (!reqConfPreCompItem->hasRemainingProcessingRequirements()) {
							finishOntologyPreprocessing(reqConfPreCompItem);
							mProcessingOntItemList.removeFirst();
						} else {
							mProcessingOntItemList.removeFirst();
							mProcessingOntItemList.append(reqConfPreCompItem);
							loopOntPreCompItem = ontPreCompItem;
						}
					}
				}
				return workTestCreated;
			}



			bool CRequirementConfigPreprocessingThread::countActiveEntites(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				bool newAxiomsCounted = false;
				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				CExpressionDataBoxMapping* expDataBoxMapping = ontology->getDataBoxes()->getExpressionDataBoxMapping();
				CActiveEntityCountVector* activeEntityCountVector = expDataBoxMapping->getActiveEntityVector();
				activeEntityCountVector->increaseVectorSize(ontology->getBuildData()->getNextEntityNumber(false));
				CActiveEntitySubExpressionVisitorUpdater* activeEntityCountUpdater = new CActiveEntitySubExpressionVisitorUpdater(activeEntityCountVector);

				cint64 lastCountedAxiom = expDataBoxMapping->getLastActiveCountedEntityAxiom();
				CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* changeAxiomList = ontology->getBuildData()->getUpdatedChangeAxiomList();
				cint64 newAxiomCount = changeAxiomList->size();
				for (cint64 currAxiom = lastCountedAxiom; currAxiom < newAxiomCount; ++currAxiom) {
					const QPair<CAxiomExpression*,bool>& axiomChangePair((*changeAxiomList)[currAxiom]);
					bool axiomAdded = axiomChangePair.second;
					CAxiomExpression* axiom = axiomChangePair.first;
					activeEntityCountUpdater->updateEntitiesForAxioms(axiom,axiomAdded);
					newAxiomsCounted = true;
				}
				expDataBoxMapping->setLastActiveCountedEntityAxiom(newAxiomCount);

				delete activeEntityCountUpdater;
				return newAxiomsCounted;
			}


			bool CRequirementConfigPreprocessingThread::createBuild(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntologyUpdateBuilder upBuilder(reqConfPreCompItem->getOntology());
				upBuilder.initializeBuilding();
				upBuilder.completeBuilding();
				return false;
			}


			bool CRequirementConfigPreprocessingThread::createPreprocess(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				CConcreteOntology* ontology = reqConfPreCompItem->getOntology();
				CConfiguration* config = reqConfPreCompItem->getCalculationConfiguration();

				COntologyConfigDependedPreProcesser ontConDepPreProcessor;
				ontConDepPreProcessor.preprocess(ontology,config);
				return false;
			}


			bool CRequirementConfigPreprocessingThread::preprocessingTested(COntologyPreprocessingItem* ontPreCompItem, CPreprocessingTestingItem* preTestItem, CPreprocessingCalculatedCallbackEvent* pcce) {
				return false;
			}


			bool CRequirementConfigPreprocessingThread::finishOntologyPreprocessing(CRequirementConfigPreprocessingItem* reqConfPreCompItem) {
				if (reqConfPreCompItem->areAllStepFinished()) {
					reqConfPreCompItem->setPreprocessingFinished(true);
					CConcreteOntology* ontology = reqConfPreCompItem->getOntology();

					CPreprocessing* preprocessing = ontology->getPreprocessing();
					preprocessing->setPreprocessed(true);
				}

				mActiveOntItemSet.remove(reqConfPreCompItem);
				mInactiveOntItemSet.insert(reqConfPreCompItem);

				reqConfPreCompItem->doPreprocessingFinishedCallback(true);
				return true;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
