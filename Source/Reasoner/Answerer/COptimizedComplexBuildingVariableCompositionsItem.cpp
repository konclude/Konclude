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

#include "COptimizedComplexBuildingVariableCompositionsItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexBuildingVariableCompositionsItem::COptimizedComplexBuildingVariableCompositionsItem(CComplexQueryProcessingData* queryProcessingData) {
				mQueryProcessingData = queryProcessingData;
				mWaitingComputationStepsCount = 0;
				mInitializedBaseConceptVariableItems = false;
				mInitializedBasicDataVariableItems = false;
				mJoinedBasicDataVariableItems = false;
				mProcessingQueued = false;
				mWaitingRoleTargetsRealizationCount = 0;
				mWaitingVariableBindingsPropagationCount = 0;
				mWaitingVariableBindingsConfirmationCount = 0;
				mLastHandVarExp = nullptr;
				mInitializedConceptDataVariableExtensionItems = false;
				mSatisfiable = true;
				mVarCompItemWaitingCount = 0;
				mSubVarBuildItemWaitingCount = 0;
				mBindingsReducible = false;
				mAbsorptionBasedQueryPartsOrdinaryEvaluated = false;
				mTemporaryMaterializationData = nullptr;
				mBuildingFinishingData = nullptr;
				mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem = nullptr;
			}


			COptimizedComplexBuildingVariableCompositionsItem::~COptimizedComplexBuildingVariableCompositionsItem() {
				if (mBuildingFinishingData) {
					delete mBuildingFinishingData;
				}
				if (mTemporaryMaterializationData) {
					delete mTemporaryMaterializationData;
				}
				if (mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem) {
					delete mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem;
				}
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::createExtendingBuildingVariableCompositionsItem() {
				COptimizedComplexBuildingVariableCompositionsItem* extBuildingVarItem = new COptimizedComplexBuildingVariableCompositionsItem(mQueryProcessingData);
				extBuildingVarItem->mVarItemIndexMappingHash = mVarItemIndexMappingHash;
				extBuildingVarItem->mVarVarCompItemHash = mVarVarCompItemHash;
				extBuildingVarItem->mUsedComplexVariableCompositionItemList = mUsedComplexVariableCompositionItemList;
				extBuildingVarItem->mVarLastItemHash = mVarLastItemHash;
				extBuildingVarItem->mVarLastItemAssociatedVariableHash = mVarLastItemAssociatedVariableHash;
				extBuildingVarItem->mLastHandVarExp = mLastHandVarExp;
				extBuildingVarItem->mAllVarExpSet = mAllVarExpSet;
				return extBuildingVarItem;
			}





			CComplexQueryMaterializationData* COptimizedComplexBuildingVariableCompositionsItem::getMaterializationData() {
				return mTemporaryMaterializationData;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setMaterializationData(CComplexQueryMaterializationData* tmpOnto) {
				mTemporaryMaterializationData = tmpOnto;
				return this;
			}



			CComplexQueryFinishingBuildingVariableCompositionsItemData* COptimizedComplexBuildingVariableCompositionsItem::getBuildingFinishingData() {
				return mBuildingFinishingData;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setBuildingFinishingData(CComplexQueryFinishingBuildingVariableCompositionsItemData* data) {
				mBuildingFinishingData = data;
				return this;
			}





			QHash<CExpressionVariable*, CBuildExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableClassTermExpressionHash() {
				return &mVariableClassTermExpressionHash;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setVariableClassTermExpressionHash(const QHash<CExpressionVariable*, CBuildExpression*>& hash) {
				mVariableClassTermExpressionHash = hash;
				return this;
			}





			bool COptimizedComplexBuildingVariableCompositionsItem::isSatisfiable() {
				return mSatisfiable;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setSatisfiability(bool satisfiable) {
				mSatisfiable = satisfiable;
				return this;
			}


			QSet<CExpressionVariable*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableSingleCardinalitySet() {
				return &mVariableSingleCardinalitySet;
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::hasVariableSingleCardinality(CExpressionVariable* varExp) {
				return mVariableSingleCardinalitySet.contains(varExp);
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariableComplexConceptItem(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem) {
				mVarConItemHash.insert(varExp, conItem);
				mRemainingVarExpSet.insert(varExp);
				mReuseVarExpComputationsCheckSet.insert(varExp);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariablePropertyAssertion(CIndividualVariableExpression* varExp, CObjectPropertyAssertionExpression* propAssExp) {
				mRemainingVarExpSet.insert(varExp);
				mReuseVarExpComputationsCheckSet.insert(varExp);
				mVarPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariablePropertyAssertions(const QList<CObjectPropertyAssertionExpression*>& propAssExpList) {
				for (CObjectPropertyAssertionExpression* propAssExp : propAssExpList) {
					CIndividualVariableExpression* indiVar1Exp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getFirstIndividualTermExpression());
					CIndividualVariableExpression* indiVar2Exp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getSecondIndividualTermExpression());
					if (indiVar1Exp && indiVar2Exp) {
						addVariablePropertyAssertion(indiVar1Exp, propAssExp);
						addVariablePropertyAssertion(indiVar2Exp, propAssExp);
					}
				}
				return this;
			}


			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getRemainingVariableExpressionSet() {
				return &mRemainingVarExpSet;
			}



			QList<CObjectPropertyAssertionExpression*> COptimizedComplexBuildingVariableCompositionsItem::getUnhanledPropertyAssertionsExpressions(CIndividualVariableExpression* varExp) {
				QList<CObjectPropertyAssertionExpression*> propAssList;
				QList<CObjectPropertyAssertionExpression*> tmpPropAssList(mVarPropAssHash.values(varExp));
				for (CObjectPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;

			}



			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingComputationStep() {
				return mWaitingComputationStepsCount > 0;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setComputationStepFinished(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount--;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setComputationStepWaiting(CComplexConceptStepComputationProcess* computationStep) {
				mWaitingComputationStepsCount++;
				return this;
			}

			CComplexQueryProcessingData* COptimizedComplexBuildingVariableCompositionsItem::getQueryProcessingData() {
				return mQueryProcessingData;
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::isInitializedBaseConceptVariableItems() {
				return mInitializedBaseConceptVariableItems;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setInitializedBaseConceptVariableItems(bool initialized) {
				mInitializedBaseConceptVariableItems = initialized;
				return this;
			}


			QHash<CIndividualVariableExpression*, COptimizedComplexConceptItem*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableConceptItemHash() {
				return &mVarConItemHash;
			}

			QHash<CExpressionVariable*, QSet<COptimizedComplexVariableCompositionItem*>*>* COptimizedComplexBuildingVariableCompositionsItem::getVariableExpressionVariableCompositionItemHash() {
				return &mVarVarCompItemHash;
			}


			CIndividualVariableExpression* COptimizedComplexBuildingVariableCompositionsItem::getOtherVariableExpression(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss) {
				CIndividualVariableExpression* otherVarExp = (CIndividualVariableExpression*)propAss->getFirstIndividualTermExpression();
				if (otherVarExp != varEpx) {
					return otherVarExp;
				} else {
					return (CIndividualVariableExpression*)propAss->getSecondIndividualTermExpression();
				}
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::isRemovingPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss) {
				CIndividualVariableExpression* otherVarEpx = getOtherVariableExpression(varEpx, propAss);
				if (otherVarEpx == varEpx) {
					return false;
				}
				QSet<CIndividualVariableExpression*> reachedVarExpSet;
				QList<CIndividualVariableExpression*> testingVarExpList;
				testingVarExpList.append(otherVarEpx);
				reachedVarExpSet.insert(otherVarEpx);
				while (!testingVarExpList.isEmpty()) {
					CIndividualVariableExpression* testingVarEpx = testingVarExpList.takeFirst();
					QList<CObjectPropertyAssertionExpression*> conPropAssList = getUnhanledPropertyAssertionsExpressions(testingVarEpx);
					for (CObjectPropertyAssertionExpression* connPropAss : conPropAssList) {
						if (connPropAss != propAss) {
							CIndividualVariableExpression* connVarEpx = getOtherVariableExpression(testingVarEpx, connPropAss);
							if (connVarEpx == varEpx) {
								return false;
							}
							if (!reachedVarExpSet.contains(connVarEpx)) {
								reachedVarExpSet.insert(connVarEpx);
								testingVarExpList.append(connVarEpx);
							}
						}
					}
					QList<CDataPropertyAssertionExpression*> conDataPropAssList = getUnhanledDataPropertyAssertionsExpressions(testingVarEpx);
					for (CDataPropertyAssertionExpression* connDataPropAss : conDataPropAssList) {
						CDataLiteralVariableExpression* connVarEpx = (CDataLiteralVariableExpression*)connDataPropAss->getDataPropertyTermExpression();
						QList<CDataPropertyAssertionExpression*> conIndiBackDataPropAssList = getUnhanledDataPropertyAssertionsExpressions(connVarEpx);
						for (CDataPropertyAssertionExpression* connIndiBackDataPropAss : conIndiBackDataPropAssList) {
							CIndividualVariableExpression* backIndiVarExp = (CIndividualVariableExpression*)connIndiBackDataPropAss->getIndividualTermExpression();
							if (!reachedVarExpSet.contains(backIndiVarExp)) {
								reachedVarExpSet.insert(backIndiVarExp);
								testingVarExpList.append(backIndiVarExp);
							}
						}
					}

				}
				return true;
			}






			bool COptimizedComplexBuildingVariableCompositionsItem::isRemovingDataPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CDataPropertyAssertionExpression* dataPropAss) {
				CDataLiteralVariableExpression* dataVarEpx = (CDataLiteralVariableExpression*)dataPropAss->getDataLiteralTermExpression();
				QList<CIndividualVariableExpression*> connIndVarExpList = getUnhandledDataExtensionIndividualTermExpressions(dataPropAss, varEpx);
				QSet<CIndividualVariableExpression*> connIndVarExpSet = connIndVarExpList.toSet();

				for (CIndividualVariableExpression* connIndVarExp : connIndVarExpList) {
					QSet<CIndividualVariableExpression*> foundIndVarExpSet;

					QSet<CIndividualVariableExpression*> reachedVarExpSet;
					QList<CIndividualVariableExpression*> testingVarExpList;
					testingVarExpList.append(connIndVarExp);
					reachedVarExpSet.insert(connIndVarExp);
					while (!testingVarExpList.isEmpty()) {
						CIndividualVariableExpression* testingVarEpx = testingVarExpList.takeFirst();
						QList<CObjectPropertyAssertionExpression*> conPropAssList = getUnhanledPropertyAssertionsExpressions(testingVarEpx);
						for (CObjectPropertyAssertionExpression* connPropAss : conPropAssList) {
							CIndividualVariableExpression* connVarEpx = getOtherVariableExpression(testingVarEpx, connPropAss);
							if (connVarEpx != varEpx && !reachedVarExpSet.contains(connVarEpx)) {

								if (connIndVarExpSet.contains(connVarEpx)) {
									foundIndVarExpSet.insert(connVarEpx);
									if (foundIndVarExpSet.size() == connIndVarExpSet.size()) {
										break;
									}		 
								}

								reachedVarExpSet.insert(connVarEpx);
								testingVarExpList.append(connVarEpx);
							}
						}
						QList<CDataPropertyAssertionExpression*> conDataPropAssList = getUnhanledDataPropertyAssertionsExpressions(testingVarEpx);
						for (CDataPropertyAssertionExpression* connDataPropAss : conDataPropAssList) {
							CDataLiteralVariableExpression* connVarEpx = (CDataLiteralVariableExpression*)connDataPropAss->getDataPropertyTermExpression();
							QList<CDataPropertyAssertionExpression*> conIndiBackDataPropAssList = getUnhanledDataPropertyAssertionsExpressions(connVarEpx);
							for (CDataPropertyAssertionExpression* connIndiBackDataPropAss : conIndiBackDataPropAssList) {
								CIndividualVariableExpression* backIndiVarExp = (CIndividualVariableExpression*)connIndiBackDataPropAss->getIndividualTermExpression();
								if (backIndiVarExp != varEpx && !reachedVarExpSet.contains(backIndiVarExp)) {

									if (connIndVarExpSet.contains(backIndiVarExp)) {
										foundIndVarExpSet.insert(backIndiVarExp);
										if (foundIndVarExpSet.size() == connIndVarExpSet.size()) {
											break;
										}
									}
									reachedVarExpSet.insert(backIndiVarExp);
									testingVarExpList.append(backIndiVarExp);
								}
							}
						}

					}

					if (foundIndVarExpSet.size() != connIndVarExpSet.size()) {
						return false;
					}

				}
				return true;
			}




			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setPropertyAssertionHandled(CObjectPropertyAssertionExpression* propAss) {
				mHandledPropAssSet.insert(propAss);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setVariableExpressionHandled(CIndividualVariableExpression* varEpx) {
				mHandledVarExpSet.insert(varEpx);
				mRemainingVarExpSet.remove(varEpx);
				return this;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setVariableExpressionUnhandled(CIndividualVariableExpression* varEpx) {
				mHandledVarExpSet.remove(varEpx);
				mRemainingVarExpSet.insert(varEpx);
				return this;
			}




			bool COptimizedComplexBuildingVariableCompositionsItem::hasWaitingVariableCompositionItems() {
				return mVarCompItemWaitingCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getVariableCompositionItemWaitingCount() {
				return mVarCompItemWaitingCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incVariableCompositionItemWaitingCount(cint64 incCount) {
				mVarCompItemWaitingCount += incCount;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decVariableCompositionItemWaitingCount(cint64 decCount) {
				mVarCompItemWaitingCount -= decCount;
				return this;
			}









			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingSubVariableBuildingItems() {
				return mSubVarBuildItemWaitingCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getWaitingSubVariableBuildingItemCount() {
				return mSubVarBuildItemWaitingCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incWaitingSubVariableBuildingItemCount(cint64 incCount) {
				mSubVarBuildItemWaitingCount += incCount;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decWaitingSubVariableBuildingItemCount(cint64 decCount) {
				mSubVarBuildItemWaitingCount -= decCount;
				return this;
			}




			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexBuildingVariableCompositionsItem::getComputeVariableMappingItemList() {
				return &mComputeMappingItemList;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addComputeVariableMappingItem(COptimizedComplexVariableCompositionItem* item) {
				mComputeMappingItemList.append(item);
				return this;
			}


			QHash<CExpressionVariable*, COptimizedComplexVariableCompositionItemVariableExpressionMapping>* COptimizedComplexBuildingVariableCompositionsItem::getVariableItemIndexMappingHash() {
				return &mVarItemIndexMappingHash;
			}


			COptimizedComplexVariableCompositionItemVariableExpressionMapping& COptimizedComplexBuildingVariableCompositionsItem::getVariableItemIndexMapping(CExpressionVariable* varExp) {
				return mVarItemIndexMappingHash[varExp];
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::isProcessingQueued() {
				return mProcessingQueued;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setProcessingQueued(bool queued) {
				mProcessingQueued = queued;
				return this;
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingRoleTargetsRealization() {
				return mWaitingRoleTargetsRealizationCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getWaitingRoleTargetsRealizationCount() {
				return mWaitingRoleTargetsRealizationCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incWaitingRoleTargetsRealizationCount(cint64 count) {
				mWaitingRoleTargetsRealizationCount += count;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decWaitingRoleTargetsRealizationCount(cint64 count) {
				mWaitingRoleTargetsRealizationCount -= count;
				return this;
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingComputation() {
				return hasWaitingVariableCompositionItems() || isWaitingSubVariableBuildingItems() || isWaitingRoleTargetsRealization() || isWaitingComputationStep() || isWaitingVariableBindingsPropagation() || isWaitingVariableBindingsConfirmation();
			}


			CExpressionVariable* COptimizedComplexBuildingVariableCompositionsItem::getLastHandledVariableExpression() {
				return mLastHandVarExp;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setLastHandledVariableExpression(CExpressionVariable* varExp) {
				mLastHandVarExp = varExp;
				return this;
			}

			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getReuseVariableExpressionComputationsCheckSet() {
				return &mReuseVarExpComputationsCheckSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexBuildingVariableCompositionsItem::getAllVariableSet() {
				return &mAllVarExpSet;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setAllVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mAllVarExpSet = varSet;
				return this;
			}

			QSet<CExpressionVariable*>* COptimizedComplexBuildingVariableCompositionsItem::getDistinguishedVariableSet() {
				return &mDistinguishedVarExpSet;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setDistinguishedVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mDistinguishedVarExpSet = varSet;
				return this;
			}


			QSet<CExpressionVariable*>* COptimizedComplexBuildingVariableCompositionsItem::getReductionDeniedVariableSet() {
				return &mReductionDeniedVariableSet;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setReductionDeniedVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mReductionDeniedVariableSet = varSet;
				return this;
			}


			QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getDataLiteralVariableIndidualVariableMappingHash() {
				return &mDataVarIndiVarHash;
			}

			QHash<CIndividualVariableExpression*, QHash<CDataLiteralVariableExpression*, CDataPropertyTermExpression*>>* COptimizedComplexBuildingVariableCompositionsItem::getIndidualVariableDataLiteralVariableRoleMappingHash() {
				return &mIndiVarDataVarHash;
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::isInitializedConceptDataVariableExtensionItems() {
				return mInitializedConceptDataVariableExtensionItems;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setInitializedConceptDataVariableExtensionItems(bool initialized) {
				mInitializedConceptDataVariableExtensionItems = initialized;
				return this;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addIndidualVariableDataLiteralVariableRoleRequiredExtension(CIndividualVariableExpression* indiVar, CDataLiteralVariableExpression* dataLitVar, CDataPropertyTermExpression* dataRole) {
				mIndiVarDataVarHash[indiVar].insertMulti(dataLitVar, dataRole);
				mDataVarIndiVarHash.insert(dataLitVar, indiVar);
				mRemainingDataVarExpSet.insert(indiVar);
				return this;
			}


			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getRemainingExtendingDataVariableSet() {
				return &mRemainingDataVarExpSet;
			}




			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariableDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList) {
				for (CDataPropertyAssertionExpression* propAssExp : dataPropAssExpList) {
					CIndividualVariableExpression* indiVar1Exp = dynamic_cast<CIndividualVariableExpression*>(propAssExp->getIndividualTermExpression());
					CDataLiteralVariableExpression* dataVar2Exp = dynamic_cast<CDataLiteralVariableExpression*>(propAssExp->getDataLiteralTermExpression());
					CDataPropertyTermExpression* dataPropTermExp = propAssExp->getDataPropertyTermExpression();
					if (indiVar1Exp && dataVar2Exp) {
						addVariableDataPropertyAssertion(indiVar1Exp, propAssExp);
						addVariableDataPropertyAssertion(dataVar2Exp, propAssExp);
						addIndidualVariableDataLiteralVariableRoleRequiredExtension(indiVar1Exp, dataVar2Exp, dataPropTermExp);
					}
				}
				return this;
			}



			bool COptimizedComplexBuildingVariableCompositionsItem::isJoinedBasicDataVariableItems() {
				return mJoinedBasicDataVariableItems;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setJoinedBasicDataVariableItems(bool initialized) {
				mJoinedBasicDataVariableItems = initialized;
				return this;
			}

			bool COptimizedComplexBuildingVariableCompositionsItem::isInitializedBasicDataVariableItems() {
				return mInitializedBasicDataVariableItems;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setInitializedBasicDataVariableItems(bool initialized) {
				mInitializedBasicDataVariableItems = initialized;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addBasicDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList) {
				mBasicDataPropAssList.append(dataPropAssExpList);
				for (CDataPropertyAssertionExpression* propAssExp : dataPropAssExpList) {
					CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(propAssExp->getDataLiteralTermExpression());
					if (dataVarExp) {
						mBasicDataVarJoiningSet.insert(dataVarExp);
					}
				}
				return this;
			}


			QList<CDataPropertyAssertionExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getBasicDataPropertyAssertionList() {
				return &mBasicDataPropAssList;
			}

			QSet<CDataLiteralVariableExpression*>* COptimizedComplexBuildingVariableCompositionsItem::getBasicDataVariableJoiningSet() {
				return &mBasicDataVarJoiningSet;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariableDataPropertyAssertion(CIndividualVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp) {
				mVarDataPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addVariableDataPropertyAssertion(CDataLiteralVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp) {
				mDataVarDataPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}

			QList<CDataPropertyAssertionExpression*> COptimizedComplexBuildingVariableCompositionsItem::getUnhanledDataPropertyAssertionsExpressions(CIndividualVariableExpression* varExp) {
				QList<CDataPropertyAssertionExpression*> propAssList;
				QList<CDataPropertyAssertionExpression*> tmpPropAssList(mVarDataPropAssHash.values(varExp));
				for (CDataPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledDataPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;
			}

			QList<CDataPropertyAssertionExpression*> COptimizedComplexBuildingVariableCompositionsItem::getUnhanledDataPropertyAssertionsExpressions(CDataLiteralVariableExpression* varExp) {
				QList<CDataPropertyAssertionExpression*> propAssList;
				QList<CDataPropertyAssertionExpression*> tmpPropAssList(mDataVarDataPropAssHash.values(varExp));
				for (CDataPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledDataPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;
			}


			QList<CIndividualVariableExpression*> COptimizedComplexBuildingVariableCompositionsItem::getUnhandledDataExtensionIndividualTermExpressions(CDataPropertyAssertionExpression* propAssExp, CIndividualVariableExpression* varExp) {
				QList<CIndividualVariableExpression*> indiTermList;
				CIndividualVariableExpression* indiVarExp = (CIndividualVariableExpression*)propAssExp->getIndividualTermExpression();
				CDataLiteralVariableExpression* dataVarExp = (CDataLiteralVariableExpression*)propAssExp->getDataLiteralTermExpression();
				QList<CDataPropertyAssertionExpression*> propAssList(getUnhanledDataPropertyAssertionsExpressions(dataVarExp));
				for (CDataPropertyAssertionExpression* propAssExp : propAssList) {
					CIndividualVariableExpression* connIndVarExp = (CIndividualVariableExpression*)propAssExp->getIndividualTermExpression();
					if (indiVarExp != connIndVarExp) {
						indiTermList.append(connIndVarExp);
					}
				}
				return indiTermList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setDataPropertyAssertionHandled(CDataPropertyAssertionExpression* propAss) {
				mHandledDataPropAssSet.insert(propAss);
				return this;
			}



			bool COptimizedComplexBuildingVariableCompositionsItem::hasAbsorptionBasedQueryPartsOrdinaryEvaluated() {
				return mAbsorptionBasedQueryPartsOrdinaryEvaluated;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setAbsorptionBasedQueryPartsOrdinaryEvaluated(bool ordnaryEvaluated) {
				mAbsorptionBasedQueryPartsOrdinaryEvaluated = ordnaryEvaluated;
				return this;
			}


			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::getAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem() {
				return mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem(COptimizedComplexBuildingVariableCompositionsItem* subItem) {
				mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem = subItem;
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingVariableCompositionsItem::getAbsorbingQueryPartsList() {
				return &mAbsorbingQueryPartsList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbingQueryParts(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorbingQueryPartsList.append(list);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbingQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorbingQueryPartsList.append(data);
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingVariableCompositionsItem::getAbsorbedQueryPartItemExtensionHandlingList() {
				return &mAbsorptionBasedQueryPartItemExtensionHandlingList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbedBasedQueryPartsItemExtensionHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorptionBasedQueryPartItemExtensionHandlingList.append(list);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbedBasedQueryPartItemExtensionHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorptionBasedQueryPartItemExtensionHandlingList.append(data);
				return this;
			}




			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingVariableCompositionsItem::getAbsorbedQueryPartEntailmentCheckingHandlingList() {
				return &mAbsorptionBasedQueryPartEntailmentCheckingHandlingList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbedBasedQueryPartsEntailmentCheckingHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorptionBasedQueryPartEntailmentCheckingHandlingList.append(list);
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorbedBasedQueryPartEntailmentCheckingHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorptionBasedQueryPartEntailmentCheckingHandlingList.append(data);
				return this;
			}





			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingVariableBindingsPropagation() {
				return mWaitingVariableBindingsPropagationCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getWaitingVariableBindingsPropagationCount() {
				return mWaitingVariableBindingsPropagationCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incVariableBindingsPropagationCount(cint64 count) {
				mWaitingVariableBindingsPropagationCount += count;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decVariableBindingsPropagationCount(cint64 count) {
				mWaitingVariableBindingsPropagationCount -= count;
				return this;
			}


			COptimizedComplexVariableCompositionItem* COptimizedComplexBuildingVariableCompositionsItem::getVariableLastCompositionItem(CExpressionVariable* variableExp) {
				return mVarLastItemHash.value(variableExp);
			}



			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::clearLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp) {
				mVarLastItemHash.insert(lastHandledVarExp, nullptr);
				mVarLastItemAssociatedVariableHash.insert(lastHandledVarExp, nullptr);
				return this;
			}



			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::updateLastHandledVariableItemAssociation(CExpressionVariable* lastHandledVarExp, COptimizedComplexVariableCompositionItem* lastHandledItem) {
				COptimizedComplexVariableCompositionItemVariableIndexMapping* mapping = mVarItemIndexMappingHash.value(lastHandledVarExp).value(lastHandledItem);
				if (mapping) {
					for (COptimizedComplexVariableCompositionItemVariableIndexMapping::const_iterator it = mapping->constBegin(), itEnd = mapping->constEnd(); it != itEnd; ++it) {
						CExpressionVariable* variable = it.value();
						mVarLastItemHash.insert(variable, lastHandledItem);
						mVarLastItemAssociatedVariableHash.insert(variable, lastHandledVarExp);
					}
					mVarLastItemHash.insert(lastHandledVarExp, lastHandledItem);
				}
				return this;
			}


			CExpressionVariable* COptimizedComplexBuildingVariableCompositionsItem::getVariableLastCompositionItemAssociatedVariableExpression(CExpressionVariable* variableExp) {
				return mVarLastItemAssociatedVariableHash.value(variableExp);
			}



			bool COptimizedComplexBuildingVariableCompositionsItem::isWaitingVariableBindingsConfirmation() {
				return mWaitingVariableBindingsConfirmationCount > 0;
			}

			cint64 COptimizedComplexBuildingVariableCompositionsItem::getWaitingVariableBindingsConfirmationCount() {
				return mWaitingVariableBindingsConfirmationCount;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::incVariableBindingsConfirmationCount(cint64 count) {
				mWaitingVariableBindingsConfirmationCount += count;
				return this;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::decVariableBindingsConfirmationCount(cint64 count) {
				mWaitingVariableBindingsConfirmationCount -= count;
				return this;
			}

			QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* COptimizedComplexBuildingVariableCompositionsItem::getAbsorptionBasedHandlingExtensionItemList() {
				return &mAbsorptionBasedHandlingExtensionItemList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addAbsorptionBasedHandlingExtensionItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem) {
				mAbsorptionBasedHandlingExtensionItemList.append(absBasedExtItem);
				return this;
			}


			bool COptimizedComplexBuildingVariableCompositionsItem::isBindingsReducible() {
				return mBindingsReducible;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::setBindingsReducible(bool reducible) {
				mBindingsReducible = reducible;
				return this;
			}

			QList<COptimizedComplexVariableCompositionItem*>* COptimizedComplexBuildingVariableCompositionsItem::getUsedComplexVariableCompositionItemList() {
				return &mUsedComplexVariableCompositionItemList;
			}

			COptimizedComplexBuildingVariableCompositionsItem* COptimizedComplexBuildingVariableCompositionsItem::addUsedComplexVariableCompositionItem(COptimizedComplexVariableCompositionItem* item) {
				mUsedComplexVariableCompositionItemList.append(item);
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
