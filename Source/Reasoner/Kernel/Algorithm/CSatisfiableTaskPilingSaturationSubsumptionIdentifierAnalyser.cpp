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

#include "CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Algorithm {


				CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser::CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser() {
				}



				bool CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser::readCalculationConfig(CCalculationConfigurationExtension *config) {					
					if (config) {
						return true;
					}
					return false;
				}



				bool CSatisfiableTaskPilingSaturationSubsumptionIdentifierAnalyser::analyseSatisfiableTask(CSatisfiableCalculationTask* statCalcTask, CCalculationAlgorithmContext* calcAlgContext) {
					CProcessingDataBox* processingDataBox = calcAlgContext->getUsedProcessingDataBox();
					CIndividualPilingProcessNodeVector* indiNodeVector = processingDataBox->getIndividualPilingProcessNodeVector();
					cint64 indiNodeCount = indiNodeVector->getItemCount();
					cint64 subsumingCount = 0;
					LOG(INFO,"::Konclude::Reasoner::Kernel::Algorithm",logTr("Analyse subsumtion relations from model."),this);
					for (cint64 indiID = 0; indiID < indiNodeCount; ++indiID) {
						CIndividualPilingProcessNode* indiNode = indiNodeVector->getData(indiID);
						if (indiNode) {
							cint64 pilingDepth = indiNode->getPilingDepth();
							CXSortedNegLinker<CConcept*>* initConLinker = indiNode->getInitializedConceptLinker();
							if (initConLinker) {
								CConcept* initConcept = initConLinker->getData();
								if (initConcept->hasClassName()) {
									CConceptProcessData* conProData = (CConceptProcessData*)initConcept->getConceptData();
									CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)conProData->getConceptReferenceLinking();
									CClassificationConceptReferenceLinking* classSatCalcRefLinkData = confSatRefLinkingData->getClassifierReferenceLinkingData();
									COptimizedClassSaturationSatisfiableTestingItem* classSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)classSatCalcRefLinkData;
									if (classSatItem) {
										CReapplyConceptPilingLabelSet* conSet = indiNode->getRootPilingIndividualNode()->getReapplyConceptPilingLabelSet(false);
										if (conSet) {
											//CReapplyConceptPilingLabelSetIterator conDesIt = conSet->getIterator(true,false,pilingDepth);
											for (CConceptPilingDescriptor* conPilDes = conSet->getConceptPilingDescriptionLinker(); conPilDes; conPilDes = conPilDes->getNext()) {
											//while (conDesIt.hasNext()) {
											//	CConceptPilingDescriptor* conPilDes = conDesIt.getConceptPilingDescriptor();
												if (!conPilDes->isNegated()) {
													CConcept* subsumerConcept = conPilDes->getConcept();
													if (subsumerConcept != initConcept && subsumerConcept->hasClassName()) {
														CConceptProcessData* subsumerConProData = (CConceptProcessData*)subsumerConcept->getConceptData();
														CConceptSaturationReferenceLinkingData* subsumerConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)subsumerConProData->getConceptReferenceLinking();
														CClassificationConceptReferenceLinking* subsumerClassSatCalcRefLinkData = subsumerConfSatRefLinkingData->getClassifierReferenceLinkingData();
														if (subsumerClassSatCalcRefLinkData) {
															COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)subsumerClassSatCalcRefLinkData;
															classSatItem->addSubsumingConceptItem(subsumerClassSatItem);
															subsumingCount++;
														}

													}
												}
												//conDesIt.moveNext();
											}
										}

										CIndividualPilingProcessNode* substituiteNode = indiNode->getSubstituteIndividualNode();
										while (substituiteNode) {
											while (substituiteNode->hasSubstituteIndividualNode()) {
												CXSortedNegLinker<CConcept*>* initializedConLinkIt = substituiteNode->getInitializedConceptLinker();
												while (initializedConLinkIt) {
													if (!initializedConLinkIt->isNegated()) {
														CConcept* subsumerConcept = initializedConLinkIt->getData();
														if (subsumerConcept != initConcept && subsumerConcept->hasClassName()) {
															CConceptProcessData* subsumerConProData = (CConceptProcessData*)subsumerConcept->getConceptData();
															CConceptSaturationReferenceLinkingData* subsumerConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)subsumerConProData->getConceptReferenceLinking();
															CClassificationConceptReferenceLinking* subsumerClassSatCalcRefLinkData = subsumerConfSatRefLinkingData->getClassifierReferenceLinkingData();
															if (subsumerClassSatCalcRefLinkData) {
																COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)subsumerClassSatCalcRefLinkData;
																classSatItem->addSubsumingConceptItem(subsumerClassSatItem);
																subsumingCount++;
															}
														}

													}
													initializedConLinkIt = initializedConLinkIt->getNext();
												}
												substituiteNode = substituiteNode->getSubstituteIndividualNode();
											}
											CReapplyConceptPilingLabelSet* subsConSet = substituiteNode->getRootPilingIndividualNode()->getReapplyConceptPilingLabelSet(false);
											//CReapplyConceptPilingLabelSetIterator subConDesIt = subsConSet->getIterator(true,false,pilingDepth);
											//while (subConDesIt.hasNext()) {
											for (CConceptPilingDescriptor* subConPilDes = subsConSet->getConceptPilingDescriptionLinker(); subConPilDes; subConPilDes = subConPilDes->getNext()) {
												//CConceptPilingDescriptor* subConPilDes = subConDesIt.getConceptPilingDescriptor();
												if (!subConPilDes->isNegated()) {
													CConcept* subsumerConcept = subConPilDes->getConcept();
													if (subsumerConcept != initConcept && subsumerConcept->hasClassName()) {
														CConceptProcessData* subsumerConProData = (CConceptProcessData*)subsumerConcept->getConceptData();
														CConceptSaturationReferenceLinkingData* subsumerConfSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)subsumerConProData->getConceptReferenceLinking();
														CClassificationConceptReferenceLinking* subsumerClassSatCalcRefLinkData = subsumerConfSatRefLinkingData->getClassifierReferenceLinkingData();
														if (subsumerClassSatCalcRefLinkData) {
															COptimizedClassSaturationSatisfiableTestingItem* subsumerClassSatItem = (COptimizedClassSaturationSatisfiableTestingItem*)subsumerClassSatCalcRefLinkData;
															classSatItem->addSubsumingConceptItem(subsumerClassSatItem);
															subsumingCount++;
														}
													}
												}
												//subConDesIt.moveNext();
											}

											substituiteNode = substituiteNode->getSubstituteIndividualNode();
										}
									}
								}
							}
						}
					}
					LOG(INFO,"::Konclude::Reasoner::Kernel::Algorithm",logTr("Analysed %1 subsumtion relations from model.").arg(subsumingCount),this);
					return false;
				}




			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
