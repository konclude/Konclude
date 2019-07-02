/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser::CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser() {
				}



				bool CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}



				bool CSatisfiableTaskSaturationSubsumptionIdentifierAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualProcessNodeVector* indiNodeVector = processingDataBox->getIndividualProcessNodeVector();
					cint64 indiNodeCount = indiNodeVector->getItemCount();
					for (cint64 indiID = 0; indiID < indiNodeCount; ++indiID) {
						CIndividualProcessNode* indiNode = indiNodeVector->getData(indiID);
						CConceptDescriptor* initConDes = indiNode->getIndividualInitializationConcept();
						if (initConDes) {
							CConcept* initConcept = initConDes->getConcept();
							if (initConcept->hasClassName()) {
								CConceptProcessData* conProData = (CConceptProcessData*)initConcept->getConceptData();
								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
								CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
								COptimizedClassSaturationSatisfiableTestingItem* classSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)classSatCalcRefLinkData;
								if (classSatItem) {
									CReapplyConceptLabelSet* conSet = indiNode->getReapplyConceptLabelSet(false);
									CConceptDescriptor* conDesIt = conSet->getAddingSortedConceptDescriptionLinker();
									while (conDesIt) {
										if (!conDesIt->isNegated()) {
											CConcept* subsumerConcept = conDesIt->getConcept();
											if (subsumerConcept != initConcept && subsumerConcept->hasClassName()) {
												CConceptProcessData* subsumerConProData = (CConceptProcessData*)subsumerConcept->getConceptData();
												CConceptSaturationReferenceLinkingData* subsumerConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)subsumerConProData->getConceptReferenceLinking();
												CClassificationConceptReferenceLinking* subsumerClassSatCalcRefLinkData = subsumerConfSatRefLinkingData->getClassifierReferenceLinkingData();
												if (subsumerClassSatCalcRefLinkData) {
													COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)subsumerClassSatCalcRefLinkData;
													classSatItem->addSubsumingConceptItem(subsumerClassSatItem);
												}

											}
										}
										conDesIt = conDesIt->getNext();
									}

									CIndividualProcessNode* substituiteNode = indiNode->getSubstituteIndividualNode();
									while (substituiteNode) {
										CReapplyConceptLabelSet* subsConSet = substituiteNode->getReapplyConceptLabelSet(false);
										CConceptDescriptor* subsConDesIt = subsConSet->getAddingSortedConceptDescriptionLinker();
										while (subsConDesIt) {
											if (!subsConDesIt->isNegated()) {
												CConcept* subsumerConcept = subsConDesIt->getConcept();
												if (subsumerConcept != initConcept && subsumerConcept->hasClassName()) {
													CConceptProcessData* subsumerConProData = (CConceptProcessData*)subsumerConcept->getConceptData();
													CConceptSaturationReferenceLinkingData* subsumerConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)subsumerConProData->getConceptReferenceLinking();
													CClassificationConceptReferenceLinking* subsumerClassSatCalcRefLinkData = subsumerConfSatRefLinkingData->getClassifierReferenceLinkingData();
													if (subsumerClassSatCalcRefLinkData) {
														COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)subsumerClassSatCalcRefLinkData;
														classSatItem->addSubsumingConceptItem(subsumerClassSatItem);
													}

												}
											}
											subsConDesIt = subsConDesIt->getNext();
										}

										substituiteNode = substituiteNode->getSubstituteIndividualNode();
									}
								}
							}
						}
					}
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
