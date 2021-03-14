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

#include "CQtConcurrentVariableMappingsCompositionPropagationComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CQtConcurrentVariableMappingsCompositionPropagationComputator::CQtConcurrentVariableMappingsCompositionPropagationComputator() {
					mComputerName = "Concurrent Qt propagation";
				}



				CAbstractVariableMappingsCompositionComputator* CQtConcurrentVariableMappingsCompositionPropagationComputator::configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionPropagationComputator::configureComputator(ontoAnsweringItem, reqProcessor, answererContext);

					mConfConcurrentPropagationHashCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentPropagationComputationTaskCount", 211);
					mConfPartitionizedMemoryManagement = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentPropagationPartitionizedMemoryManagement", false);
					mConfPerformanceLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentPropagationPerformanceLogging", false);
					mConfConcurrentVariableMappingPartCount = mConfConcurrentPropagationHashCount;
					mConfBatchSizeLimit = 200;
					mConfBatchSizeCount = mConfConcurrentPropagationHashCount;

					return this;
				}



				bool CQtConcurrentVariableMappingsCompositionPropagationComputator::computeVariableMappingsPropagationComposition(COptimizedComplexVariableRolePropagationAbstractItem* compVarItem, COptimizedComplexBuildingIndividualVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {

					COptimizedComplexVariableRolePropagationAbstractItem* rolePropItem = (COptimizedComplexVariableRolePropagationAbstractItem*)compVarItem;
					COptimizedComplexVariableCompositionItem* baseVarItem = rolePropItem->getBaseItem();
					CRole* role = rolePropItem->getRole();
					bool inversed = rolePropItem->getInversed();
					COptimizedComplexVariableIndividualMappings* baseVariableMapping = baseVarItem->getVariableMapping();
					COptimizedComplexVariableCompositionItemDependence* baseItemDep = rolePropItem->getBaseItemDependence();
					//COptimizedComplexVariableIndividualMappings* variableMapping = rolePropItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappingsMultiHash* propagatedVarMapping = (COptimizedComplexVariableIndividualMappingsMultiHash*)rolePropItem->getVariableMapping();

					CRoleRealization* roleReal = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();

					QList<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataBase*>* waitingProcessedRealizationIteratorDataList = rolePropItem->getWaitingProcessedRealizationIteratorDataList();
					QList<COntologyProcessingRequirement*>* schedulingReqList = rolePropItem->geSchedulingRealizationRequirementIteratorDataList();

					QHash<CRealizationIndividualInstanceItemReference, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorData*>* instItemRolePropInstIterationDataHash = rolePropItem->getInstanceItemRolePropagationInstanceIterationDataHash();
					QVector<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged*>* instItemRolePropInstIterationDataHashVector = rolePropItem->getInstanceItemRolePropagationInstanceIterationDataHashVector();
					if (instItemRolePropInstIterationDataHashVector->size() < mConfConcurrentPropagationHashCount) {
						instItemRolePropInstIterationDataHashVector->resize(mConfConcurrentPropagationHashCount);
					}


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



					CRoleRealization* roleRealization = mOntoAnsweringItem->getOntology()->getRealization()->getRoleRealization();
					CRoleInstantiatedItem* roleInstancesItem = roleRealization->getRoleInstantiatedItem(role);





































					cint64 currentBatchSizeCount = mConfBatchSizeCount;
					cint64 currentBatchSizeLimit = mConfBatchSizeLimit;




					function<void(COptimizedComplexVariableCompositionItemDependence* itemDep, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> propagationMappingsBatchingFunc =
						[&](COptimizedComplexVariableCompositionItemDependence* itemDep, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {

						cint64 batchSize = 1;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* firstBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* lastBindingLinker = firstBindingLinker;
						while (batchSize++ < currentBatchSizeLimit && itemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							lastBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
						}

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
						CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* data = nullptr;
						if (!mConfPartitionizedMemoryManagement) {
							data = keyMappingBatchDataVector.createBatchLinker();
						} else {
							data = vecData->createBatchLinker();
						}
						data->setBatchBindingsCardinalityLinkers(firstBindingLinker, lastBindingLinker);
						vecData->addBatchLinker(data);
					};




					function<void(COptimizedComplexVariableCompositionItemDependence* itemDep, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> propagationCardinalitesBatchingFunc =
						[&](COptimizedComplexVariableCompositionItemDependence* itemDep, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {

						cint64 batchSize = 1;
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* firstCardinalityLinker = itemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						COptimizedComplexVariableIndividualUpdateCardinalityLinker* lastCardinalityLinker = firstCardinalityLinker;
						while (batchSize++ < currentBatchSizeLimit && itemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							lastCardinalityLinker = itemDep->getBatchCurrentUpdatedCardinalityLinker(true);
						}

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
						CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* data = nullptr;
						if (!mConfPartitionizedMemoryManagement) {
							data = keyMappingBatchDataVector.createBatchLinker();
						} else {
							data = vecData->createBatchLinker();
						}
						data->setBatchCardinalityUpdateLinkers(firstCardinalityLinker, lastCardinalityLinker);
						vecData->addBatchLinker(data);
					};


























					// map & reduce of step 1
					cint64 keyMappingSize = mConfConcurrentPropagationHashCount;

					function<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* const & batchData)> bindingBatchesHashingKeyMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* const & batchData) ->CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker> {

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker> keyMappedVec(keyMappingSize);
						if (batchData) {
							CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* batchLinker = batchData->getNextBatchLinker();

							while (batchLinker) {





								COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* contIterData = batchLinker->getNextContinueIterationData(true);
								while (contIterData) {

									CRealizationIndividualInstanceItemReference instItemRef = contIterData->getPropagationInstanceItemReference();
									cint64 propIndiId = instItemRef.getIndividualID();

									cint64 keyHashValue = propIndiId;
									cint64 mappedKeyIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % keyMappingSize;

									CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* keyMappedVecData = keyMappedVec.getVectorLinkerData(mappedKeyIndex, true);
									CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker* mappedKeyDataLinker = nullptr;
									if (!mConfPartitionizedMemoryManagement) {
										mappedKeyDataLinker = keyMappedVec.createBatchLinker();
									} else {
										mappedKeyDataLinker = keyMappedVecData->createBatchLinker();
									}
									mappedKeyDataLinker->initMappedKeysBindingsCardinalityData(instItemRef, mappedKeyIndex, nullptr, nullptr, nullptr);
									keyMappedVecData->addBatchLinker(mappedKeyDataLinker);

									contIterData = batchLinker->getNextContinueIterationData(true);
								}



								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								while (bindingsCardinalityCurrentLinker) {


									COptimizedComplexVariableIndividualBindings* varBinding = bindingsCardinalityCurrentLinker->getBindings();
									CRealizationIndividualInstanceItemReference instItemRef = varBinding->getBinding(varIdx).reference;
									cint64 propIndiId = instItemRef.getIndividualID();

									cint64 keyHashValue = propIndiId;
									cint64 mappedKeyIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % keyMappingSize;

									CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* keyMappedVecData = keyMappedVec.getVectorLinkerData(mappedKeyIndex, true);
									CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker* mappedKeyDataLinker = nullptr;
									if (!mConfPartitionizedMemoryManagement) {
										mappedKeyDataLinker = keyMappedVec.createBatchLinker();
									} else {
										mappedKeyDataLinker = keyMappedVecData->createBatchLinker();
									}
									mappedKeyDataLinker->initMappedKeysBindingsCardinalityData(instItemRef, mappedKeyIndex, bindingsCardinalityCurrentLinker->getBindings(), bindingsCardinalityCurrentLinker->getInitialCardinalities(), nullptr);
									keyMappedVecData->addBatchLinker(mappedKeyDataLinker);

									bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								}



								COptimizedComplexVariableIndividualUpdateCardinalityLinker* cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								while (cardinalityUpdateLinker) {
									
									COptimizedComplexVariableIndividualBindings* varBinding = cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings();
									CRealizationIndividualInstanceItemReference instItemRef = varBinding->getBinding(varIdx).reference;
									cint64 propIndiId = instItemRef.getIndividualID();

									cint64 keyHashValue = propIndiId;
									cint64 mappedKeyIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % keyMappingSize;

									CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* keyMappedVecData = keyMappedVec.getVectorLinkerData(mappedKeyIndex, true);
									CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker* mappedKeyDataLinker = nullptr;
									if (!mConfPartitionizedMemoryManagement) {
										mappedKeyDataLinker = keyMappedVec.createBatchLinker();
									} else {
										mappedKeyDataLinker = keyMappedVecData->createBatchLinker();
									}
									mappedKeyDataLinker->initMappedKeysBindingsCardinalityData(instItemRef, mappedKeyIndex, cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings(), cardinalityUpdateLinker->getNewCardinality(), cardinalityUpdateLinker->getPreviousCardinality());
									keyMappedVecData->addBatchLinker(mappedKeyDataLinker);

									cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								}



								batchLinker = batchLinker->getNext();
							}
						}

						return keyMappedVec;
					};


					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>& reduceVec)> bindingBatchesHashingKeyReduceFunc =
						[&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>& reduceVec) -> void {

						if (vec.size() < reduceVec.size()) {
							vec.resize(reduceVec.size());
						}
						for (cint64 i = 0; i < vec.count(); ++i) {
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>*& vecData = vec[i];
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* const & reduceVecData = reduceVec[i];
							if (reduceVecData) {
								if (!vecData) {
									vecData = reduceVecData;
								} else {
									vecData->mergeBatchLinkers(reduceVecData);
								}
							}

						}
						vec.appendMemoryPool(reduceVec.getMemoryPools());
					};








































					// map & reduce of step 2
					cint64 bindingsMappingSize = mConfConcurrentVariableMappingPartCount;



					auto variableMappingCreationFunc = [&](COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities, const CRealizationIndividualInstanceItemReference& propInstItem, 
							COptimizedComplexVariableIndividualBindings* propagedBindings, COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagedLinker, COptimizedComplexVariableIndividualBindingsCardinality* propagedCardinalites, CMemoryAllocationManager* memMan) {

						COptimizedComplexVariableIndividualBindings* varBinding = varBindingCardLinker->getBindings();
						if (!propagedBindings) {
							propagedBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem, memMan);
						}
						if (!propagedCardinalites && varCardinalities) {
							propagedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem, memMan);
						}
						if (varCardinalities) {
							*propagedCardinalites = *varCardinalities;
						}


						if (replacing) {
							if (!propagedCardinalites) {
								propagedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem, memMan);
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
							propagedLinker = createBindingsLinkerForVariableCompositionItems(propagedBindings, propagedCardinalites, compVarItem, buildingVarItem, memMan);
						} else {
							propagedLinker->setInitialCardinality(propagedCardinalites);
						}

						if (!inserting) {
							//if (variableMapping->addInsertingBindingsCardinalityLinker(propagedLinker, true)) {
							//	propagedBindings = nullptr;
							//	propagedCardinalites = nullptr;
							//	propagedLinker = nullptr;
							//} else {
							//	propagedCardinalites = propagedLinker->getCurrentCardinalities();
							//}
							propagedLinker = nullptr;
						}

						return propagedLinker;
					};






					auto propagatedVariableMappingCreationCopyingVecFunc = [&](COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker, COptimizedComplexVariableIndividualBindingsCardinality* varCardinalities, const CRealizationIndividualInstanceItemReference& propInstItem,
						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& bindingsMappedVec, CMemoryAllocationManager* memMan, COptimizedComplexVariableIndividualBindings*& tmpJoinedBindings) -> void {


						CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker* mappedBindingsDataLinker = nullptr;
						if (!mConfPartitionizedMemoryManagement) {
							mappedBindingsDataLinker = bindingsMappedVec.createBatchLinker();

							COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagatedLinker = variableMappingCreationFunc(varBindingCardLinker, varCardinalities, propInstItem, nullptr, nullptr, nullptr, memMan);

							if (propagatedLinker) {
								COptimizedComplexVariableIndividualBindingsHasher bindingsHasher(propagatedLinker);
								cint64 keyHashValue = bindingsHasher.getHashValue();
								cint64 mappedBindingsIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % bindingsMappingSize;

								mappedBindingsDataLinker->initMappedJoinedBindingsCardinalityData(mappedBindingsIndex, bindingsHasher, propagatedLinker);
								bindingsMappedVec.addBatchLinker(mappedBindingsIndex, mappedBindingsDataLinker);
							}

						} else {

							if (!tmpJoinedBindings) {
								tmpJoinedBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem, memMan);
							}
							COptimizedComplexVariableIndividualBindingsCardinalityLinker tmpPropagatedLinker;
							COptimizedComplexVariableIndividualBindingsCardinality tmpJoinedCardinalites;

							COptimizedComplexVariableIndividualBindingsCardinalityLinker* propagatedLinker = variableMappingCreationFunc(varBindingCardLinker, varCardinalities, propInstItem, tmpJoinedBindings, &tmpPropagatedLinker, &tmpJoinedCardinalites, memMan);
							if (propagatedLinker) {

								COptimizedComplexVariableIndividualBindingsHasher tmpBindingsHasher(&tmpPropagatedLinker);
								cint64 keyHashValue = tmpBindingsHasher.getHashValue();
								cint64 mappedBindingsIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % bindingsMappingSize;

								CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>* bindingsMappedVecData = bindingsMappedVec.getVectorLinkerData(mappedBindingsIndex, true);

								// copy data with corresponding memory manager
								memMan = bindingsMappedVecData->getMemoryAllocationManager();
								COptimizedComplexVariableIndividualBindings* joinedBindings = createBindingsForVariableCompositionItems(newBindingSize, compVarItem, buildingVarItem, memMan);
								for (cint64 i = 0; i < newBindingSize; ++i) {
									joinedBindings->setBinding(i, tmpJoinedBindings->getBinding(i));
								}
								COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;
								if (tmpPropagatedLinker.getCurrentCardinalities()) {
									joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(compVarItem, buildingVarItem, memMan);
									*joinedCardinalites = tmpJoinedCardinalites;
								}
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, compVarItem, buildingVarItem, memMan);
								COptimizedComplexVariableIndividualBindingsHasher bindingsHasher(joinedLinker, keyHashValue);

								mappedBindingsDataLinker = bindingsMappedVecData->createBatchLinker();
								mappedBindingsDataLinker->initMappedJoinedBindingsCardinalityData(mappedBindingsIndex, bindingsHasher, joinedLinker);
								bindingsMappedVecData->addBatchLinker(mappedBindingsDataLinker);
							}
						}
					};




					auto iterationDataHandleFunc = [&](COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iteratorData, COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* instItemRolePropInstIterationDataHash,
						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& bindingsMappedVec, CMemoryAllocationManager* memMan, COptimizedComplexVariableIndividualBindings*& tmpJoinedBindings) {


						//QString indiString = CIRIName::getRecentIRIName(iteratorData->debugPropInstItemRef.getIndividual()->getIndividualNameLinker());
						//COptimizedKPSetRoleInstancesRedirectionItem* initialRedirectionItem = (COptimizedKPSetRoleInstancesRedirectionItem*)roleInstancesItem;
						//COptimizedKPSetRoleInstancesItem* roleItem = (COptimizedKPSetRoleInstancesItem*)initialRedirectionItem->getRedirectedItem();
						//QString roleString = CIRIName::getRecentIRIName(roleItem->getRole()->getPropertyNameLinker());
						//QString inverseRoleString = CIRIName::getRecentIRIName(roleItem->getInverseRole()->getPropertyNameLinker());
						//if (indiString == "http://ontology.dumontierlab.com/eswc-example-graph-3#datapoint1" && (roleString == "http://ontology.dumontierlab.com/isPartOf" || inverseRoleString == "http://ontology.dumontierlab.com/isPartOf")) {
						//	bool debug = true;
						//}

						CRealizationIndividualInstanceItemReference instItemRef = iteratorData->getPropagationInstanceItemReference();
						if (!iteratorData->isRealizationIterationInitialized()) {
							iteratorData->setRealizationIterationInitialized(true);
							iteratorData->setRealizationIterator(roleRealization->getRoleFillerInstancesIterator(instItemRef, roleInstancesItem, inversed, mRoleFillerRealizationSorting));
						}

						if (!iteratorData->isRealizationScheduled()) {
							CRealizationIndividualInstanceItemReferenceIterator* fillerIterator = iteratorData->getRealizationIterator();

							while (fillerIterator && !fillerIterator->atEnd() && !fillerIterator->requiresInitialization()) {
								CRealizationIndividualInstanceItemReference fillerInstanceItemRef = fillerIterator->currentIndividualInstanceItemReference();

								iteratorData->addPropagationInstanceItemLinker(fillerInstanceItemRef);

								instItemRolePropInstIterationDataHash->incFillerInstanceItemCount(iteratorData->getVariableMappingIteratorCount());

								fillerIterator->moveNext();
							}

							if (fillerIterator) {
								double remainingFillerEstimationCount = fillerIterator->remainingInstancesEstimation().getEstimatedCount();
								double prevFillerEstimation = iteratorData->getExpectedFillerCount();
								double newFillerEstimation = iteratorData->getPropagationInstanceItemCount() + remainingFillerEstimationCount;
								iteratorData->setExpectedFillerCount(newFillerEstimation);
								double diffFillerEstimation = newFillerEstimation - prevFillerEstimation;
								instItemRolePropInstIterationDataHash->incFillerInstanceItemCount(diffFillerEstimation);
							}

							if (fillerIterator && fillerIterator->requiresInitialization()) {
								bindingsMappedVec.addRealizationSchedulingIterationData(iteratorData);
								//COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(fillerIterator);
								//schedulingRealizationIteratorDataList->append(iteratorData);
								//schedulingReqList->append(roleRealReq);
								iteratorData->setRealizationScheduled(true);
							} else if (!fillerIterator || fillerIterator->atEnd()) {
								if (fillerIterator) {
									instItemRolePropInstIterationDataHash->incPropagationHandledInstanceItemCount();
									delete fillerIterator;
									iteratorData->setRealizationIterator(nullptr);
								}
								CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* variableMappingIteratorLinker = iteratorData->getVariableMappingPropagationIteratorLinker();
								for (CXLinker<COptimizedComplexVariableIndividualBindingsCardinalityLinker*>* variableMappingIteratorLinkerIt = variableMappingIteratorLinker; variableMappingIteratorLinkerIt; variableMappingIteratorLinkerIt = variableMappingIteratorLinkerIt->getNext()) {
									COptimizedComplexVariableIndividualBindingsCardinalityLinker* varBindingCardLinker = variableMappingIteratorLinkerIt->getData();
									COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = varBindingCardLinker->getInitialCardinalities();
									instItemRolePropInstIterationDataHash->incPropagatedInstanceItemCount();

									CXLinker<CRealizationIndividualInstanceItemReference>* rolePropInstItemLinker = iteratorData->getPropagationInstanceItemLinker();
									for (CXLinker<CRealizationIndividualInstanceItemReference>* rolePropInstItemLinkerIt = rolePropInstItemLinker; rolePropInstItemLinkerIt; rolePropInstItemLinkerIt = rolePropInstItemLinkerIt->getNext()) {
										const CRealizationIndividualInstanceItemReference& propInstItem = rolePropInstItemLinkerIt->getData();
										instItemRolePropInstIterationDataHash->incRealizationFinishedFillerInstanceItemCount(iteratorData->getPropagationInstanceItemCount());
										propagatedVariableMappingCreationCopyingVecFunc(varBindingCardLinker, cardinalites, propInstItem, bindingsMappedVec, memMan, tmpJoinedBindings);
									}
								}
								iteratorData->clearVariableMappingPropagationIterator();


								CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* cardinalityUpdateIteratorLinker = iteratorData->getCardinalityUpdatePropagationIteratorLinker();
								for (CXLinker<COptimizedComplexVariableIndividualUpdateCardinalityLinker*>* cardinalityUpdateIteratorLinkerIt = cardinalityUpdateIteratorLinker; cardinalityUpdateIteratorLinkerIt; cardinalityUpdateIteratorLinkerIt = cardinalityUpdateIteratorLinkerIt->getNext()) {
									COptimizedComplexVariableIndividualUpdateCardinalityLinker* updatedCardinalityLinker = cardinalityUpdateIteratorLinkerIt->getData();

									COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = updatedCardinalityLinker->getPreviousCardinality();
									COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = updatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
									COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = updatedCardinalityLinker->getNewCardinality();
									COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();

									COptimizedComplexVariableIndividualBindingsCardinality tmpDiffCardinalites = *newCardinalites;
									tmpDiffCardinalites.substractCardinalities(prevCardinalites);

									CXLinker<CRealizationIndividualInstanceItemReference>* rolePropInstItemLinker = iteratorData->getPropagationInstanceItemLinker();
									for (CXLinker<CRealizationIndividualInstanceItemReference>* rolePropInstItemLinkerIt = rolePropInstItemLinker; rolePropInstItemLinkerIt; rolePropInstItemLinkerIt = rolePropInstItemLinkerIt->getNext()) {
										const CRealizationIndividualInstanceItemReference& propInstItem = rolePropInstItemLinkerIt->getData();
										propagatedVariableMappingCreationCopyingVecFunc(bindingLinker, &tmpDiffCardinalites, propInstItem, bindingsMappedVec, memMan, tmpJoinedBindings);
									}
								}
								iteratorData->clearCardinalityUpdatePropagationIterator();

							}						
						}
					};




					function<CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* const & keyMappedBindingsVecData)> bindingBatchesHashingPropagationMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>* const & keyMappedBindingsVecData) -> CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker> {

						CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker> bindingsMappedVec(bindingsMappingSize);

						COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* lastPropHash = nullptr;
						QTime* time = nullptr;
						if (mConfPerformanceLogging) {
							time = new QTime();
							time->start();
						}
						if (keyMappedBindingsVecData) {
							CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker* batchLinker = keyMappedBindingsVecData->getNextBatchLinker();
							COptimizedComplexVariableIndividualBindings* tmpJoinedBindings = nullptr;

							while (batchLinker) {

								cint64 mappedKeyIndex = batchLinker->getMappedKeyIndex();
								COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged*& instItemRolePropInstIterationDataHash = (*instItemRolePropInstIterationDataHashVector)[mappedKeyIndex];
								if (!instItemRolePropInstIterationDataHash) {
									instItemRolePropInstIterationDataHash = new COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged();
								}
								lastPropHash = instItemRolePropInstIterationDataHash;


								CRealizationIndividualInstanceItemReference instItemRef = batchLinker->getInstanceItemReference();

								COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iterationData = instItemRolePropInstIterationDataHash->getRolePropagationProcessingRealizationIteratorData(instItemRef);
							
								COptimizedComplexVariableIndividualBindings* varMapping = batchLinker->getVariableMapping();
								if (varMapping) {
									COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = batchLinker->getNewCardinalites();
									COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = batchLinker->getPreviousCardinalites();

									COptimizedComplexVariableIndividualBindingsCardinalityLinker* linker = CObjectAllocator< COptimizedComplexVariableIndividualBindingsCardinalityLinker >::allocateAndConstruct(instItemRolePropInstIterationDataHash->getMemoryAllocationManager());
									linker->initBindingsCardinalityLinker(varMapping, newCardinalites);
									iterationData->addVariableMappingPropagationIterator(linker);
									instItemRolePropInstIterationDataHash->mPropagatedCount++;
								}

								iterationDataHandleFunc(iterationData, instItemRolePropInstIterationDataHash, bindingsMappedVec, bindingsMappedVec.getMemoryAllocationManager(), tmpJoinedBindings);

								batchLinker = batchLinker->getNext();								
							}
						}

						if (lastPropHash && time) {
							lastPropHash->mProcessingTime += time->elapsed();
							delete time;
						}


						return bindingsMappedVec;
					};




					function<void(CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& reduceVec)> bindingBatchesHashingPropagationReduceFunc =
						[&](CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>& reduceVec) -> void {

						if (vec.size() < reduceVec.size()) {
							vec.resize(reduceVec.size());
						}
						for (cint64 i = 0; i < vec.count(); ++i) {
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>*& vecData = vec[i];
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>* const & reduceVecData = reduceVec[i];
							if (reduceVecData) {
								if (!vecData) {
									vecData = reduceVecData;
								} else {
									vecData->mergeBatchLinkers(reduceVecData);
								}
							}

						}
						vec.mergeRealizationSchedulingIterationDataLinkers(reduceVec);
						vec.appendMemoryPool(reduceVec.getMemoryPools());
					};







































					// map (& reduce) of step 3

					function<COptimizedComplexVariableIndividualMappingsMultiHashPart*(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>* const & mappedBindingsVecData)> bindingBatchesInsertingMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>* const & mappedBindingsVecData) -> COptimizedComplexVariableIndividualMappingsMultiHashPart* {



						QTime* time = nullptr;
						if (mConfPerformanceLogging) {
							time = new QTime();
							time->start();
						}

						COptimizedComplexVariableIndividualMappingsMultiHashPart* propagatedVarMappingMultiHashPart = nullptr;

						if (mappedBindingsVecData) {
							CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker* batchLinker = mappedBindingsVecData->getNextBatchLinker();

							while (batchLinker) {

								cint64 mappedBindingsIndex = batchLinker->getBindingsIndex();
								if (!propagatedVarMappingMultiHashPart) {
									propagatedVarMappingMultiHashPart = propagatedVarMapping->getMultiHashPart(mappedBindingsIndex);
								}


								COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = batchLinker->getBindingsCardinalityLinker();
								cint64 hashValue = batchLinker->getBindingHasher().getHashValue();

								propagatedVarMappingMultiHashPart->addInsertingBindingsCardinalityLinkerCopy(joinedLinker, hashValue, true);

								batchLinker = batchLinker->getNext();
							}
						}

						if (propagatedVarMappingMultiHashPart && time) {
							propagatedVarMappingMultiHashPart->mStatsProcessingTime += time->elapsed();
							delete time;
						}


						return propagatedVarMappingMultiHashPart;
					};




					function<void(const QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>& mappedVarMappingParts)> bindingBatchesInsertingReduceFunc =
						[&](const QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>& mappedVarMappingParts) -> void {

						for (QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>::const_iterator it = mappedVarMappingParts.constBegin(), itEnd = mappedVarMappingParts.constEnd(); it != itEnd; ++it) {
							COptimizedComplexVariableIndividualMappingsMultiHashPart* reduceMappPart = (*it);
							if (reduceMappPart) {
								propagatedVarMapping->updateLinkers(reduceMappPart);
								propagatedVarMapping->updateBindingCount(reduceMappPart);
							}
						}
					};




					function<void(CMemoryPool* memPools)> clearMemoryPoolsFunc = [&](CMemoryPool* memPools) -> void {
						while (memPools) {
							CMemoryPool* tmpMemPool = memPools;
							memPools = memPools->getNextMemoryPool();
							delete[] tmpMemPool->getMemoryBlockData();
							delete tmpMemPool;
						}
					};





























					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> mapReducedConcurrentPropagationComputation = [&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {
						if (!keyMappingBatchDataVector.isEmpty()) {

							QTime* stepTimer = nullptr;
							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker> mappedPropagationHashKeyVec = QtConcurrent::blockingMappedReduced<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedKeyBindingsCardinalityLinker>>(keyMappingBatchDataVector, bindingBatchesHashingKeyMapFunc, bindingBatchesHashingKeyReduceFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < keyMappingBatchDataVector.size(); ++i) {
									if (keyMappingBatchDataVector[i]) {
										++partitionsUsed;
									}
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 1 of concurrent propagation computation for item %3 with %4/%1 tasks took %2 ms.").arg(keyMappingBatchDataVector.size()).arg(stepTimer->elapsed()).arg(rolePropItem->getComputationStepId()).arg(partitionsUsed), this);
								delete stepTimer;
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker> mappedPropagatedHashKeyVec = QtConcurrent::blockingMappedReduced<CQtConcurrentVariableMappingsCompositionPropagationRealizationSchedulingBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationMappedPropagatedBindingsCardinalityLinker>>(mappedPropagationHashKeyVec, bindingBatchesHashingPropagationMapFunc, bindingBatchesHashingPropagationReduceFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedPropagationHashKeyVec.size(); ++i) {
									if (mappedPropagationHashKeyVec[i]) {
										++partitionsUsed;
									}
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 2 of concurrent propagation computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedPropagationHashKeyVec.size()).arg(stepTimer->elapsed()).arg(rolePropItem->getComputationStepId()).arg(partitionsUsed), this);
								delete stepTimer;

								cint64 statMaxJoiningCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalJoiningCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < mappedPropagationHashKeyVec.size(); ++i) {
									COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* instItemRolePropInstIterationDataHash = (*instItemRolePropInstIterationDataHashVector)[i];
									if (instItemRolePropInstIterationDataHash && instItemRolePropInstIterationDataHash->mPropagatedCount > 0) {
										++statUsedCount;
										statMaxJoiningCount = qMax(statMaxJoiningCount, instItemRolePropInstIterationDataHash->mPropagatedCount);
										statMaxProcessingTime = qMax(statMaxProcessingTime, instItemRolePropInstIterationDataHash->mProcessingTime);
										statTotalJoiningCount += instItemRolePropInstIterationDataHash->mPropagatedCount;
										statTotalProcessingTime += instItemRolePropInstIterationDataHash->mProcessingTime;
									}
								}
								cint64 averageJoiningCount = 0;
								if (statUsedCount > 0) {
									averageJoiningCount = statTotalJoiningCount / statUsedCount;
								}
								cint64 averageProcessingTime = 0;
								if (statUsedCount > 0) {
									averageProcessingTime = statTotalProcessingTime / statUsedCount;
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 2 of concurrent propagation computation for item %1 had in average %2 (max %3) propagated mappings and took in average %4 (max %5) ms.").arg(rolePropItem->getComputationStepId()).arg(averageJoiningCount).arg(statMaxJoiningCount).arg(averageProcessingTime).arg(statMaxProcessingTime), this);
							}
							CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* realSchedItDataLinker = mappedPropagatedHashKeyVec.getFirstRealizationSchedulingIterationDataLinker();
							for (CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* realSchedItDataLinkerIt = realSchedItDataLinker; realSchedItDataLinkerIt; realSchedItDataLinkerIt = realSchedItDataLinkerIt->getNext()) {
								COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iteratorData = realSchedItDataLinkerIt->getData();
								COntologyProcessingIteratorRealizationRequirement* roleRealReq = new COntologyProcessingIteratorRealizationRequirement(iteratorData->getRealizationIterator());
								schedulingRealizationIteratorDataList->append(iteratorData);
								schedulingReqList->append(roleRealReq);
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*> mappedVarMappingParts = QtConcurrent::blockingMapped<QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>>(mappedPropagatedHashKeyVec, bindingBatchesInsertingMapFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedPropagatedHashKeyVec.size(); ++i) {
									if (mappedPropagatedHashKeyVec[i]) {
										++partitionsUsed;
									}
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 3 of concurrent propagation computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedPropagatedHashKeyVec.size()).arg(stepTimer->elapsed()).arg(rolePropItem->getComputationStepId()).arg(partitionsUsed), this);
								delete stepTimer;

								cint64 statMaxInsertionCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalInsertionCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < mappedPropagatedHashKeyVec.size(); ++i) {
									COptimizedComplexVariableIndividualMappingsMultiHashPart* joinedVarMappingMultiHashPart = propagatedVarMapping->getMultiHashPart(i);
									if (joinedVarMappingMultiHashPart && joinedVarMappingMultiHashPart->mStatInsertionCount > 0) {
										++statUsedCount;
										statMaxInsertionCount = qMax(statMaxInsertionCount, joinedVarMappingMultiHashPart->mStatInsertionCount);
										statMaxProcessingTime = qMax(statMaxProcessingTime, joinedVarMappingMultiHashPart->mStatsProcessingTime);
										statTotalInsertionCount += joinedVarMappingMultiHashPart->mStatInsertionCount;
										statTotalProcessingTime += joinedVarMappingMultiHashPart->mStatsProcessingTime;
									}
								}
								cint64 averageInsertionCount = 0;
								if (statUsedCount > 0) {
									averageInsertionCount = statTotalInsertionCount / statUsedCount;
								}
								cint64 averageProcessingTime = 0;
								if (statUsedCount > 0) {
									averageProcessingTime = statTotalProcessingTime / statUsedCount;
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 3 of concurrent propagation computation for item %1 had in average %2 (max %3) bindings insertions and took in average %4 (max %5) ms.").arg(rolePropItem->getComputationStepId()).arg(averageInsertionCount).arg(statMaxInsertionCount).arg(averageProcessingTime).arg(statMaxProcessingTime), this);
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							bindingBatchesInsertingReduceFunc(mappedVarMappingParts);
							CMemoryPool* mappedKeyBatchHashVecMemPools = keyMappingBatchDataVector.takeMemoryPools();
							CMemoryPool* mappedJoiningHashVecMemPools = mappedPropagationHashKeyVec.takeMemoryPools();
							CMemoryPool* mappedJoinedHashVecMemPools = mappedPropagatedHashKeyVec.takeMemoryPools();
							CConcurrentTaskScheduler::run([=] {
								clearMemoryPoolsFunc(mappedKeyBatchHashVecMemPools);
							});
							CConcurrentTaskScheduler::run([=] {
								clearMemoryPoolsFunc(mappedJoiningHashVecMemPools);
							});
							CConcurrentTaskScheduler::run([=] {
								clearMemoryPoolsFunc(mappedJoinedHashVecMemPools);
							});
							keyMappingBatchDataVector.clear();
							keyMappingBatchDataVector.resize(keyMappingBatchDataVector.getIndexSize());
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedVarMappingParts.size(); ++i) {
									if (mappedVarMappingParts[i]) {
										++partitionsUsed;
									}
								}
								LOG(INFO, getLogDomain(), logTr("Propagation step 4 of concurrent propagation computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedVarMappingParts.size()).arg(stepTimer->elapsed()).arg(rolePropItem->getComputationStepId()).arg(partitionsUsed), this);
								delete stepTimer;
							}

						}
					};







































					bool successorCollectionContinued = false;

					if (!waitingProcessedRealizationIteratorDataList->isEmpty() && rolePropItem->requiresMoreVariableMappingsComputation()) {
						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker> keyMappingBatchDataVector(currentBatchSizeCount);
						CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* contLinker = nullptr;
						cint64 currentContLinkerCount = 0;
						while (!waitingProcessedRealizationIteratorDataList->isEmpty() && rolePropItem->requiresMoreVariableMappingsComputation()) {
							COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged* iteratorData = (COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*)waitingProcessedRealizationIteratorDataList->takeFirst();
							iteratorData->getRealizationIterator()->begin();
							iteratorData->setRealizationScheduled(false);
							rolePropItem->decScheduledRealizationCount();

							CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*>* linker = CObjectAllocator< CXLinker<COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataMemoryManaged*> >::allocateAndConstruct(keyMappingBatchDataVector.getMemoryAllocationManager());
							linker->initLinker(iteratorData);

							currentContLinkerCount++;
							contLinker = linker->append(contLinker);

							if (currentContLinkerCount >= currentBatchSizeLimit) {
								CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
								CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* data = nullptr;
								if (!mConfPartitionizedMemoryManagement) {
									data = keyMappingBatchDataVector.createBatchLinker();
								} else {
									data = vecData->createBatchLinker();
								}
								data->addContinueIterationDataLinker(contLinker);
								vecData->addBatchLinker(data);
								contLinker = nullptr;
								currentContLinkerCount = 0;
							}

							successorCollectionContinued = true;
						}

						if (contLinker) {
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
							CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker* data = nullptr;
							if (!mConfPartitionizedMemoryManagement) {
								data = keyMappingBatchDataVector.createBatchLinker();
							} else {
								data = vecData->createBatchLinker();
							}
							data->addContinueIterationDataLinker(contLinker);
							vecData->addBatchLinker(data);
						}

						mapReducedConcurrentPropagationComputation(keyMappingBatchDataVector);
					}



					bool mappingProcessed = false;
					CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker> keyMappingBatchDataVector(currentBatchSizeCount);
					while (rolePropItem->requiresMoreVariableMappingsComputation() && (!baseItemDep->isBatchProcessed() || baseItemDep->loadNextBatch())) {



						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionPropagationBindingsCardinalityLinkerBatchLinker> keyMappingBatchDataVector(currentBatchSizeCount);
						while (rolePropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							mappingProcessed = true;
							propagationMappingsBatchingFunc(baseItemDep, keyMappingBatchDataVector);
						}
						mapReducedConcurrentPropagationComputation(keyMappingBatchDataVector);


						while (rolePropItem->requiresMoreVariableMappingsComputation() && baseItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							mappingProcessed = true;
							propagationCardinalitesBatchingFunc(baseItemDep, keyMappingBatchDataVector);
						}
						mapReducedConcurrentPropagationComputation(keyMappingBatchDataVector);


					}



					rolePropItem->setFillerInstanceItemCount(0);
					rolePropItem->setRealizationFinishedFillerInstanceItemCount(0);
					rolePropItem->setPropagationHandledInstanceItemCount(0);
					rolePropItem->setPropagatedInstanceItemCount(0);
					for (cint64 i = 0; i < mConfConcurrentPropagationHashCount; ++i) {
						COptimizedComplexVariableRolePropagationProcessingRealizationIteratorDataHashMemoryManaged* instItemRolePropInstIterationDataHash = (*instItemRolePropInstIterationDataHashVector)[i];
						if (instItemRolePropInstIterationDataHash) {
							rolePropItem->incFillerInstanceItemCount(instItemRolePropInstIterationDataHash->getFillerInstanceItemCount());
							rolePropItem->incRealizationFinishedFillerInstanceItemCount(instItemRolePropInstIterationDataHash->getRealizationFinishedFillerInstanceItemCount());
							rolePropItem->incPropagationHandledInstanceItemCount(instItemRolePropInstIterationDataHash->getPropagationHandledInstanceItemCount());
							rolePropItem->incPropagatedInstanceItemCount(instItemRolePropInstIterationDataHash->getPropagatedInstanceItemCount());
						}
					}




					propagatedVarMapping->incCurrentUpdateId();



					return mappingProcessed;

				}






			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
