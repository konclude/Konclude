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

#include "CSequentialVariableMappingsCompositionPropagationComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CSequentialVariableMappingsCompositionPropagationComputator::CSequentialVariableMappingsCompositionPropagationComputator() {
					mComputerName = "Sequential propagation";
				}



				CAbstractVariableMappingsCompositionComputator* CSequentialVariableMappingsCompositionPropagationComputator::configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionPropagationComputator::configureComputator(ontoAnsweringItem, reqProcessor, answererContext);
					return this;
				}



				bool CSequentialVariableMappingsCompositionPropagationComputator::computeVariableMappingsPropagationComposition(COptimizedComplexVariableRolePropagationAbstractItem* compVarItem, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {

					COptimizedComplexVariableRolePropagationAbstractItem* rolePropItem = (COptimizedComplexVariableRolePropagationAbstractItem*)compVarItem;
					COptimizedComplexVariableCompositionItem* baseVarItem = rolePropItem->getBaseItem();
					CRole* role = rolePropItem->getRole();
					bool inversed = rolePropItem->getInversed();
					COptimizedComplexVariableIndividualMappings* baseVariableMapping = baseVarItem->getVariableMapping();
					COptimizedComplexVariableCompositionItemDependence* baseItemDep = rolePropItem->getBaseItemDependence();
					COptimizedComplexVariableIndividualMappings* variableMapping = rolePropItem->getVariableMapping();
					CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase*>* waitingProcessedRealizationIteratorDataList = rolePropItem->getWaitingProcessedRealizationIteratorDataList();
					QList<COntologyProcessingRequirement*>* schedulingReqList = rolePropItem->geSchedulingRealizationRequirementIteratorDataList();

					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* instItemRolePropInstIterationDataHash = rolePropItem->getInstanceItemRolePropagationInstanceIterationDataHash();

					// get variable index
					cint64 varIdx = rolePropItem->getPropagationVariableIndex();
					cint64 baseBindingSize = baseVariableMapping->getBindingSize();
					cint64 newBindingSize = baseVariableMapping->getBindingSize();
					if (compVarItem->getCompositionType() == COptimizedComplexVariableCompositionItem::ROLE_PROPAGATION) {
						newBindingSize = baseVariableMapping->getBindingSize() + 1;
					}
					cint64 newVarIdx = baseBindingSize;
					bool joining = false;
					bool replacing = false;
					if (compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationJoiningItem::ROLE_PROPAGATION_JOIN) {
						COptimizedComplexVariableRolePropagationJoiningItem* joinRolePropItem = (COptimizedComplexVariableRolePropagationJoiningItem*)compVarItem;
						joining = true;
						newVarIdx = joinRolePropItem->getJoiningVariableIndex();
					}
					if (compVarItem->getCompositionType() == COptimizedComplexVariableRolePropagationReplacementItem::ROLE_PROPAGATION_REPLACEMENT) {
						COptimizedComplexVariableRolePropagationReplacementItem* replRolePropItem = (COptimizedComplexVariableRolePropagationReplacementItem*)compVarItem;
						replacing = true;
						newVarIdx = replRolePropItem->getReplacementVariableIndex();
					}

					cint64* baseBindingMapping = new cint64[newBindingSize];
					for (cint64 i = 0; i < baseBindingSize; ++i) {
						baseBindingMapping[i] = i;
					}

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase*>* schedulingRealizationIteratorDataList = rolePropItem->getSchedulingRealizationIteratorDataList();

					CRoleInstantiatedItem* roleItem = roleReal->getRoleInstantiatedItem(role);

					COptimizedComplexVariableIndividualBindings* propagedBindings = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinality* propagedCardinalites = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagedLinker = nullptr;
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();
					CMemoryAllocationManager* bindingsMemMan = variableMapping->getBindingsMemoryAllocationManager();


					function<void(COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities, const CRealizationIndividualInstanceItemReference& propInstItem)> variableMappingCreationFunc = [&](COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities, const CRealizationIndividualInstanceItemReference& propInstItem) {

						COptimizedComplexVariableIndividualBindings* varBinding = varBindingCardLinker->getBindings();
						if (!propagedBindings) {
							propagedBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem, bindingsMemMan);
						}
						if (!propagedCardinalites && varCardinalities) {
							propagedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem, bindingsMemMan);
						}
						if (varCardinalities) {
							*propagedCardinalites = *varCardinalities;
						}


						if (replacing) {
							if (!propagedCardinalites) {
								propagedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem, bindingsMemMan);
								propagedCardinalites->initCardinalities(1, 1);
							}
							propagedCardinalites->setCardinalityUpdateId(-1);

							TIndividualInstanceItemDataBinding& reducedBinding = varBinding->getBinding(newVarIdx);
							CInstanceBindingIndividualCountingVisitor visitor;
							sameRealization->visitSameIndividuals(reducedBinding.reference, &visitor);
							if (visitor.individualCount > 1) {
								propagedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(visitor.individualCount * propagedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
							}

						}

						COptimizedComplexVariableIndividualBindings nextNewBinding(newBindingSize);
						for (cint64 i = 0; i < baseBindingSize; ++i) {
							propagedBindings->setBinding(baseBindingMapping[i], varBinding->getBinding(i));
						}
						bool inserting = true;
						if (joining) {
							if (propInstItem == varBinding->getBinding(newVarIdx).reference) {
								inserting = true;
							} else {
								inserting = false;
							}
						}
						propagedBindings->setBinding(newVarIdx, propInstItem);

						if (!propagedLinker) {
							propagedLinker = createBindingsLinkerForVariableCompositionItems(propagedBindings, propagedCardinalites, compVarItem, buildingVarItem, bindingsMemMan);
						} else {
							propagedLinker->setInitialCardinality(propagedCardinalites);
						}

						if (inserting) {
							if (variableMapping->addInsertingBindingsCardinalityLinker(propagedLinker, true)) {
								propagedBindings = nullptr;
								propagedCardinalites = nullptr;
								propagedLinker = nullptr;
							} else {
								propagedCardinalites = propagedLinker->getCurrentCardinalities();
							}
						}

					};


					CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
					CRoleInstantiatedItem* roleInstancesItem = roleRealization->getRoleInstantiatedItem(role);

					function<void(COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData)> iterationDataHandleFunc = [&](COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData) {


						//QString indiString = CIRIName::getRecentIRIName(iteratorData->debugPropInstItemRef.getIndividual()->getIndividualNameLinker());
						//COptimizedKPSetRoleInstancesRedirectionItem* initialRedirectionItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleInstancesItem;
						//COptimizedKPSetRoleInstancesItem* roleItem = (COptimizedKPSetRoleInstancesItem*)initialRedirectionItem->getRedirectedItem();
						//QString roleString = CIRIName::getRecentIRIName(roleItem->getRole()->getPropertyNameLinker());
						//QString inverseRoleString = CIRIName::getRecentIRIName(roleItem->getInverseRole()->getPropertyNameLinker());
						//if (indiString == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1" && (roleString == "http://ontology.dumontierlab.com/isPartOf" || inverseRoleString == "http://ontology.dumontierlab.com/isPartOf")) {
						//	bool debug = true;
						//}


						if (!iteratorData->isRealizationScheduled()) {
							QSet<CRealizationIndividualInstanceItemReference>* rolePropInstItemSet = iteratorData->getPropagationInstanceItemSet();
							CRealizationIndividualInstanceItemReferenceIterator* fillerIterator = iteratorData->getRealizationIterator();

							while (fillerIterator && !fillerIterator->atEnd() && !fillerIterator->requiresInitialization()) {
								CRealizationIndividualInstanceItemReference fillerInstanceItemRef = fillerIterator->currentIndividualInstanceItemReference();
								iteratorData->getPropagationInstanceItemSet()->insert(fillerInstanceItemRef);

								rolePropItem->incFillerInstanceItemCount(iteratorData->getVariableMappingPropagationIteratorList()->size());

								fillerIterator->moveNext();
							}

							if (fillerIterator) {
								double remainingFillerEstimationCount = fillerIterator->remainingInstancesEstimation().getEstimatedCount();
								double prevFillerEstimation = iteratorData->getExpectedFillerCount();
								double newFillerEstimation = iteratorData->getPropagationInstanceItemSet()->size() + remainingFillerEstimationCount;
								iteratorData->setExpectedFillerCount(newFillerEstimation);
								double diffFillerEstimation = newFillerEstimation - prevFillerEstimation;
								rolePropItem->incFillerInstanceItemCount(diffFillerEstimation);
							}

							if (fillerIterator && fillerIterator->requiresInitialization()) {
								COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(fillerIterator);
								schedulingRealizationIteratorDataList->append(iteratorData);
								schedulingReqList->append(roleRealReq);
								iteratorData->setRealizationScheduled(true);
							} else if (!fillerIterator || fillerIterator->atEnd()) {
								if (fillerIterator) {
									rolePropItem->incPropagationHandledInstanceItemCount();
									delete fillerIterator;
									iteratorData->setRealizationIterator(nullptr);
								}
								QList<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* variableMappingIteratorList = iteratorData->getVariableMappingPropagationIteratorList();
								while (!variableMappingIteratorList->isEmpty()) {
									COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker = variableMappingIteratorList->takeFirst();
									COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = varBindingCardLinker->getInitialCardinalities();
									rolePropItem->incPropagatedInstanceItemCount();
									if (!rolePropInstItemSet->isEmpty()) {
										rolePropItem->incRealizationFinishedFillerInstanceItemCount(rolePropInstItemSet->size());
										for (const CRealizationIndividualInstanceItemReference& propInstItem : *rolePropInstItemSet) {
											variableMappingCreationFunc(varBindingCardLinker, cardinalites, propInstItem);
										}
									}
								}


								QList<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* cardinalityUpdateIteratorList = iteratorData->getCardinalityUpdatePropagationIteratorList();
								while (!cardinalityUpdateIteratorList->isEmpty()) {
									COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = cardinalityUpdateIteratorList->takeFirst();

									COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
									COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
									COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
									COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

									COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
									tmpDiffCardinalites.substractCardinalities(prevCardinalites);

									if (!rolePropInstItemSet->isEmpty()) {
										for (const CRealizationIndividualInstanceItemReference& propInstItem : *rolePropInstItemSet) {
											variableMappingCreationFunc(bindingLinker, &tmpDiffCardinalites, propInstItem);
										}
									}
								}
							}
						}
					};

					bool successorCollectionContinued = false;
					while (!waitingProcessedRealizationIteratorDataList->isEmpty() && rolePropItem->requiresMoreVariableMappingsComputation()) {
						COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData* iteratorData = (COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*)waitingProcessedRealizationIteratorDataList->takeFirst();
						iteratorData->getRealizationIterator()->begin();
						iteratorData->setRealizationScheduled(false);
						rolePropItem->decScheduledRealizationCount();
						iterationDataHandleFunc(iteratorData);
						successorCollectionContinued = true;
					}



					bool mappingProcessed = false;
					while (rolePropItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {

						while (rolePropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							mappingProcessed = true;
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = baseItemDep->getBatchCurrentBindingsCardinalityLinker(true);

							COptimizedComplexVariableIndividualBindings* varBinding = bindingLinker->getBindings();
							CRealizationIndividualInstanceItemReference instItemRef = varBinding->getBinding(varIdx).reference;

							//if (CIRIName::getRecentIRIName(instItemRef.getIndividual()->getIndividualNameLinker()) == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1") {
							//	bool debug = true;
							//}

							COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*& iterationData = (*instItemRolePropInstIterationDataHash)[instItemRef];
							if (!iterationData) {
								iterationData = new COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData();
								//iterationData->debugPropInstItemRef = instItemRef;
								iterationData->setRealizationIterator(roleRealization->getRoleFillerInstancesIterator(instItemRef, roleInstancesItem, inversed, mRoleFillerRealizationSorting));
								iterationData->getVariableMappingPropagationIteratorList()->append(bindingLinker);
								iterationDataHandleFunc(iterationData);
							} else if (!iterationData->getRealizationIterator()) {
								COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();
								QSet<CRealizationIndividualInstanceItemReference>* rolePropInstItemSet = iterationData->getPropagationInstanceItemSet();
								rolePropItem->incPropagatedInstanceItemCount();
								rolePropItem->incFillerInstanceItemCount(rolePropInstItemSet->size());
								rolePropItem->incRealizationFinishedFillerInstanceItemCount(rolePropInstItemSet->size());
								for (const CRealizationIndividualInstanceItemReference& propInstItem : *rolePropInstItemSet) {
									variableMappingCreationFunc(bindingLinker, cardinalites, propInstItem);
								}
							} else {
								iterationData->addVariableMappingPropagationIterator(bindingLinker);
								rolePropItem->incFillerInstanceItemCount(iterationData->getPropagationInstanceItemSet()->size());
								iterationDataHandleFunc(iterationData);
							}

						}


						while (rolePropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							mappingProcessed = true;
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = baseItemDep->getBatchCurrentUpdatedCardinalityLinker(true);

							COptimizedComplexVariableIndividualBindings* varBinding = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker()->getBindings();
							CRealizationIndividualInstanceItemReference instItemRef = varBinding->getBinding(varIdx).reference;

							COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*& iterationData = (*instItemRolePropInstIterationDataHash)[instItemRef];
							iterationData->addCardinalityUpdatePropagationIterator(updatedCardinalityLinker);
							iterationDataHandleFunc(iterationData);
						}

					}

					variableMapping->incCurrentUpdateId();



					return mappingProcessed;

				}






			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
