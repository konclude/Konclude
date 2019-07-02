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

#include "CConfigDependedSubsumptionClassifierFactory.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CConfigDependedSubsumptionClassifierFactory::CConfigDependedSubsumptionClassifierFactory(CReasonerManager *reasonerManager) {
				reasonerMan = reasonerManager;
			}


			CConfigDependedSubsumptionClassifierFactory::~CConfigDependedSubsumptionClassifierFactory() {
			}

			CSubsumptionClassifier *CConfigDependedSubsumptionClassifierFactory::createClassifier(CConcreteOntology *ontology, CConfigurationBase *config) {
				CSubsumptionClassifier *classifier = 0;
				bool automatedClassifierSelection = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.AutomatedOptimizedClassifierSelection",true);
				if (automatedClassifierSelection) {
					if (isClassificationBySaturationCalculationSufficient(ontology,config)) {
						LOG(INFO,"::Konclude::Reasoner::Classifier::Factory",logTr("Ontology '%1' has EL structure, using tableau saturation for classification.").arg(ontology->getOntologyName()),this);
						classifier = new COptimizedClassExtractedSaturationSubsumptionClassifierThread(reasonerMan);
					} else if (isClassificationBySatisfiableCalculationSufficient(ontology,config)) {
						LOG(INFO,"::Konclude::Reasoner::Classifier::Factory",logTr("Ontology '%1' is deterministic, using satisfiable tests for classification.").arg(ontology->getOntologyName()),this);
						classifier = new COptimizedSubClassSubsumptionClassifierThread(reasonerMan);
					}
				}
				if (!classifier) {
					QString classifierName = CConfigDataReader::readConfigString(config,"Konclude.Calculation.Classification.Classifier","Konclude.Calculation.Classification.Classifier.OptimizedKPSetClassClassifier");

					if (classifierName == "Konclude.Calculation.Classification.Classifier.OptimizedClassSaturationClassifier") {
						classifier = new COptimizedClassSaturationSubsumptionClassifierThread(reasonerMan);
					} else if (classifierName == "Konclude.Calculation.Classification.Classifier.OptimizedSubClassClassifier") {
						classifier = new COptimizedSubClassSubsumptionClassifierThread(reasonerMan);
					} else if (classifierName == "Konclude.Calculation.Classification.Classifier.OptimizedKPSetClassClassifier") {
						classifier = new COptimizedKPSetClassSubsumptionClassifierThread(reasonerMan);
					} else if (classifierName == "Konclude.Calculation.Classification.Classifier.OptimizedClassExtractedSaturationClassifier") {
						classifier = new COptimizedClassExtractedSaturationSubsumptionClassifierThread(reasonerMan);
					}
				}
				return classifier;
			}

			CSubsumptionClassifier *CConfigDependedSubsumptionClassifierFactory::createBackgroundClassifier(CConcreteOntology *ontology, CConfigurationBase *config) {
				CSubsumptionClassifier *classifier = createClassifier(ontology,config);
				return classifier;
			}


			bool CConfigDependedSubsumptionClassifierFactory::isClassificationBySatisfiableCalculationSufficient(CConcreteOntology *ontology, CConfigurationBase *config) {
				COntologyStructureSummary* ontoStrSum = ontology->getStructureSummary();
				if (ontoStrSum) {
					return ontoStrSum->hasOnlyDeterministicConceptClasses();
				} else {
					return false;
				}
			}

			bool CConfigDependedSubsumptionClassifierFactory::isClassificationBySaturationCalculationSufficient(CConcreteOntology *ontology, CConfigurationBase *config) {

				bool subsumerSaturationExtraction = CConfigDataReader::readConfigBoolean(config,"Konclude.Calculation.Classification.SaturationSubsumerExtraction",true);
				if (!subsumerSaturationExtraction) {
					return false;
				}


				CSaturationData* satData = ontology->getPrecomputation()->getSaturationModelData();
				if (satData) {
					CSaturationTaskData* satTaskData = dynamic_cast<CSaturationTaskData*>(satData);
					if (satTaskData) {
						CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
						if (!satCalcTask->getProcessingDataBox()->isInsufficientNodeOccured() && !satCalcTask->getProcessingDataBox()->isProblematicEQCandidateOccured()) {
							CBOXSET<CConcept*>* eqConceptNonCandSet = ontology->getTBox()->getEquivalentConceptNonCandidateSet(false);
							if (!eqConceptNonCandSet || eqConceptNonCandSet->isEmpty()) {
								return true;
							}
						}
					}
				}
				return false;
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
