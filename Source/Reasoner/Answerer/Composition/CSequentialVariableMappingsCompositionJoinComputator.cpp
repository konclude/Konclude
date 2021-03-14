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

#include "CSequentialVariableMappingsCompositionJoinComputator.h"

namespace Konclude {

	namespace Reasoner {

		namespace Answerer {

			namespace Composition {

				CSequentialVariableMappingsCompositionJoinComputator::CSequentialVariableMappingsCompositionJoinComputator() {
					mComputerName = "Sequential join";
				}




				bool CSequentialVariableMappingsCompositionJoinComputator::computeVariableMappingsJoinComposition(COptimizedComplexVariableJoiningItem* joiningItem, COptimizedComplexBuildingVariableCompositionsItem* buildingVarItem, CAnswererContext* answererContext, bool& processing) {

					COptimizedComplexVariableCompositionItem* leftItem = joiningItem->getLeftItem();
					COptimizedComplexVariableCompositionItem* rightItem = joiningItem->getRightItem();

					COptimizedComplexVariableIndividualMappings* leftVarMapping = leftItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappings* rightVarMapping = rightItem->getVariableMapping();
					COptimizedComplexVariableIndividualMappings* joinedVarMapping = joiningItem->getVariableMapping();

					cint64 leftSize = leftVarMapping->getBindingSize();
					cint64 rightSize = rightVarMapping->getBindingSize();
					cint64 joinSize = joinedVarMapping->getBindingSize();
					

					COptimizedComplexVariableJoiningBindingPositionMapping* variablePositionMapping = joiningItem->getPositionMapping();

					bool itemSameVarReductionJoining = variablePositionMapping->isSelfSameVariableReduction();
					cint64* rightPosMapArray = variablePositionMapping->getRightPosMapArray();
					cint64* leftPosMapArray = variablePositionMapping->getLeftPosMapArray();

					CXLinker<cint64>* leftKeyBindingLinker = variablePositionMapping->getLeftKeyBindingLinker();
					CXLinker<cint64>* rightKeyBindingLinker = variablePositionMapping->getRightKeyBindingLinker();

					COptimizedComplexVariableJoiningHash*& joiningHash = joiningItem->getJoiningHash();

					//QHash<QString, cint64>& debugVarBindStringCardHash = joiningItem->debugVarBindStringCardHash;

					if (!joiningHash) {
						joiningHash = new COptimizedComplexVariableJoiningHash(leftKeyBindingLinker, rightKeyBindingLinker);
					}


					COptimizedComplexVariableCompositionItemDependence* leftItemDep = joiningItem->getLeftItemDependence();
					COptimizedComplexVariableCompositionItemDependence* rightItemDep = joiningItem->getRightItemDependence();


					cint64& leftSampleKeyCount = joiningItem->getLeftSampleKeyCount();
					cint64& rightSampleKeyCount = joiningItem->getRightSampleKeyCount();

					cint64& leftSampleInsertionCount = joiningItem->getLeftSampleInsertionCount();
					cint64& rightSampleInsertionCount = joiningItem->getRightSampleInsertionCount();


					COptimizedComplexVariableIndividualBindings* joinedBindings = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinality* joinedCardinalites = nullptr;
					COptimizedComplexVariableIndividualBindingsCardinalityLinker* joinedLinker = nullptr;

					bool reduction = variablePositionMapping->isVariableReduction();
					CSameRealization* sameRealization = mOntoAnsweringItem->getOntology()->getRealization()->getSameRealization();





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
					CMemoryAllocationManager* bindingsMemMan = joinedVarMapping->getBindingsMemoryAllocationManager();



					auto joiningVariableMappingCreationFunc = [&](COptimizedComplexVariableIndividualBindings* leftvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* leftNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* leftPrevCardinalites, COptimizedComplexVariableIndividualBindings* rightvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* rightNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* righPrevCardinalites) -> void {


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

						if (!joinedBindings) {
							joinedBindings = createBindingsForVariableCompositionItems(joinSize, joiningItem, buildingVarItem, bindingsMemMan);
						}
						if (!joinedCardinalites && (leftCardinalites || rightCardinalites || reduction)) {
							joinedCardinalites = createBindingsCardinalitesForVariableCompositionItems(joiningItem, buildingVarItem, bindingsMemMan);
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
						//bool created = debugVarBindStringCardHash.contains(bindingsString);
						//cint64& card = debugVarBindStringCardHash[bindingsString];
						//cint64 prevCard = card;
						//if (joinedCardinalites) {
						//	card += joinedCardinalites->getSameIndividualsJointlyConsideredCardinality();
						//} else {
						//	card++;
						//}


						if (!joinedLinker) {
							joinedLinker = createBindingsLinkerForVariableCompositionItems(joinedBindings, joinedCardinalites, joiningItem, buildingVarItem, bindingsMemMan);
						} else {
							joinedLinker->setInitialCardinality(joinedCardinalites);
						}
						if (joinedVarMapping->addInsertingBindingsCardinalityLinker(joinedLinker, true)) {
							joinedBindings = nullptr;
							joinedCardinalites = nullptr;
							joinedLinker = nullptr;
						} else {
							joinedCardinalites = joinedLinker->getCurrentCardinalities();
						}
					};

					auto joiningVariableMappingCreationLeftRightFunc = [&](COptimizedComplexVariableIndividualBindings* leftvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* leftNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* leftPrevCardinalites, COptimizedComplexVariableIndividualBindings* rightvarMapping, COptimizedComplexVariableIndividualBindingsCardinality* rightNewCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* righPrevCardinalites, bool swap) -> void {
						if (swap) {
							joiningVariableMappingCreationFunc(rightvarMapping, rightNewCardinalites, righPrevCardinalites, leftvarMapping, leftNewCardinalites, leftPrevCardinalites);
						} else {
							joiningVariableMappingCreationFunc(leftvarMapping, leftNewCardinalites, leftPrevCardinalites, rightvarMapping, rightNewCardinalites, righPrevCardinalites);
						}
					};


					auto joiningVariableMappingCreationLeftRightLinkerFunc = [&](COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* leftLinker, COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* rightLinker, bool swap) -> void {
						joiningVariableMappingCreationLeftRightFunc(leftLinker->getBindings(), leftLinker->getNewCardinalities(), leftLinker->getPreviousCardinalities(), rightLinker->getBindings(), rightLinker->getNewCardinalities(), rightLinker->getPreviousCardinalities(), swap);
					};


					function<void(bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites, cint64& sampleLimit)> joiningHashSampleInserterFunc = [&](bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites, cint64& sampleCount) -> void {

						CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
						cint64 splitCount = leftSplitCount;
						cint64 splitPos = leftSplitPos;
						if (!left) {
							remainingBindingLinker = rightRemainingBindingLinker;
							splitPos = rightSplitPos;
							splitCount = rightSplitCount;
						}

						COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(varMapping->getBindingArray(), keyBindingLinker, remainingBindingLinker);

						cint64 keyHashValue = bindingHasher.getHashValue();
						uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);

						if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {

							COptimizedComplexVariableJoiningData& joiningData = (*joiningHash)[bindingHasher];

							//if (left) {
							//	joiningData.mLeftAdded++;
							//} else {
							//	joiningData.mRightAdded++;
							//}

							if (!joiningData.hasBindings(left)) {
								++sampleCount;
							}
							COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker = new COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker(varMapping, newCardinalites, prevCardinalites);
							for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!left); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
								joiningVariableMappingCreationLeftRightLinkerFunc(existingOtherLinkerIt, linker, left);
							}
							joiningData.addBindingLinker(left, linker);
						}
					};



					function<void(bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites)> joiningHashInserterFunc = [&](bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites) -> void {
						CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
						cint64 splitCount = leftSplitCount;
						cint64 splitPos = leftSplitPos;
						if (!left) {
							remainingBindingLinker = rightRemainingBindingLinker;
							splitPos = rightSplitPos;
							splitCount = rightSplitCount;
						}

						COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(varMapping->getBindingArray(), keyBindingLinker, remainingBindingLinker);
						cint64 keyHashValue = bindingHasher.getHashValue();
						uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);

						if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {
							COptimizedComplexVariableJoiningData& joiningData = (*joiningHash)[bindingHasher];

							//if (left) {
							//	joiningData.mLeftAdded++;
							//} else {
							//	joiningData.mRightAdded++;
							//}


							COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* linker = new COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker(varMapping, newCardinalites, prevCardinalites);
							for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!left); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
								joiningVariableMappingCreationLeftRightLinkerFunc(existingOtherLinkerIt, linker, left);
							}
							joiningData.addBindingLinker(left, linker);
						}
					};


					// assuming that all mappings from the inserter side have been inserted
					function<void(bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites)> joiningHashCheckerFunc = [&](bool left, CXLinker<cint64>* keyBindingLinker, COptimizedComplexVariableIndividualBindings* varMapping, COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites, COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites) -> void {
						CXLinker<cint64>* remainingBindingLinker = leftRemainingBindingLinker;
						cint64 splitCount = leftSplitCount;
						cint64 splitPos = leftSplitPos;
						if (!left) {
							remainingBindingLinker = rightRemainingBindingLinker;
							splitPos = rightSplitPos;
							splitCount = rightSplitCount;
						}

						COptimizedComplexVariableJoiningWithRemainingHasher bindingHasher(varMapping->getBindingArray(), keyBindingLinker, remainingBindingLinker);
						cint64 keyHashValue = bindingHasher.getHashValue();
						uint uintHashValue = uint((keyHashValue >> (8 * sizeof(uint) - 1)) ^ keyHashValue);

						if (!splitComputationMode || bindingHasher.getRemainingHashValue() % splitCount == splitPos) {
							const COptimizedComplexVariableJoiningData& joiningData = joiningHash->value(bindingHasher);

							//if (left) {
							//	joiningData.mLeftAdded++;
							//} else {
							//	joiningData.mRightAdded++;
							//}

							COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker tmpLinker(varMapping, newCardinalites, prevCardinalites);
							for (COptimizedComplexVariableJoiningBindingsCardinalitiesDataLinker* existingOtherLinkerIt = joiningData.getBindingLinker(!left); existingOtherLinkerIt; existingOtherLinkerIt = existingOtherLinkerIt->getNext()) {
								joiningVariableMappingCreationLeftRightLinkerFunc(existingOtherLinkerIt, &tmpLinker, left);
							}
						}
					};



					//if (leftVarMapping->size() > mConfSamplingBasedJoinMappingSize || rightVarMapping->size() > mConfSamplingBasedJoinMappingSize) {
					if (leftItemDep->isBatchProcessed(false) && rightItemDep->isBatchProcessed(false)) {
						leftItemDep->loadNextBatch(false);
						rightItemDep->loadNextBatch(false);
					}

					bool mappingProcessed = false;
					bool requiresScheduling = false;
					while (!joiningItem->isSamplingCompleted() && requiresMoreVariableMappingComputation(joiningItem, true) && (!leftItemDep->isBatchProcessed(false) || !rightItemDep->isBatchProcessed(false) || leftItemDep->loadNextBatch(false) || rightItemDep->loadNextBatch(false)) && !requiresScheduling) {

						while (requiresMoreVariableMappingComputation(joiningItem, true) && ((leftItemDep->getBatchCurrentBindingsCardinalityLinker(false) && leftSampleInsertionCount < mConfSamplingBasedJoinMappingSize) || (rightItemDep->getBatchCurrentBindingsCardinalityLinker(false) && rightSampleInsertionCount < mConfSamplingBasedJoinMappingSize))) {

							if (requiresMoreVariableMappingComputation(joiningItem, true) && leftItemDep->getBatchCurrentBindingsCardinalityLinker(false) && leftSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								mappingProcessed = true;
								++leftSampleInsertionCount;
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = leftItemDep->getBatchCurrentBindingsCardinalityLinker(true);
								COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
								COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

								joiningHashSampleInserterFunc(true, leftKeyBindingLinker, bindings, cardinalites, nullptr, leftSampleKeyCount);
							}

							if (requiresMoreVariableMappingComputation(joiningItem, true) && rightItemDep->getBatchCurrentBindingsCardinalityLinker(false) && rightSampleInsertionCount < mConfSamplingBasedJoinMappingSize) {
								mappingProcessed = true;
								++rightSampleInsertionCount;
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = rightItemDep->getBatchCurrentBindingsCardinalityLinker(true);
								COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
								COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = bindingLinker->getInitialCardinalities();

								joiningHashSampleInserterFunc(false, rightKeyBindingLinker, bindings, cardinalites, nullptr, rightSampleKeyCount);
							}

						}

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

							// inserting remaining mappings
							while (requiresMoreVariableMappingComputation(joiningItem, false) && insertingItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
								mappingProcessed = true;
								COptimizedComplexVariableIndividualBindingsCardinalityLinker* inertingLinker = insertingItemDep->getBatchCurrentBindingsCardinalityLinker(true);

								COptimizedComplexVariableIndividualBindings* bindings = inertingLinker->getBindings();
								COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = inertingLinker->getInitialCardinalities();

								joiningHashInserterFunc(leftInsertion, insertingKeyBindingLinker, bindings, cardinalites, nullptr);
							}




							// inserting remaining cardinality updates
							while (requiresMoreVariableMappingComputation(joiningItem, false) && insertingItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
								mappingProcessed = true;
								COptimizedComplexVariableIndividualUpdateCardinalityLinker* insertingUpdatedCardinalityLinker = insertingItemDep->getBatchCurrentUpdatedCardinalityLinker(true);

								COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = insertingUpdatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
								COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
								COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = insertingUpdatedCardinalityLinker->getPreviousCardinality();
								COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = insertingUpdatedCardinalityLinker->getNewCardinality();

								joiningHashInserterFunc(leftInsertion, insertingKeyBindingLinker, bindings, newCardinalites, prevCardinalites);
							}
						}

						if (insertingItemDep->isProcessingFinished()) {
							while (requiresMoreVariableMappingComputation(joiningItem, false) && (!checkingItemDep->isBatchProcessed() || checkingItemDep->loadNextBatch())) {


								// checking remaining mappings
								while (requiresMoreVariableMappingComputation(joiningItem, false) && checkingItemDep->getBatchCurrentBindingsCardinalityLinker(false)) {
									mappingProcessed = true;
									COptimizedComplexVariableIndividualBindingsCardinalityLinker* checkingLinker = checkingItemDep->getBatchCurrentBindingsCardinalityLinker(true);

									COptimizedComplexVariableIndividualBindings* bindings = checkingLinker->getBindings();
									COptimizedComplexVariableIndividualBindingsCardinality* cardinalites = checkingLinker->getInitialCardinalities();

									joiningHashCheckerFunc(!leftInsertion, checkingKeyBindingLinker, bindings, cardinalites, nullptr);
								}


								// checking remaining cardinality updates
								while (requiresMoreVariableMappingComputation(joiningItem, false) && checkingItemDep->getBatchCurrentUpdatedCardinalityLinker(false)) {
									mappingProcessed = true;
									COptimizedComplexVariableIndividualUpdateCardinalityLinker* checkingUpdatedCardinalityLinker = checkingItemDep->getBatchCurrentUpdatedCardinalityLinker(true);

									COptimizedComplexVariableIndividualBindingsCardinalityLinker* bindingLinker = checkingUpdatedCardinalityLinker->getUpdatedBindingsCardinalityLinker();
									COptimizedComplexVariableIndividualBindings* bindings = bindingLinker->getBindings();
									COptimizedComplexVariableIndividualBindingsCardinality* prevCardinalites = checkingUpdatedCardinalityLinker->getPreviousCardinality();
									COptimizedComplexVariableIndividualBindingsCardinality* newCardinalites = checkingUpdatedCardinalityLinker->getNewCardinality();

									joiningHashCheckerFunc(!leftInsertion, checkingKeyBindingLinker, bindings, newCardinalites, prevCardinalites);
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
