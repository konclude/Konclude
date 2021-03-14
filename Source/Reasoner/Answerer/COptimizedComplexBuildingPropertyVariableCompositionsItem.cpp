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

#include "COptimizedComplexBuildingPropertyVariableCompositionsItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			COptimizedComplexBuildingPropertyVariableCompositionsItem::COptimizedComplexBuildingPropertyVariableCompositionsItem(CComplexQueryExpressionProcessingData* queryProcessingData) 
					: COptimizedComplexBuildingVariableCompositionsItem(queryProcessingData) {

				mInitializedBaseRoleVariableItems = false;
			}


			COptimizedComplexBuildingPropertyVariableCompositionsItem::~COptimizedComplexBuildingPropertyVariableCompositionsItem() {
			}


			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::addVariableComplexSuperRole(CPropertyVariableExpression* varExp, CRole* role) {
				mVarSuperRoleHash.insert(varExp, role);
				mRemainingVarExpSet.insert(varExp);
				mAllVarExpSet.insert(varExp);
				return this;
			}

			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::addVariableComplexSubRole(CPropertyVariableExpression* varExp, CRole* role) {
				mVarSubRoleHash.insert(varExp, role);
				mRemainingVarExpSet.insert(varExp);
				mAllVarExpSet.insert(varExp);
				return this;
			}
			


			bool COptimizedComplexBuildingPropertyVariableCompositionsItem::isWaitingComputation() {
				return hasWaitingVariableCompositionItems() || isWaitingSubVariableBuildingItems();
			}



			QSet<CPropertyVariableExpression*>* COptimizedComplexBuildingPropertyVariableCompositionsItem::getRemainingVariableExpressionSet() {
				return &mRemainingVarExpSet;
			}

			QSet<CPropertyVariableExpression*>* COptimizedComplexBuildingPropertyVariableCompositionsItem::getAllVariableSet() {
				return &mAllVarExpSet;
			}

			QList<CRole*> COptimizedComplexBuildingPropertyVariableCompositionsItem::getVariableSuperRoles(CPropertyVariableExpression* varExp) {
				return mVarSuperRoleHash.values(varExp);
			}

			QList<CRole*> COptimizedComplexBuildingPropertyVariableCompositionsItem::getVariableSubRoles(CPropertyVariableExpression* varExp) {
				return mVarSubRoleHash.values(varExp);
			}



			bool COptimizedComplexBuildingPropertyVariableCompositionsItem::isInitializedBaseRoleVariableItems() {
				return mInitializedBaseRoleVariableItems;
			}

			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::setInitializedBaseRoleVariableItems(bool initialized) {
				mInitializedBaseRoleVariableItems = initialized;
				return this;
			}




			QSet<QPair<CExpressionVariable*, CExpressionVariable*>>* COptimizedComplexBuildingPropertyVariableCompositionsItem::getVariableSubSuperPropagationSet() {
				return &mVariableSubSuperPropagationSet;
			}

			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::addVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar) {
				mVariableSubSuperPropagationSet.insert(QPair<CExpressionVariable*, CExpressionVariable*>(subVar, superVar));
				mVariableSubPropagationHash.insertMulti(superVar, subVar);
				mVariableSuperPropagationHash.insertMulti(subVar, superVar);
				return this;
			}


			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::removeVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar) {
				mVariableSubSuperPropagationSet.remove(QPair<CExpressionVariable*, CExpressionVariable*>(subVar, superVar));
				return this;
			}

			bool COptimizedComplexBuildingPropertyVariableCompositionsItem::hasVariableSubSuperPropagation(CExpressionVariable* subVar, CExpressionVariable* superVar) {
				return mVariableSubSuperPropagationSet.contains(QPair<CExpressionVariable*, CExpressionVariable*>(subVar, superVar));
			}


			QSet<QPair<CExpressionVariable*, CExpressionVariable*>> COptimizedComplexBuildingPropertyVariableCompositionsItem::getVariableRemainingSubSuperPropagationSet(CExpressionVariable* varExp) {
				QSet<QPair<CExpressionVariable*, CExpressionVariable*>> variableSubSuperPropagationSet;
				QList<CExpressionVariable*> subVarList = mVariableSubPropagationHash.values(varExp);
				QList<CExpressionVariable*> superVarList = mVariableSuperPropagationHash.values(varExp);
				for (CExpressionVariable* subVarExp : subVarList) {
					if (!mVariableSubSuperPropagationSet.contains(QPair<CExpressionVariable*, CExpressionVariable*>(subVarExp, varExp))) {
						variableSubSuperPropagationSet.insert(QPair<CExpressionVariable*, CExpressionVariable*>(subVarExp, varExp));
					}
				}
				for (CExpressionVariable* superVarExp : superVarList) {
					if (!mVariableSubSuperPropagationSet.contains(QPair<CExpressionVariable*, CExpressionVariable*>(varExp, superVarExp))) {
						variableSubSuperPropagationSet.insert(QPair<CExpressionVariable*, CExpressionVariable*>(varExp, superVarExp));
					}
				}
				return variableSubSuperPropagationSet;
			}




			bool COptimizedComplexBuildingPropertyVariableCompositionsItem::isRemovingPropagationDisconnecting(CExpressionVariable* varExp, CExpressionVariable* subVar, CExpressionVariable* superVar) {

				CExpressionVariable* otherVarEpx = superVar;
				if (varExp == superVar) {
					otherVarEpx = subVar;
				}
				QSet<CExpressionVariable*> reachedVarExpSet;
				QList<CExpressionVariable*> testingVarExpList;
				testingVarExpList.append(otherVarEpx);
				reachedVarExpSet.insert(otherVarEpx);

				while (!testingVarExpList.isEmpty()) {
					CExpressionVariable* testingVarEpx = testingVarExpList.takeFirst();
					QSet<QPair<CExpressionVariable*, CExpressionVariable*>> variableSubSuperPropagationSet = getVariableRemainingSubSuperPropagationSet(testingVarEpx);
					for (QPair<CExpressionVariable*, CExpressionVariable* > variableSubSuperPair : variableSubSuperPropagationSet) {
						if (variableSubSuperPair.first != subVar && variableSubSuperPair.second != superVar) {
							CExpressionVariable* testConnOtherVar = variableSubSuperPair.first;
							if (testConnOtherVar == testingVarEpx) {
								testConnOtherVar = variableSubSuperPair.second;
							}
							if (testConnOtherVar == varExp) {
								return false;
							}
							if (!reachedVarExpSet.contains(testConnOtherVar)) {
								reachedVarExpSet.insert(testConnOtherVar);
								testingVarExpList.append(testConnOtherVar);
							}

						}
					}

				}
				return true;
			}


			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::setVariableExpressionUnhandled(CPropertyVariableExpression* varExp) {
				mRemainingVarExpSet.insert(varExp);
				return this;
			}

			COptimizedComplexBuildingPropertyVariableCompositionsItem* COptimizedComplexBuildingPropertyVariableCompositionsItem::setVariableExpressionHandled(CPropertyVariableExpression* varExp) {
				mRemainingVarExpSet.remove(varExp);
				return this;
			}



		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
