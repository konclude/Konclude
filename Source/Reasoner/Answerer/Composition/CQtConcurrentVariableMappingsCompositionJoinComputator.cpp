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

#include "CQtConcurrentVariableMappingsCompositionJoinComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CQtConcurrentVariableMappingsCompositionJoinComputator::CQtConcurrentVariableMappingsCompositionJoinComputator() {
					mComputerName = "Concurrent Qt join";
				}


				CAbstractVariableMappingsCompositionComputator* CQtConcurrentVariableMappingsCompositionJoinComputator::configureComputator(COptimizedComplexExpressionOntologyAnsweringItem* ontoAnsweringItem, CAbstractVariableMappingsCompositionItemRequirementProcessor* reqProcessor, CAnswererContext* answererContext) {
					CAbstractVariableMappingsCompositionJoinComputator::configureComputator(ontoAnsweringItem, reqProcessor, answererContext);
					mConcurrentJoinComputationTaskCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinComputationTaskCount", 211);
					mConfBatchMaxHandlingCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinComputationMaxBatchHandlingCount", 10000);
					mConfSplitTestingBatchMaxHandlingCount = CConfigDataReader::readConfigInteger(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinLastStepSplitTestingComputationMaxBatchHandlingCount", 1000);
					mConfPartitionizedMemoryManagement = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinPartitionizedMemoryManagement", false);
					mConfPerformanceLogging = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinPerformanceLogging", false);
					mConfCheckingSideDirectJoining = CConfigDataReader::readConfigBoolean(ontoAnsweringItem->getCalculationConfiguration(), "Konclude.Answering.ConcurrentJoinDirectJoiningForCheckingSide", false);
					mConfConcurrentJoiningHashCount = mConcurrentJoinComputationTaskCount;
					mConfConcurrentVariableMappingPartCount = mConcurrentJoinComputationTaskCount;
					mConfBatchSizeLimit = 200;
					mConfBatchSizeCount = mConcurrentJoinComputationTaskCount;
					return this;
				}
				

				bool CQtConcurrentVariableMappingsCompositionJoinComputator::computeVariableMappingsJoinComposition(COptimizedComplexVariableJoiningItem* joiningItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {

					COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
					COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

					COptimizedComplexVariableIndividualMappings* leftVarMapping = leftItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappings* rightVarMapping = rightItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappingsMultiHash* joinedVarMapping = (COptimizedComplexVariableIndividualMappingsMultiHash*)joiningItem->getVariableMapping();

					cint64 leftSize = leftVarMapping->getBindingSize();
					cint64 rightSize = rightVarMapping->getBindingSize();
					cint64 joinSize = joinedVarMapping->getBindingSize();


					COptimizedComplexVariableJoiningBindingPositionMapping* variablePositionMapping = joiningItem->getPositionMapping();
					
					bool itemSameVarReductionJoining = variablePositionMapping->isSelfSameVariableReduction();
					cint64* rightPosMapArray = variablePositionMapping->getRightPosMapArray();
					cint64* leftPosMapArray = variablePositionMapping->getLeftPosMapArray();

					CXLinker<cint64>* leftKeyBindingLinker = variablePositionMapping->getLeftKeyBindingLinker();
					CXLinker<cint64>* rightKeyBindingLinker = variablePositionMapping->getRightKeyBindingLinker();



					QVector<COptimizedComplexVariableJoiningHashMemoryManaged*>& joiningHashVector = joiningItem->getJoiningHashVector();
					if (joiningHashVector.size() < mConfConcurrentJoiningHashCount) {
						joiningHashVector.resize(mConfConcurrentJoiningHashCount);
					}



					COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();


					cint64& leftSampleKeyCount = joiningItem->getLeftSampleKeyCount();
					cint64& rightSampleKeyCount = joiningItem->getRightSampleKeyCount();

					cint64& leftSampleInsertionCount = joiningItem->getLeftSampleInsertionCount();
					cint64& rightSampleInsertionCount = joiningItem->getRightSampleInsertionCount();

					QAtomicInt atomicLeftSampleKeyCount(leftSampleKeyCount);
					QAtomicInt atomicRightSampleKeyCount(rightSampleKeyCount);



					bool reduction = variablePositionMapping->isVariableReduction();
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();



					auto joiningVariableMappingCreationFunc = [&](	COptimizedComplexVariableIndividualBindings* leftvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* leftNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* leftPrevCardinalites, 
																	COptimizedComplexVariableIndividualBindings* rightvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* rightNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* righPrevCardinalites, 
																	COptimizedComplexVariableIndividualBindings* joinedBindings, COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker, COptimizedComplexVariableIndividualBindingsCardinality* providedJoinedCardinalites, CMemoryAllocationManager* memMan) -> COptimizedComplexVariableIndividualBindingsCardinalityLinker* {


						COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;

						COptimizedComplexVariableIndividualBindingsCardinality tmpLeftCardinalites;
						COptimizedComplexVariableIndividualBindingsCardinality tmpRightCardinalites;

						COptimizedComplexVariableIndividualBindingsCardinality* leftCardinalites = nullptr;
						COptimizedComplexVariableIndividualBindingsCardinality* rightCardinalites = nullptr;

						bool cardCreated = false;

						if (leftNewCardinalites && leftPrevCardinalites) {
							tmpLeftCardinalites = *leftNewCardinalites;
							tmpLeftCardinalites.substractCardinalities(leftPrevCardinalites);
							leftCardinalites = &tmpLeftCardinalites;
						} else if (leftNewCardinalites && !leftPrevCardinalites) {
							tmpLeftCardinalites = *leftNewCardinalites;
							leftCardinalites = &tmpLeftCardinalites;
						}

						if (rightNewCardinalites && righPrevCardinalites) {
							tmpRightCardinalites = *rightNewCardinalites;
							tmpRightCardinalites.substractCardinalities(righPrevCardinalites);
							rightCardinalites = &tmpRightCardinalites;
						} else if (rightNewCardinalites && !righPrevCardinalites) {
							tmpRightCardinalites = *rightNewCardinalites;
							rightCardinalites = &tmpRightCardinalites;
						}


						TIndividualInstanceItemDataBinding* leftBinding = leftvarMapping->getBindingArray();
						TIndividualInstanceItemDataBinding* rightBinding = rightvarMapping->getBindingArray();

						if (!joinedCardinalites && (leftCardinalites || rightCardinalites || reduction)) {
							joinedCardinalites = providedJoinedCardinalites;
							if (!joinedCardinalites) {
								joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(joiningItem, buildingVarItem, memMan);
							}
							cardCreated = true;
						}
						if (leftCardinalites && rightCardinalites) {
							*joinedCardinalites = *rightCardinalites;
							joinedCardinalites->setSameIndividualsJointlyConsideredCardinality(joinedCardinalites->getSameIndividualsJointlyConsideredCardinality() * leftCardinalites->getSameIndividualsJointlyConsideredCardinality());
							joinedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(joinedCardinalites->getSameIndividualsSeparatlyConsideredCardinality() * leftCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
						} else if (rightCardinalites) {
							*joinedCardinalites = *rightCardinalites;
						} else if (leftCardinalites) {
							*joinedCardinalites = *leftCardinalites;
						} else if (joinedCardinalites) {
							joinedCardinalites->initCardinalities(1, 1);
						}

						if (joinedCardinalites) {
							joinedCardinalites->setCardinalityUpdateId(-1);
						}


						if (!joinedBindings) {
							joinedBindings = createBindingsForVariableCompositionItems(joinSize, joiningItem, buildingVarItem, memMan);
						}

						for (cint64 i = 0; i < leftSize; ++i) {
							if (leftPosMapArray[i] > -1) {
								joinedBindings->setBinding(leftPosMapArray[i], leftBinding[i]);
							} else if (leftPosMapArray[i] == -2) {
								TIndividualInstanceItemDataBinding& reducedBinding = leftBinding[i];
								CInstanceBindingIndividualCountingVisitor visitor;
								sameRealization->visitSameIndividuals(reducedBinding.reference, &visitor);
								if (visitor.individualCount > 1) {
									joinedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(visitor.individualCount * joinedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
								}
							}
						}
						for (cint64 i = 0; i < rightSize; ++i) {
							if (rightPosMapArray[i] > -1) {
								joinedBindings->setBinding(rightPosMapArray[i], rightBinding[i]);
							} else if (rightPosMapArray[i] == -2) {
								TIndividualInstanceItemDataBinding& reducedBinding = rightBinding[i];
								CInstanceBindingIndividualCountingVisitor visitor;
								sameRealization->visitSameIndividuals(reducedBinding.reference, &visitor);
								if (visitor.individualCount > 1) {
									joinedCardinalites->setSameIndividualsSeparatlyConsideredCardinality(visitor.individualCount * joinedCardinalites->getSameIndividualsSeparatlyConsideredCardinality());
								}
							}
						}

						//QStringList bindingsStringList;
						//for (cint64 i = 0; i < joinSize; ++i) {
						//	bindingsStringList.append(CIRIName::getRecentIRIName(joinedBindings->getBinding(i).reference.getIndividual()->getIndividualNameLinker()));
						//}
						//QString bindingsString = bindingsStringList.join("\t");

						if (!joinedLinker) {
							joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, joiningItem, buildingVarItem, memMan);
						} else {
							joinedLinker->setInitialCardinality(joinedCardinalites);
						}
						return joinedLinker;
					};

					auto joiningVariableMappingCreationLeftRightFunc = [&]( COptimizedComplexVariableIndividualBindings* leftvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* leftNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* leftPrevCardinalites, 
																			COptimizedComplexVariableIndividualBindings* rightvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* rightNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* righPrevCardinalites, bool swap, 
																			COptimizedComplexVariableIndividualBindings* joinedBindings, COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker, COptimizedComplexVariableIndividualBindingsCardinality* providedJoinedCardinalites, CMemoryAllocationManager* memMan) -> COptimizedComplexVariableIndividualBindingsCardinalityLinker* {
						if (swap) {
							return joiningVariableMappingCreationFunc(rightvarMapping, rightNewCardinalites, righPrevCardinalites, leftvarMapping, leftNewCardinalites, leftPrevCardinalites, joinedBindings, joinedLinker, providedJoinedCardinalites, memMan);
						} else {
							return joiningVariableMappingCreationFunc(leftvarMapping, leftNewCardinalites, leftPrevCardinalites, rightvarMapping, rightNewCardinalites, righPrevCardinalites, joinedBindings, joinedLinker, providedJoinedCardinalites, memMan);
						}
					};


					auto joiningVariableMappingCreationLeftRightLinkerFunc = [&]( COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* leftLinker, COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* rightLinker, bool swap, 
																				  COptimizedComplexVariableIndividualBindings* joinedBindings, COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker, COptimizedComplexVariableIndividualBindingsCardinality* providedJoinedCardinalites, CMemoryAllocationManager* memMan) -> COptimizedComplexVariableIndividualBindingsCardinalityLinker* {
						return joiningVariableMappingCreationLeftRightFunc(leftLinker->getBindings(), leftLinker->getNewCardinalities(), leftLinker->getPreviousCardinalities(), rightLinker->getBindings(), rightLinker->getNewCardinalities(), rightLinker->getPreviousCardinalities(), swap, joinedBindings, joinedLinker, providedJoinedCardinalites, memMan);
					};















					//QList<QString> images;
					//std::function<QString(const QString &)> mapFunc = [](const QString &img) {
					//	return img + "x";
					//};
					//std::function<void(QString &, const QString &)> redFunc = [](QString &img, const QString &red) {
					//};
					//QFuture<QString> thumbnails = QtConcurrent::mapped(images, mapFunc);








					cint64 currentBatchSizeCount = mConfBatchSizeCount;
					cint64 currentBatchSizeLimit = mConfBatchSizeLimit;




					function<void(bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, cint64& sampleInsertionCount, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> joiningHashSampleBatchingFunc =
						[&](bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, cint64& sampleInsertionCount, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {

						cint64 batchSize = 1;
						++sampleInsertionCount;
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* firstBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
						COptimizedComplexVariableIndividualBindingsCardinalityLinker* lastBindingLinker = firstBindingLinker;
						while (batchSize++ < currentBatchSizeLimit && itemDep->getBatchCurrentBindingsCardinalityLinker(false) && sampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
							++sampleInsertionCount;
							lastBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
						}

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
						CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* data = nullptr;
						if (!mConfPartitionizedMemoryManagement) {
							data = keyMappingBatchDataVector.createBatchLinker();
						} else {
							data = vecData->createBatchLinker();
						}
						data->setBatchBindingsCardinalityLinkers(firstBindingLinker, lastBindingLinker);
						data->setInserting(true);
						data->setLeftSide(left);
						vecData->addBatchLinker(data);
					};


					cint64 batchMaxHandlingCount = mConfBatchMaxHandlingCount;
					if (mConfAllowSplitModeActivation && joiningItem->isLastComputation() && !joiningItem->isSplitComputationMode()) {
						batchMaxHandlingCount = mConfSplitTestingBatchMaxHandlingCount;
					}


					function<void(bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, bool inserting, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> joiningHashBindingsInsertingCheckingBatchingFunc =
						[&](bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, bool inserting, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {

						cint64 batchHandlingCount = 0;
						while (batchHandlingCount++ < batchMaxHandlingCount && itemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
							cint64 batchSize = 1;
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* firstBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* lastBindingLinker = firstBindingLinker;
							while (batchSize++ < currentBatchSizeLimit && itemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
								lastBindingLinker = itemDep->getBatchCurrentBindingsCardinalityLinker(true);
							}

							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
							CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* data = nullptr;
							if (!mConfPartitionizedMemoryManagement) {
								data = keyMappingBatchDataVector.createBatchLinker();
							} else {
								data = vecData->createBatchLinker();
							}
							data->setBatchBindingsCardinalityLinkers(firstBindingLinker, lastBindingLinker);
							data->setInserting(inserting);
							data->setLeftSide(left);
							vecData->addBatchLinker(data);
						}
					};



					function<void(bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, bool inserting, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> joiningHashCardinalitiesInsertingCheckingBatchingFunc =
						[&](bool left, COptimizedComplexVariableCompositionItemDependence* itemDep, bool inserting, CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {

						cint64 batchHandlingCount = 0;
						while (batchHandlingCount++ < batchMaxHandlingCount && itemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
							cint64 batchSize = 1;
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* firstCardinalityLinker = itemDep->getBatchCurrentUpdatedCardinalityLinker(true);
							COptimizedComplexVariableIndividualUpdateCardinalityLinker* lastCardinalityLinker = firstCardinalityLinker;
							while (batchSize++ < currentBatchSizeLimit && itemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
								lastCardinalityLinker = itemDep->getBatchCurrentUpdatedCardinalityLinker(true);
							}
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* vecData = keyMappingBatchDataVector.getVectorLinkerData(keyMappingBatchDataVector.getNextIndex(true), true);
							CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* data = nullptr;
							if (!mConfPartitionizedMemoryManagement) {
								data = keyMappingBatchDataVector.createBatchLinker();
							} else {
								data = vecData->createBatchLinker();
							}
							data->setBatchCardinalityUpdateLinkers(firstCardinalityLinker, lastCardinalityLinker);
							data->setInserting(inserting);
							data->setLeftSide(left);
							vecData->addBatchLinker(data);
						}
					};





					bool splitComputationMode = joiningItem->isSplitComputationMode();
					cint64 leftSplitCount = joiningItem->getLeftSplitCount();
					cint64 rightSplitCount = joiningItem->getRightSplitCount();
					cint64 leftSplitPos = joiningItem->getLeftSplitPosition();
					cint64 rightSplitPos = joiningItem->getRightSplitPosition();

					CXLinker<cint64>* leftRemainingBindingLinker = nullptr;
					CXLinker<cint64>* rightRemainingBindingLinker = nullptr;

					if (splitComputationMode) {
						leftRemainingBindingLinker = variablePositionMapping->getLeftRemainingBindingLinker();
						rightRemainingBindingLinker = variablePositionMapping->getRightRemainingBindingLinker();
					}

					cint64 splitComputationCount = 0;







					// map & reduce of step 1
					cint64 keyMappingSize = mConfConcurrentJoiningHashCount;

					function<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* const & batchData)> bindingBatchesHashingKeyMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* const & batchData) ->CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker> {

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker> keyMappedVec(keyMappingSize);
						if (batchData) {

							CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* batchLinker = batchData->getNextBatchLinker();
							while (batchLinker) {

								bool leftSide = batchLinker->isLeftSide();
								bool inserting = batchLinker->isInserting();

								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								while (bindingsCardinalityCurrentLinker) {

									CXLinker<cint64>* keyBindingLinker = leftKeyBindingLinker;
									CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
									cint64 splitCount = leftSplitCount;
									cint64 splitPos = leftSplitPos;
									if (!leftSide) {
										keyBindingLinker = rightKeyBindingLinker;
										remainingBindingLinker = rightRemainingBindingLinker;
										splitPos = rightSplitPos;
										splitCount = rightSplitCount;
									}

									COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(bindingsCardinalityCurrentLinker->getBindings()->getBindingArray(), keyBindingLinker, remainingBindingLinker);
									cint64 keyHashValue = bindingHasher.getHashValue();
									uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);
									cint64 mappedKeyIndex = uintHashValue % keyMappingSize;

									if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {

										CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>* keyMappedVecData = keyMappedVec.getVectorLinkerData(mappedKeyIndex, true);
										CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* mappedKeyDataLinker = nullptr;
										if (!mConfPartitionizedMemoryManagement) {
											mappedKeyDataLinker = keyMappedVec.createBatchLinker();
										} else {
											mappedKeyDataLinker = keyMappedVecData->createBatchLinker();
										}
										mappedKeyDataLinker->initMappedKeysBindingsCardinalityData(bindingHasher, mappedKeyIndex, bindingsCardinalityCurrentLinker->getBindings(), bindingsCardinalityCurrentLinker->getInitialCardinalities(), nullptr, leftSide, inserting);
										keyMappedVecData->addBatchLinker(mappedKeyDataLinker);

									}
									bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								}



								COptimizedComplexVariableIndividualUpdateCardinalityLinker* cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								while (cardinalityUpdateLinker) {

									CXLinker<cint64>* keyBindingLinker = leftKeyBindingLinker;
									CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
									cint64 splitCount = leftSplitCount;
									cint64 splitPos = leftSplitPos;
									if (!leftSide) {
										keyBindingLinker = rightKeyBindingLinker;
										remainingBindingLinker = rightRemainingBindingLinker;
										splitPos = rightSplitPos;
										splitCount = rightSplitCount;
									}

									COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings()->getBindingArray(), keyBindingLinker, remainingBindingLinker);
									cint64 keyHashValue = bindingHasher.getHashValue();
									uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);
									cint64 mappedKeyIndex = uintHashValue % keyMappingSize;

									if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {
										CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>* keyMappedVecData = keyMappedVec.getVectorLinkerData(mappedKeyIndex, true);
										CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* mappedKeyDataLinker = nullptr;
										if (!mConfPartitionizedMemoryManagement) {
											mappedKeyDataLinker = keyMappedVec.createBatchLinker();
										} else {
											mappedKeyDataLinker = keyMappedVecData->createBatchLinker();
										}
										mappedKeyDataLinker->initMappedKeysBindingsCardinalityData(bindingHasher, mappedKeyIndex, cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings(), cardinalityUpdateLinker->getNewCardinality(), cardinalityUpdateLinker->getPreviousCardinality(), leftSide, inserting);
										keyMappedVecData->addBatchLinker(mappedKeyDataLinker);

									}
									cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								}


								batchLinker = batchLinker->getNext();
							}
						}

						return keyMappedVec;
					};


					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>& reduceVec)> bindingBatchesHashingKeyReduceFunc =
						[&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>& reduceVec) -> void {

						if (vec.size() < reduceVec.size()) {
							vec.resize(reduceVec.size());
						}
						for (cint64 i = 0; i < vec.count(); ++i) {
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>*& vecData = vec[i];
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>* const & reduceVecData = reduceVec[i];
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


					auto joinedVariableMappingCreationRightLeftCopyingVecInsertionFunc = [&](COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* leftLinker, COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* rightLinker, bool swap,
																							 CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>& bindingsMappedVec, CMemoryAllocationManager* memMan, COptimizedComplexVariableIndividualBindings*& tmpJoinedBindings) -> void {
						

						CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker* mappedBindingsDataLinker = nullptr;
						if (!mConfPartitionizedMemoryManagement) {
							mappedBindingsDataLinker = bindingsMappedVec.createBatchLinker();

							COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = joiningVariableMappingCreationLeftRightLinkerFunc(leftLinker, rightLinker, swap, nullptr, nullptr, nullptr, memMan);

							COptimizedComplexVariableIndividualBindingsHasher bindingsHasher(joinedLinker);
							cint64 keyHashValue = bindingsHasher.getHashValue();
							cint64 mappedBindingsIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % bindingsMappingSize;

							mappedBindingsDataLinker->initMappedJoinedBindingsCardinalityData(mappedBindingsIndex, bindingsHasher, joinedLinker);
							bindingsMappedVec.addBatchLinker(mappedBindingsIndex, mappedBindingsDataLinker);

						} else {

							if (!tmpJoinedBindings) {
								tmpJoinedBindings = createBindingsForVariableCompositionItems(joinSize, joiningItem, buildingVarItem, memMan);
							}
							COptimizedComplexVariableIndividualBindingsCardinalityLinker tmpJoinedLinker;
							COptimizedComplexVariableIndividualBindingsCardinality tmpJoinedCardinalites;

							joiningVariableMappingCreationLeftRightLinkerFunc(leftLinker, rightLinker, swap, tmpJoinedBindings, &tmpJoinedLinker, &tmpJoinedCardinalites, memMan);

							COptimizedComplexVariableIndividualBindingsHasher tmpBindingsHasher(&tmpJoinedLinker);
							cint64 keyHashValue = tmpBindingsHasher.getHashValue();
							cint64 mappedBindingsIndex = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue) % bindingsMappingSize;

							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>* bindingsMappedVecData = bindingsMappedVec.getVectorLinkerData(mappedBindingsIndex, true);

							// copy data with corresponding memory manager
							memMan = bindingsMappedVecData->getMemoryAllocationManager();
							COptimizedComplexVariableIndividualBindings* joinedBindings = createBindingsForVariableCompositionItems(joinSize, joiningItem, buildingVarItem, memMan);
							for (cint64 i = 0; i < joinSize; ++i) {
								joinedBindings->setBinding(i, tmpJoinedBindings->getBinding(i));
							}
							COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;
							if (tmpJoinedLinker.getCurrentCardinalities()) {
								joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(joiningItem, buildingVarItem, memMan);
								*joinedCardinalites = tmpJoinedCardinalites;
							}
							COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, joiningItem, buildingVarItem, memMan);
							COptimizedComplexVariableIndividualBindingsHasher bindingsHasher(joinedLinker, keyHashValue);

							mappedBindingsDataLinker = bindingsMappedVecData->createBatchLinker();
							mappedBindingsDataLinker->initMappedJoinedBindingsCardinalityData(mappedBindingsIndex, bindingsHasher, joinedLinker);
							bindingsMappedVecData->addBatchLinker(mappedBindingsDataLinker);
						}
					};




					function<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>* const & keyMappedBindingsVecData)> bindingBatchesHashingJoinMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>* const & keyMappedBindingsVecData) -> CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> {

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> bindingsMappedVec(bindingsMappingSize);

						COptimizedComplexVariableJoiningHashMemoryManaged* lastJoiningHash = nullptr;
						QTime* time = nullptr;
						if (mConfPerformanceLogging) {
							time = new QTime();
							time->start();
						}
						if (keyMappedBindingsVecData) {
							CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker* batchLinker = keyMappedBindingsVecData->getNextBatchLinker();
							COptimizedComplexVariableIndividualBindings* tmpJoinedBindings = nullptr;

							while (batchLinker) {

								cint64 mappedKeyIndex = batchLinker->getMappedKeyIndex();
								COptimizedComplexVariableJoiningHashMemoryManaged*& joiningHash = joiningHashVector[mappedKeyIndex];
								if (!joiningHash) {
									joiningHash = new COptimizedComplexVariableJoiningHashMemoryManaged(leftKeyBindingLinker, rightKeyBindingLinker);
								}
								lastJoiningHash = joiningHash;

								const COptimizedComplexVariableJoiningHasher& bindingHasher = batchLinker->getBindingHasher();

								bool sampling = !joiningItem->isSamplingCompleted();
								bool leftSide = batchLinker->isLeftSide();
								bool inserting = batchLinker->isInserting();



								COptimizedComplexVariableIndividualBindings* varMapping = batchLinker->getVariableMapping();
								COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = batchLinker->getNewCardinalites();
								COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = batchLinker->getPreviousCardinalites();

								COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker = joiningHash->createDataLinker();
								if (inserting) {
									COptimizedComplexVariableJoiningData& joiningData = (*joiningHash)[bindingHasher];

									if (sampling && !joiningData.hasBindings(leftSide)) {
										// TODO: make thread/concurrent safe
										if (leftSide) {
											atomicLeftSampleKeyCount.ref();
										} else {
											atomicRightSampleKeyCount.ref();
										}
									}

									linker->initLinker(varMapping, newCardinalites, prevCardinalites);
#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
									joiningData.mInsertionCount++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
									joiningHash->mInsertionCount++;
									for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!leftSide); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
										joinedVariableMappingCreationRightLeftCopyingVecInsertionFunc(existingOtherLinkerIt, linker, leftSide, bindingsMappedVec, bindingsMappedVec.getMemoryAllocationManager(), tmpJoinedBindings);
#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningData.mJoinedCount++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningHash->mJoinedCount++;
									}
									joiningData.addBindingLinker(leftSide, linker);
									if (leftSide) {
#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningData.mLeftAdded++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningHash->mLeftAdded++;
									} else {
#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningData.mRightAdded++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningHash->mRightAdded++;
									}
								} else {
									const COptimizedComplexVariableJoiningData& joiningData = joiningHash->value(bindingHasher);

#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
									joiningData.mCheckingCount++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
									joiningHash->mCheckingCount++;
									COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker tmpLinker(varMapping, newCardinalites, prevCardinalites);
									for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!leftSide); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
										joinedVariableMappingCreationRightLeftCopyingVecInsertionFunc(existingOtherLinkerIt, &tmpLinker, leftSide, bindingsMappedVec, bindingsMappedVec.getMemoryAllocationManager(), tmpJoinedBindings);
#ifdef CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningData.mJoinedCount++;
#endif //CONCURRENT_JOIN_COMPUTATION_PERFORMANCE_MEASUREMENT
										joiningHash->mJoinedCount++;
									}
								}

								batchLinker = batchLinker->getNext();
							}
						}

						if (lastJoiningHash && time) {
							lastJoiningHash->mProcessingTime += time->elapsed();
							delete time;
						}


						return bindingsMappedVec;
					};




					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>& reduceVec)> bindingBatchesHashingJoinReduceFunc =
						[&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>& vec, const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>& reduceVec) -> void {

						if (vec.size() < reduceVec.size()) {
							vec.resize(reduceVec.size());
						}
						for (cint64 i = 0; i < vec.count(); ++i) {
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>*& vecData = vec[i];
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>* const & reduceVecData = reduceVec[i];
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









					// map (& reduce) of step 3

					function<COptimizedComplexVariableIndividualMappingsMultiHashPart*(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>* const & mappedBindingsVecData)> bindingBatchesInsertingMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>* const & mappedBindingsVecData) -> COptimizedComplexVariableIndividualMappingsMultiHashPart* {



						QTime* time = nullptr;
						if (mConfPerformanceLogging) {
							time = new QTime();
							time->start();
						}

						COptimizedComplexVariableIndividualMappingsMultiHashPart* joinedVarMappingMultiHashPart = nullptr;

						if (mappedBindingsVecData) {

							CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker* batchLinker = mappedBindingsVecData->getNextBatchLinker();

							while (batchLinker) {

								cint64 mappedBindingsIndex = batchLinker->getBindingsIndex();
								if (!joinedVarMappingMultiHashPart) {
									joinedVarMappingMultiHashPart = joinedVarMapping->getMultiHashPart(mappedBindingsIndex);
								}


								COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = batchLinker->getBindingsCardinalityLinker();
								cint64 hashValue = batchLinker->getBindingHasher().getHashValue();

								joinedVarMappingMultiHashPart->addInsertingBindingsCardinalityLinkerCopy(joinedLinker, hashValue, true);

								batchLinker = batchLinker->getNext();
							}
						}

						if (joinedVarMappingMultiHashPart && time) {
							joinedVarMappingMultiHashPart->mStatsProcessingTime += time->elapsed();
							delete time;
						}


						return joinedVarMappingMultiHashPart;
					};




					function<void(const QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>& mappedVarMappingParts)> bindingBatchesInsertingReduceFunc =
						[&](const QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>& mappedVarMappingParts) -> void {

						for (QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>::const_iterator it = mappedVarMappingParts.constBegin(), itEnd = mappedVarMappingParts.constEnd(); it != itEnd; ++it) {
							COptimizedComplexVariableIndividualMappingsMultiHashPart* reduceMappPart = (*it);
							if (reduceMappPart) {
								joinedVarMapping->updateLinkers(reduceMappPart);
								joinedVarMapping->updateBindingCount(reduceMappPart);
							}
						}
					};




					function<void(CMemoryPool* memPools)> clearMemoryPoolsFunc = [&](CMemoryPool* memPools) -> void {
						while (memPools) {
							CMemoryPool* tmpMemPool = memPools;
							memPools = memPools->getNextMemoryPool();
							delete [] tmpMemPool->getMemoryBlockData();
							delete tmpMemPool;
						}
					};

					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> mapReducedConcurrentJoinInsertionComputation = [&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {
						if (!keyMappingBatchDataVector.isEmpty()) {

							QTime* stepTimer = nullptr;
							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker> mappedJoiningHashKeyVec = QtConcurrent::blockingMappedReduced<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedKeysBindingsCardinalityLinker>>(keyMappingBatchDataVector, bindingBatchesHashingKeyMapFunc, bindingBatchesHashingKeyReduceFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < keyMappingBatchDataVector.size(); ++i) {
									if (keyMappingBatchDataVector[i]) {
										++partitionsUsed;
									}
								}
								QString x1 = QString("Insertion step 1 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(keyMappingBatchDataVector.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x1.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x1, this);
								}
								delete stepTimer;
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> mappedJoinedHashKeyVec = QtConcurrent::blockingMappedReduced<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>>(mappedJoiningHashKeyVec, bindingBatchesHashingJoinMapFunc, bindingBatchesHashingJoinReduceFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedJoiningHashKeyVec.size(); ++i) {
									if (mappedJoiningHashKeyVec[i]) {
										++partitionsUsed;
									}
								}
								QString x2 = QString("Insertion step 2 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedJoiningHashKeyVec.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x2.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x2, this);
								}
								delete stepTimer;

								cint64 statMaxJoiningCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalJoiningCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < mappedJoiningHashKeyVec.size(); ++i) {
									COptimizedComplexVariableJoiningHashMemoryManaged* joiningHash = joiningHashVector[i];
									if (joiningHash && joiningHash->mJoinedCount > 0) {
										++statUsedCount;
										statMaxJoiningCount = qMax(statMaxJoiningCount, joiningHash->mJoinedCount);
										statMaxProcessingTime = qMax(statMaxProcessingTime, joiningHash->mProcessingTime);
										statTotalJoiningCount += joiningHash->mJoinedCount;
										statTotalProcessingTime += joiningHash->mProcessingTime;
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
								QString x3 = QString("Insertion step 2 of concurrent join computation for item %1 had in average %2 (max %3) created joins and took in average %4 (max %5) ms.").arg(joiningItem->getComputationStepId()).arg(averageJoiningCount).arg(statMaxJoiningCount).arg(averageProcessingTime).arg(statMaxProcessingTime);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x3.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x3, this);
								}
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*> mappedVarMappingParts = QtConcurrent::blockingMapped<QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>>(mappedJoinedHashKeyVec, bindingBatchesInsertingMapFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedJoinedHashKeyVec.size(); ++i) {
									if (mappedJoinedHashKeyVec[i]) {
										++partitionsUsed;
									}
								}
								QString x4 = QString("Insertion step 3 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedJoinedHashKeyVec.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x4.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x4, this);
								}
								delete stepTimer;

								cint64 statMaxInsertionCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalInsertionCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < mappedJoinedHashKeyVec.size(); ++i) {
									COptimizedComplexVariableIndividualMappingsMultiHashPart* joinedVarMappingMultiHashPart = joinedVarMapping->getMultiHashPart(i);
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
								QString x5 = QString("Insertion step 3 of concurrent join computation for item %1 had in average %2 (max %3) bindings insertions and took in average %4 (max %5) ms.").arg(joiningItem->getComputationStepId()).arg(averageInsertionCount).arg(statMaxInsertionCount).arg(averageProcessingTime).arg(statMaxProcessingTime);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x5.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x5, this);
								}
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							bindingBatchesInsertingReduceFunc(mappedVarMappingParts);
							CMemoryPool* mappedKeyBatchHashVecMemPools = keyMappingBatchDataVector.takeMemoryPools();
							CMemoryPool* mappedJoiningHashVecMemPools = mappedJoiningHashKeyVec.takeMemoryPools();
							CMemoryPool* mappedJoinedHashVecMemPools = mappedJoinedHashKeyVec.takeMemoryPools();
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
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedVarMappingParts.size(); ++i) {
									if (mappedVarMappingParts[i]) {
										++partitionsUsed;
									}
								}
								QString x6 = QString("Insertion step 4 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedVarMappingParts.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x6.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x6, this);
								}
								delete stepTimer;
							}

						}
					};
































					function<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>(const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* const & batchData)> bindingBatchesHashingDirectJoiningMapFunc =
						[&](const CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVectorData<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>* const & batchData) ->CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> {

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> bindingsMappedVec(bindingsMappingSize);
						if (batchData) {
							COptimizedComplexVariableIndividualBindings* tmpJoinedBindings = nullptr;

							CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker* batchLinker = batchData->getNextBatchLinker();
							while (batchLinker) {

								bool leftSide = batchLinker->isLeftSide();

								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								while (bindingsCardinalityCurrentLinker) {

									CXLinker<cint64>* keyBindingLinker = leftKeyBindingLinker;
									CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
									cint64 splitCount = leftSplitCount;
									cint64 splitPos = leftSplitPos;
									if (!leftSide) {
										keyBindingLinker = rightKeyBindingLinker;
										remainingBindingLinker = rightRemainingBindingLinker;
										splitPos = rightSplitPos;
										splitCount = rightSplitCount;
									}

									COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(bindingsCardinalityCurrentLinker->getBindings()->getBindingArray(), keyBindingLinker, remainingBindingLinker);
									cint64 keyHashValue = bindingHasher.getHashValue();
									uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);
									cint64 mappedKeyIndex = uintHashValue % keyMappingSize;

									if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {
										COptimizedComplexVariableJoiningHashMemoryManaged* joiningHash = joiningHashVector[mappedKeyIndex];

										if (joiningHash) {

											const COptimizedComplexVariableJoiningData& joiningData = joiningHash->value(bindingHasher);

											bool leftSide = batchLinker->isLeftSide();

											COptimizedComplexVariableIndividualBindings* varMapping = bindingsCardinalityCurrentLinker->getBindings();
											COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = bindingsCardinalityCurrentLinker->getInitialCardinalities();
											COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = nullptr;

											COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker = joiningHash->createDataLinker();

											COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker tmpLinker(varMapping, newCardinalites, prevCardinalites);
											for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!leftSide); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
												joinedVariableMappingCreationRightLeftCopyingVecInsertionFunc(existingOtherLinkerIt, &tmpLinker, leftSide, bindingsMappedVec, bindingsMappedVec.getMemoryAllocationManager(), tmpJoinedBindings);
											}
										}

									}

									bindingsCardinalityCurrentLinker = batchLinker->getNextBatchBindingsCardinalityLinker(true);
								}



								COptimizedComplexVariableIndividualUpdateCardinalityLinker* cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								while (cardinalityUpdateLinker) {

									CXLinker<cint64>* keyBindingLinker = leftKeyBindingLinker;
									CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
									cint64 splitCount = leftSplitCount;
									cint64 splitPos = leftSplitPos;
									if (!leftSide) {
										keyBindingLinker = rightKeyBindingLinker;
										remainingBindingLinker = rightRemainingBindingLinker;
										splitPos = rightSplitPos;
										splitCount = rightSplitCount;
									}

									COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings()->getBindingArray(), keyBindingLinker, remainingBindingLinker);
									cint64 keyHashValue = bindingHasher.getHashValue();
									uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);
									cint64 mappedKeyIndex = uintHashValue % keyMappingSize;

									if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {
										COptimizedComplexVariableJoiningHashMemoryManaged* joiningHash = joiningHashVector[mappedKeyIndex];

										if (joiningHash) {

											const COptimizedComplexVariableJoiningData& joiningData = joiningHash->value(bindingHasher);

											bool leftSide = batchLinker->isLeftSide();

											COptimizedComplexVariableIndividualBindings* varMapping = cardinalityUpdateLinker->getUpdatedBindingsCardinalityLinker()->getBindings();
											COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = cardinalityUpdateLinker->getNewCardinality();
											COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = cardinalityUpdateLinker->getPreviousCardinality();

											COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker = joiningHash->createDataLinker();

											COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker tmpLinker(varMapping, newCardinalites, prevCardinalites);
											for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!leftSide); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
												joinedVariableMappingCreationRightLeftCopyingVecInsertionFunc(existingOtherLinkerIt, &tmpLinker, leftSide, bindingsMappedVec, bindingsMappedVec.getMemoryAllocationManager(), tmpJoinedBindings);
											}
										}
									}


									cardinalityUpdateLinker = batchLinker->getNextCardinalityUpdateLinker(true);
								}


								batchLinker = batchLinker->getNext();
							}
						}

						return bindingsMappedVec;
					};





					function<void(CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector)> mapReducedConcurrentJoinCheckingComputation = [&](CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker>& keyMappingBatchDataVector) -> void {
						if (!keyMappingBatchDataVector.isEmpty()) {

							QTime* stepTimer = nullptr;
							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker> mappedJoinedHashKeyVec = QtConcurrent::blockingMappedReduced<CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinMappedJoinedBindingsCardinalityLinker>>(keyMappingBatchDataVector, bindingBatchesHashingDirectJoiningMapFunc, bindingBatchesHashingJoinReduceFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < joiningHashVector.size(); ++i) {
									if (joiningHashVector[i]) {
										++partitionsUsed;
									}
								}
								QString x1 = QString("Checking step 1 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(joiningHashVector.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x1.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x1, this);
								}
								delete stepTimer;

								cint64 statMaxJoiningCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalJoiningCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < joiningHashVector.size(); ++i) {
									COptimizedComplexVariableJoiningHashMemoryManaged* joiningHash = joiningHashVector[i];
									if (joiningHash && joiningHash->mJoinedCount > 0) {
										++statUsedCount;
										statMaxJoiningCount = qMax(statMaxJoiningCount, joiningHash->mJoinedCount);
										statMaxProcessingTime = qMax(statMaxProcessingTime, joiningHash->mProcessingTime);
										statTotalJoiningCount += joiningHash->mJoinedCount;
										statTotalProcessingTime += joiningHash->mProcessingTime;
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

								QString x2 = QString("Checking step 1 of concurrent join computation for item %1 had in average %2 (max %3) created joins and took in average %4 (max %5) ms.").arg(joiningItem->getComputationStepId()).arg(averageJoiningCount).arg(statMaxJoiningCount).arg(averageProcessingTime).arg(statMaxProcessingTime);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x2.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x2, this);
								}
							}





							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*> mappedVarMappingParts = QtConcurrent::blockingMapped<QVector<COptimizedComplexVariableIndividualMappingsMultiHashPart*>>(mappedJoinedHashKeyVec, bindingBatchesInsertingMapFunc);
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedJoinedHashKeyVec.size(); ++i) {
									if (mappedJoinedHashKeyVec[i]) {
										++partitionsUsed;
									}
								}
								QString x3 = QString("Checking step 2 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedJoinedHashKeyVec.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x3.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x3, this);
								}
								delete stepTimer;

								cint64 statMaxInsertionCount = 0;
								cint64 statMaxProcessingTime = 0;
								cint64 statTotalInsertionCount = 0;
								cint64 statTotalProcessingTime = 0;
								cint64 statUsedCount = 0;
								for (cint64 i = 0; i < mappedJoinedHashKeyVec.size(); ++i) {
									COptimizedComplexVariableIndividualMappingsMultiHashPart* joinedVarMappingMultiHashPart = joinedVarMapping->getMultiHashPart(i);
									if (joinedVarMappingMultiHashPart && joinedVarMappingMultiHashPart->mStatInsertionCount > 0) {
										++statUsedCount;
										statMaxInsertionCount = qMax(statMaxInsertionCount, joinedVarMappingMultiHashPart->mStatInsertionCount);
										statMaxProcessingTime = qMax(statMaxProcessingTime, joinedVarMappingMultiHashPart->mStatsProcessingTime);
										statTotalInsertionCount += joinedVarMappingMultiHashPart->mStatInsertionCount;
										statTotalProcessingTime += joinedVarMappingMultiHashPart->mStatsProcessingTime;
									}
								}
								cint64 averageTotalInsertionCount = 0;
								if (statUsedCount > 0) {
									averageTotalInsertionCount = statTotalInsertionCount / statUsedCount;
								}
								cint64 averageProcessingTime = 0;
								if (statUsedCount > 0) {
									averageProcessingTime = statTotalProcessingTime / statUsedCount;
								}

								QString x4 = QString("Checking step 2 of concurrent join computation for item %1 had in average %2 (max %3) bindings insertions and took in average %4 (max %5) ms.").arg(joiningItem->getComputationStepId()).arg(averageTotalInsertionCount).arg(statMaxInsertionCount).arg(averageProcessingTime).arg(statMaxProcessingTime);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x4.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x4, this);
								}
							}



							if (mConfPerformanceLogging) {
								stepTimer = new QTime();
								stepTimer->start();
							}
							bindingBatchesInsertingReduceFunc(mappedVarMappingParts);
							CMemoryPool* mappedKeyBatchHashVecMemPools = keyMappingBatchDataVector.takeMemoryPools();
							CMemoryPool* mappedJoinedHashVecMemPools = mappedJoinedHashKeyVec.takeMemoryPools();
							CConcurrentTaskScheduler::run([=] {
								clearMemoryPoolsFunc(mappedKeyBatchHashVecMemPools);
							});
							CConcurrentTaskScheduler::run([=] {
								clearMemoryPoolsFunc(mappedJoinedHashVecMemPools);
							});
							keyMappingBatchDataVector.clear();
							if (stepTimer) {
								cint64 partitionsUsed = 0;
								for (cint64 i = 0; i < mappedVarMappingParts.size(); ++i) {
									if (mappedVarMappingParts[i]) {
										++partitionsUsed;
									}
								}
								QString x5 = QString("Checking step 3 of concurrent join computation for item %3 with %4/%1 tasks took %2 ms.").arg(mappedVarMappingParts.size()).arg(stepTimer->elapsed()).arg(joiningItem->getComputationStepId()).arg(partitionsUsed);
								if (mConfDirectPerformanceConsoleOutput) {
									cout << x5.toLocal8Bit().constData() << endl;
								} else {
									LOG(INFO, getLogDomain(), x5, this);
								}
								delete stepTimer;
							}

						}
					};


					auto checkingHandlingFunc = mapReducedConcurrentJoinInsertionComputation;
					if (mConfCheckingSideDirectJoining) {
						checkingHandlingFunc = mapReducedConcurrentJoinCheckingComputation;
					}





					//if (leftVarMapping->size() > mConfSamplingBasedJoinMappingSize || rightVarMapping->size() > mConfSamplingBasedJoinMappingSize) {
					if (leftItemDep->isBatchProcessed(false) && rightItemDep->isBatchProcessed(false)) {
						leftItemDep->loadNextBatch(false);
						rightItemDep->loadNextBatch(false);
					}

					bool mappingProcessed = false;
					bool requiresScheduling = false;
					while (!joiningItem->isSamplingCompleted() && requiresMoreVariableMappingComputation(joiningItem, true) && (!leftItemDep->isBatchProcessed(false) || !rightItemDep->isBatchProcessed(false) || leftItemDep->loadNextBatch(false) || rightItemDep->loadNextBatch(false)) && !requiresScheduling) {

						CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> keyMappingBatchDataVector(currentBatchSizeCount);
						while (requiresMoreVariableMappingComputation(joiningItem, true) && ((leftItemDep->getBatchCurrentBindingsCardinalityLinker(false) && leftSampleInsertionCount < mConfSamplingBasedJoinMappingSize) || (rightItemDep->getBatchCurrentBindingsCardinalityLinker(false) && rightSampleInsertionCount < mConfSamplingBasedJoinMappingSize))) {

							if (requiresMoreVariableMappingComputation(joiningItem, true) && leftItemDep->getBatchCurrentBindingsCardinalityLinker(false) && leftSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								mappingProcessed = true;

								joiningHashSampleBatchingFunc(true, leftItemDep, leftSampleInsertionCount, keyMappingBatchDataVector);
							}

							if (requiresMoreVariableMappingComputation(joiningItem, true) && rightItemDep->getBatchCurrentBindingsCardinalityLinker(false) && rightSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								mappingProcessed = true;

								joiningHashSampleBatchingFunc(false, rightItemDep, rightSampleInsertionCount, keyMappingBatchDataVector);
							}
						}
						mapReducedConcurrentJoinInsertionComputation(keyMappingBatchDataVector);

						if ((leftSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || leftItemDep->isProcessingFinished(false)) && (rightSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || rightItemDep->isProcessingFinished(false))) {
							joiningItem->setSamplingCompleted(true);
						}

						if (leftItemDep->isBatchProcessed(false)) {
							leftItemDep->loadNextBatch(false);
						}

						if (rightItemDep->isBatchProcessed(false)) {
							rightItemDep->loadNextBatch(false);
						}

						if (!joiningItem->isSamplingCompleted() && (rightSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || rightItemDep->isProcessingFinished(false))) {
							if (leftItemDep->isBatchProcessed(false) && !leftItemDep->isProcessingFinished(false) && leftSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								requiresScheduling = true;
							}
						}
						if (!joiningItem->isSamplingCompleted() && (leftSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || leftItemDep->isProcessingFinished(false))) {
							if (rightItemDep->isBatchProcessed(false) && !rightItemDep->isProcessingFinished(false) && rightSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								requiresScheduling = true;
							}
						}
					}
					if ((leftSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || leftItemDep->isProcessingFinished(false)) && (rightSampleInsertionCount >= mConfSamplingBasedJoinMappingSize || rightItemDep->isProcessingFinished(false))) {
						joiningItem->setSamplingCompleted(true);
					}


					if (joiningItem->isSamplingCompleted()) {
						leftSampleKeyCount = atomicLeftSampleKeyCount;
						rightSampleKeyCount = atomicRightSampleKeyCount;
					}
					bool leftInsertion = isSamplingLeftInsertionDecision(joiningItem);


					if (joiningItem->isSamplingCompleted()) {

						COptimizedComplexVariableCompositionItemDependence* insertingItemDep = leftItemDep;
						COptimizedComplexVariableCompositionItemDependence* checkingItemDep = rightItemDep;


						CXLinker<cint64>* insertingKeyBindingLinker = leftKeyBindingLinker;
						CXLinker<cint64>* checkingKeyBindingLinker = rightKeyBindingLinker;


						if (!leftInsertion) {
							insertingItemDep = rightItemDep;
							checkingItemDep = leftItemDep;

							insertingKeyBindingLinker = rightKeyBindingLinker;
							checkingKeyBindingLinker = leftKeyBindingLinker;
						}


						while (requiresMoreVariableMappingComputation(joiningItem, false) && (!insertingItemDep->isBatchProcessed() || insertingItemDep->loadNextBatch())) {

							while (requiresMoreVariableMappingComputation(joiningItem, false) && insertingItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
								mappingProcessed = true;

								CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> keyMappingBindingsBatchDataVector(currentBatchSizeCount);
								joiningHashBindingsInsertingCheckingBatchingFunc(leftInsertion, insertingItemDep, true, keyMappingBindingsBatchDataVector);
								mapReducedConcurrentJoinInsertionComputation(keyMappingBindingsBatchDataVector);
							}



							// inserting remaining cardinality updates
							while (requiresMoreVariableMappingComputation(joiningItem, false) && insertingItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
								mappingProcessed = true;

								CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> keyMappingCardinalitiesBatchDataVector(currentBatchSizeCount);
								joiningHashCardinalitiesInsertingCheckingBatchingFunc(leftInsertion, insertingItemDep, true, keyMappingCardinalitiesBatchDataVector);
								mapReducedConcurrentJoinInsertionComputation(keyMappingCardinalitiesBatchDataVector);
							}
						}



						if (insertingItemDep->isProcessingFinished()) {
							while (requiresMoreVariableMappingComputation(joiningItem, false) && (!checkingItemDep->isBatchProcessed() || checkingItemDep->loadNextBatch())) {


								// checking remaining mappings
								while (requiresMoreVariableMappingComputation(joiningItem, false) && checkingItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
									mappingProcessed = true;

									CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> keyMappingBindingsBatchDataVector(currentBatchSizeCount);
									joiningHashBindingsInsertingCheckingBatchingFunc(!leftInsertion, checkingItemDep, false, keyMappingBindingsBatchDataVector);
									checkingHandlingFunc(keyMappingBindingsBatchDataVector);
								}


								// checking remaining cardinality updates
								while (requiresMoreVariableMappingComputation(joiningItem, false) && checkingItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
									mappingProcessed = true;

									CQtConcurrentVariableMappingsCompositionBaseBatchLinkerVector<CQtConcurrentVariableMappingsCompositionJoinBindingsCardinalityLinkerBatchLinker> keyMappingCardinalitiesBatchDataVector(currentBatchSizeCount);
									joiningHashCardinalitiesInsertingCheckingBatchingFunc(!leftInsertion, checkingItemDep, false, keyMappingCardinalitiesBatchDataVector);
									checkingHandlingFunc(keyMappingCardinalitiesBatchDataVector);
								}
							}

						}

					}

					joinedVarMapping->incCurrentUpdateId();


					return mappingProcessed;
				}



			}; // end namespace Composition

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
