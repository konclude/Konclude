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

#include "COptimizedComplexBuildingIndividualVariableCompositionsItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexBuildingIndividualVariableCompositionsItem::COptimizedComplexBuildingIndividualVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData)
					: COptimizedComplexBuildingVariableCompositionsItem(queryProcessingData) {

				mWaitingComputationStepsCount = 0;
				mInitializedBaseConceptVariableItems = false;
				mInitializedBasicDataVariableItems = false;
				mJoinedBasicDataVariableItems = false;
				mWaitingRoleTargetsRealizationCount = 0;
				mWaitingVariableBindingsPropagationCount = 0;
				mWaitingVariableBindingsConfirmationCount = 0;
				mInitializedConceptDataVariableExtensionItems = false;
				mBindingsReducible = false;
				mAbsorptionBasedQueryPartsOrdinaryEvaluated = false;
				mTemporaryMaterializationData = nullptr;
				mBuildingFinishingData = nullptr;
				mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem = nullptr;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem::~COptimizedComplexBuildingIndividualVariableCompositionsItem() {
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
			 

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::createExtendingBuildingVariableCompositionsItem() {
				COptimizedComplexBuildingIndividualVariableCompositionsItem* extBuildingVarItem = new COptimizedComplexBuildingIndividualVariableCompositionsItem(mQueryProcessingData);
				extBuildingVarItem->mVarItemIndexMappingHash = mVarItemIndexMappingHash;
				extBuildingVarItem->mVarVarCompItemHash = mVarVarCompItemHash;
				extBuildingVarItem->mUsedComplexVariableCompositionItemSet = mUsedComplexVariableCompositionItemSet;
				for (COptimizedComplexVariableCompositionItem* usedComplexVariableCompositionItem : mUsedComplexVariableCompositionItemSet) {
					usedComplexVariableCompositionItem->incQueryUsageCount();
				}
				extBuildingVarItem->mUsedComplexConceptItemSet = mUsedComplexConceptItemSet;
				for (COptimizedComplexConceptItem* usedComplexConceptItem : mUsedComplexConceptItemSet) {
					usedComplexConceptItem->incQueryUsageCount();
				}
				extBuildingVarItem->mVarLastItemHash = mVarLastItemHash;
				extBuildingVarItem->mVarLastItemAssociatedVariableHash = mVarLastItemAssociatedVariableHash;
				extBuildingVarItem->mLastHandVarExp = mLastHandVarExp;
				extBuildingVarItem->mAllVarExpSet = mAllVarExpSet;
				return extBuildingVarItem;
			}





			CComplexQueryMaterializationData* COptimizedComplexBuildingIndividualVariableCompositionsItem::getMaterializationData() {
				return mTemporaryMaterializationData;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setMaterializationData(CComplexQueryMaterializationData* tmpOnto) {
				mTemporaryMaterializationData = tmpOnto;
				return this;
			}



			CComplexQueryFinishingBuildingVariableCompositionsItemData* COptimizedComplexBuildingIndividualVariableCompositionsItem::getBuildingFinishingData() {
				return mBuildingFinishingData;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setBuildingFinishingData(CComplexQueryFinishingBuildingVariableCompositionsItemData* data) {
				mBuildingFinishingData = data;
				return this;
			}
			



			QSet<CExpressionVariable*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getVariableSingleCardinalitySet() {
				return &mVariableSingleCardinalitySet;
			}

			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::hasVariableSingleCardinality(CExpressionVariable* varExp) {
				return mVariableSingleCardinalitySet.contains(varExp);
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariableComplexConceptItem(CIndividualVariableExpression* varExp, COptimizedComplexConceptItem* conItem) {
				mVarConItemHash.insert(varExp, conItem);
				mRemainingVarExpSet.insert(varExp);
				mReuseVarExpComputationsCheckSet.insert(varExp);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariablePropertyAssertion(CIndividualVariableExpression* varExp, CObjectPropertyAssertionExpression* propAssExp) {
				mRemainingVarExpSet.insert(varExp);
				mReuseVarExpComputationsCheckSet.insert(varExp);
				mVarPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariablePropertyAssertions(const QList<CObjectPropertyAssertionExpression*>& propAssExpList) {
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


			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getRemainingVariableExpressionSet() {
				return &mRemainingVarExpSet;
			}




			QList<CObjectPropertyAssertionExpression*> COptimizedComplexBuildingIndividualVariableCompositionsItem::getUnhanledPropertyAssertionsExpressions(CIndividualVariableExpression* varExp) {
				QList<CObjectPropertyAssertionExpression*> propAssList;
				QList<CObjectPropertyAssertionExpression*> tmpPropAssList(mVarPropAssHash.values(varExp));
				for (CObjectPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;

			}



			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isInitializedBaseConceptVariableItems() {
				return mInitializedBaseConceptVariableItems;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setInitializedBaseConceptVariableItems(bool initialized) {
				mInitializedBaseConceptVariableItems = initialized;
				return this;
			}



			CIndividualVariableExpression* COptimizedComplexBuildingIndividualVariableCompositionsItem::getOtherVariableExpression(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss) {
				CIndividualVariableExpression* otherVarExp = (CIndividualVariableExpression*)propAss->getFirstIndividualTermExpression();
				if (otherVarExp != varEpx) {
					return otherVarExp;
				} else {
					return (CIndividualVariableExpression*)propAss->getSecondIndividualTermExpression();
				}
			}


			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isRemovingPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CObjectPropertyAssertionExpression* propAss) {
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






			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isRemovingDataPropertyAssertionDisconnecting(CIndividualVariableExpression* varEpx, CDataPropertyAssertionExpression* dataPropAss) {
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




			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setPropertyAssertionHandled(CObjectPropertyAssertionExpression* propAss) {
				mHandledPropAssSet.insert(propAss);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setVariableExpressionHandled(CIndividualVariableExpression* varEpx) {
				mHandledVarExpSet.insert(varEpx);
				mRemainingVarExpSet.remove(varEpx);
				return this;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setVariableExpressionUnhandled(CIndividualVariableExpression* varEpx) {
				mHandledVarExpSet.remove(varEpx);
				mRemainingVarExpSet.insert(varEpx);
				return this;
			}












			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isWaitingRoleTargetsRealization() {
				return mWaitingRoleTargetsRealizationCount > 0;
			}

			cint64 COptimizedComplexBuildingIndividualVariableCompositionsItem::getWaitingRoleTargetsRealizationCount() {
				return mWaitingRoleTargetsRealizationCount;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::incWaitingRoleTargetsRealizationCount(cint64 count) {
				mWaitingRoleTargetsRealizationCount += count;
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::decWaitingRoleTargetsRealizationCount(cint64 count) {
				mWaitingRoleTargetsRealizationCount -= count;
				return this;
			}


			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isWaitingComputation() {
				return hasWaitingVariableCompositionItems() || isWaitingSubVariableBuildingItems() || isWaitingRoleTargetsRealization() || isWaitingComputationStep() || isWaitingVariableBindingsPropagation() || isWaitingVariableBindingsConfirmation();
			}



			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getReuseVariableExpressionComputationsCheckSet() {
				return &mReuseVarExpComputationsCheckSet;
			}

			QSet<CExpressionVariable*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAllVariableSet() {
				return &mAllVarExpSet;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setAllVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mAllVarExpSet = varSet;
				return this;
			}

			QSet<CExpressionVariable*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getDistinguishedVariableSet() {
				return &mDistinguishedVarExpSet;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setDistinguishedVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mDistinguishedVarExpSet = varSet;
				return this;
			}


			QSet<CExpressionVariable*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getReductionDeniedVariableSet() {
				return &mReductionDeniedVariableSet;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setReductionDeniedVariableSet(const QSet<CExpressionVariable*>& varSet) {
				mReductionDeniedVariableSet = varSet;
				return this;
			}


			QHash<CDataLiteralVariableExpression*, CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getDataLiteralVariableIndidualVariableMappingHash() {
				return &mDataVarIndiVarHash;
			}

			QHash<CIndividualVariableExpression*, QHash<CDataLiteralVariableExpression*, CDataPropertyTermExpression*>>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getIndidualVariableDataLiteralVariableRoleMappingHash() {
				return &mIndiVarDataVarHash;
			}

			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isInitializedConceptDataVariableExtensionItems() {
				return mInitializedConceptDataVariableExtensionItems;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setInitializedConceptDataVariableExtensionItems(bool initialized) {
				mInitializedConceptDataVariableExtensionItems = initialized;
				return this;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addIndidualVariableDataLiteralVariableRoleRequiredExtension(CIndividualVariableExpression* indiVar, CDataLiteralVariableExpression* dataLitVar, CDataPropertyTermExpression* dataRole) {
				mIndiVarDataVarHash[indiVar].insertMulti(dataLitVar, dataRole);
				mDataVarIndiVarHash.insert(dataLitVar, indiVar);
				mRemainingDataVarExpSet.insert(indiVar);
				return this;
			}


			QSet<CIndividualVariableExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getRemainingExtendingDataVariableSet() {
				return &mRemainingDataVarExpSet;
			}




			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariableDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList) {
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



			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isJoinedBasicDataVariableItems() {
				return mJoinedBasicDataVariableItems;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setJoinedBasicDataVariableItems(bool initialized) {
				mJoinedBasicDataVariableItems = initialized;
				return this;
			}

			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isInitializedBasicDataVariableItems() {
				return mInitializedBasicDataVariableItems;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setInitializedBasicDataVariableItems(bool initialized) {
				mInitializedBasicDataVariableItems = initialized;
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addBasicDataPropertyAssertions(const QList<CDataPropertyAssertionExpression*>& dataPropAssExpList) {
				mBasicDataPropAssList.append(dataPropAssExpList);
				for (CDataPropertyAssertionExpression* propAssExp : dataPropAssExpList) {
					CDataLiteralVariableExpression* dataVarExp = dynamic_cast<CDataLiteralVariableExpression*>(propAssExp->getDataLiteralTermExpression());
					if (dataVarExp) {
						mBasicDataVarJoiningSet.insert(dataVarExp);
					}
				}
				return this;
			}


			QList<CDataPropertyAssertionExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getBasicDataPropertyAssertionList() {
				return &mBasicDataPropAssList;
			}

			QSet<CDataLiteralVariableExpression*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getBasicDataVariableJoiningSet() {
				return &mBasicDataVarJoiningSet;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariableDataPropertyAssertion(CIndividualVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp) {
				mVarDataPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addVariableDataPropertyAssertion(CDataLiteralVariableExpression* varExp, CDataPropertyAssertionExpression* propAssExp) {
				mDataVarDataPropAssHash.insertMulti(varExp, propAssExp);
				return this;
			}

			QList<CDataPropertyAssertionExpression*> COptimizedComplexBuildingIndividualVariableCompositionsItem::getUnhanledDataPropertyAssertionsExpressions(CIndividualVariableExpression* varExp) {
				QList<CDataPropertyAssertionExpression*> propAssList;
				QList<CDataPropertyAssertionExpression*> tmpPropAssList(mVarDataPropAssHash.values(varExp));
				for (CDataPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledDataPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;
			}

			QList<CDataPropertyAssertionExpression*> COptimizedComplexBuildingIndividualVariableCompositionsItem::getUnhanledDataPropertyAssertionsExpressions(CDataLiteralVariableExpression* varExp) {
				QList<CDataPropertyAssertionExpression*> propAssList;
				QList<CDataPropertyAssertionExpression*> tmpPropAssList(mDataVarDataPropAssHash.values(varExp));
				for (CDataPropertyAssertionExpression* propAssExp : tmpPropAssList) {
					if (!mHandledDataPropAssSet.contains(propAssExp)) {
						propAssList.append(propAssExp);
					}
				}
				return propAssList;
			}


			QList<CIndividualVariableExpression*> COptimizedComplexBuildingIndividualVariableCompositionsItem::getUnhandledDataExtensionIndividualTermExpressions(CDataPropertyAssertionExpression* propAssExp, CIndividualVariableExpression* varExp) {
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

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setDataPropertyAssertionHandled(CDataPropertyAssertionExpression* propAss) {
				mHandledDataPropAssSet.insert(propAss);
				return this;
			}



			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::hasAbsorptionBasedQueryPartsOrdinaryEvaluated() {
				return mAbsorptionBasedQueryPartsOrdinaryEvaluated;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setAbsorptionBasedQueryPartsOrdinaryEvaluated(bool ordnaryEvaluated) {
				mAbsorptionBasedQueryPartsOrdinaryEvaluated = ordnaryEvaluated;
				return this;
			}


			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem() {
				return mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem(COptimizedComplexBuildingIndividualVariableCompositionsItem* subItem) {
				mAbsorptionBasedQueryPartsOrdinaryEvaluationSubVariableBuiltItem = subItem;
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorbingQueryPartsList() {
				return &mAbsorbingQueryPartsList;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbingQueryParts(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorbingQueryPartsList.append(list);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbingQueryPart(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorbingQueryPartsList.append(data);
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorbedQueryPartItemExtensionHandlingList() {
				return &mAbsorptionBasedQueryPartItemExtensionHandlingList;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbedBasedQueryPartsItemExtensionHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorptionBasedQueryPartItemExtensionHandlingList.append(list);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbedBasedQueryPartItemExtensionHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorptionBasedQueryPartItemExtensionHandlingList.append(data);
				return this;
			}




			QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorbedQueryPartEntailmentCheckingHandlingList() {
				return &mAbsorptionBasedQueryPartEntailmentCheckingHandlingList;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbedBasedQueryPartsEntailmentCheckingHandling(const QList<COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData*>& list) {
				mAbsorptionBasedQueryPartEntailmentCheckingHandlingList.append(list);
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorbedBasedQueryPartEntailmentCheckingHandling(COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* data) {
				mAbsorptionBasedQueryPartEntailmentCheckingHandlingList.append(data);
				return this;
			}





			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isWaitingVariableBindingsPropagation() {
				return mWaitingVariableBindingsPropagationCount > 0;
			}

			cint64 COptimizedComplexBuildingIndividualVariableCompositionsItem::getWaitingVariableBindingsPropagationCount() {
				return mWaitingVariableBindingsPropagationCount;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::incVariableBindingsPropagationCount(cint64 count) {
				mWaitingVariableBindingsPropagationCount += count;
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::decVariableBindingsPropagationCount(cint64 count) {
				mWaitingVariableBindingsPropagationCount -= count;
				return this;
			}




			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isWaitingVariableBindingsConfirmation() {
				return mWaitingVariableBindingsConfirmationCount > 0;
			}

			cint64 COptimizedComplexBuildingIndividualVariableCompositionsItem::getWaitingVariableBindingsConfirmationCount() {
				return mWaitingVariableBindingsConfirmationCount;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::incVariableBindingsConfirmationCount(cint64 count) {
				mWaitingVariableBindingsConfirmationCount += count;
				return this;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::decVariableBindingsConfirmationCount(cint64 count) {
				mWaitingVariableBindingsConfirmationCount -= count;
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorptionBasedHandlingExtensionItemPropagationList() {
				return &mAbsorptionBasedHandlingExtensionItemPropagationList;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorptionBasedHandlingExtensionPropagationItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem) {
				mAbsorptionBasedHandlingExtensionItemPropagationList.append(absBasedExtItem);
				return this;
			}



			QList<COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getAbsorptionBasedHandlingExtensionItemJoiningList() {
				return &mAbsorptionBasedHandlingExtensionItemJoiningList;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::addAbsorptionBasedHandlingExtensionJoiningItem(COptimizedComplexVariableAbsorptionBasedHandlingExtensionItem* absBasedExtItem) {
				mAbsorptionBasedHandlingExtensionItemJoiningList.append(absBasedExtItem);
				return this;
			}



			bool COptimizedComplexBuildingIndividualVariableCompositionsItem::isBindingsReducible() {
				return mBindingsReducible;
			}

			COptimizedComplexBuildingIndividualVariableCompositionsItem* COptimizedComplexBuildingIndividualVariableCompositionsItem::setBindingsReducible(bool reducible) {
				mBindingsReducible = reducible;
				return this;
			}



			QList<CExpressionVariable*>* COptimizedComplexBuildingIndividualVariableCompositionsItem::getExtendibleQueryTransferringExtractionVariableList() {
				return &mExtendibleQueryTransferringExtractionVariableList;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
