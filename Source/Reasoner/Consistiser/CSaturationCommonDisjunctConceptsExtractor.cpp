/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CSaturationCommonDisjunctConceptsExtractor.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CSaturationCommonDisjunctConceptsExtractor::CSaturationCommonDisjunctConceptsExtractor() {
			}

			bool CSaturationCommonDisjunctConceptsExtractor::extractCommonDisjunctConcepts(CConcreteOntology* ontology, CSaturationData* saturationData, QSet<CConcept*>* disjunctionConceptSet) {
				CSaturationTaskData* satTaskData = (CSaturationTaskData*)saturationData;
				if (satTaskData) {
					CSatisfiableCalculationTask* satCalcTask = satTaskData->getSaturationTask();
					if (satCalcTask) {

						CMemoryAllocationManager* mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
						CImplicationReplacementVector* repVector = ontology->getDataBoxes()->getMBox()->getImplicationReplacementVector(true);

						for (QSet<CConcept*>::const_iterator it = disjunctionConceptSet->constBegin(), itEnd = disjunctionConceptSet->constEnd(); it != itEnd; ++it) {
							CConcept* disjunctionConcept(*it);
							bool disjunctionNegation = false;
							cint64 disjunctionOpCode = disjunctionConcept->getOperatorCode();
							if (disjunctionOpCode == CCAND || disjunctionOpCode == CCEQ) {
								disjunctionNegation = true;
							}
							CConceptProcessData* disjConProData = (CConceptProcessData*)disjunctionConcept->getConceptData();
							if (disjConProData) {
								CReplacementData* disjReplData = disjConProData->getReplacementData();
								bool replacementDataInitialized = false;
								QSet< QPair<CConcept*,bool> > commonConceptNegationPairSet;

								CConceptSaturationReferenceLinkingData* confSatRefLinkingData = (CConceptSaturationReferenceLinkingData*)disjConProData->getConceptReferenceLinking();
								if (confSatRefLinkingData) {
									CSaturationConceptReferenceLinking* disjunctConceptRefLinkData = confSatRefLinkingData->getConceptSaturationReferenceLinkingData(disjunctionNegation);
									if (disjunctConceptRefLinkData) {
										CIndividualSaturationProcessNode* satIndiNode = (CIndividualSaturationProcessNode*)disjunctConceptRefLinkData->getIndividualProcessNodeForConcept();
										if (satIndiNode && satIndiNode->isCompleted()) {
											CReapplyConceptSaturationLabelSet* conSet = satIndiNode->getReapplyConceptSaturationLabelSet(false);
											if (conSet) {
												for (CConceptSaturationDescriptor* conDesIt = conSet->getConceptSaturationDescriptionLinker(); conDesIt; conDesIt = conDesIt->getNext()) {
													CConcept* concept = conDesIt->getConcept();
													bool negation = conDesIt->isNegated();

													if (concept != disjunctionConcept && concept->getOperatorCode() != CCTOP) {
														if (!disjReplData) {
															disjReplData = CObjectAllocator<CReplacementData>::allocateAndConstruct(mMemMan);
															disjReplData->initReplacementData(nullptr);
															disjConProData->setReplacementData(disjReplData);
															repVector->setData(concept->getConceptTag(),disjReplData);
															replacementDataInitialized = true;
														}

														if (!replacementDataInitialized) {
															for (CXNegLinker<CConcept*>* commonConLinkIt = disjReplData->getCommonDisjunctConceptLinker(); commonConLinkIt; commonConLinkIt = commonConLinkIt->getNext()) {
																CConcept* commonConcept = commonConLinkIt->getData();
																bool commonConceptNeg = commonConLinkIt->isNegated();
																commonConceptNegationPairSet.insert( QPair<CConcept*,bool>(commonConcept,commonConceptNeg) );
															}
														}

														if (!commonConceptNegationPairSet.contains(QPair<CConcept*,bool>(concept,negation))) {
															commonConceptNegationPairSet.insert(QPair<CConcept*,bool>(concept,negation));
															CXNegLinker<CConcept*>* commNegConLinker = CObjectAllocator< CXNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
															commNegConLinker->initNegLinker(concept,negation);
															disjReplData->addCommonDisjunctConceptLinker(commNegConLinker);
														}
													}

												}
											}
										}
									}
								}
							}
						}
					}
				}
				return false;
			}

		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
