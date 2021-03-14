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

#include "CAnsweringSplitPropagationTestControllingItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringSplitPropagationTestControllingItem::CAnsweringSplitPropagationTestControllingItem(cint64 testingItemId, COptimizedComplexVariableAbsorptionBasedHandlingQueryPartData* absorptionData) {
				mTestingItemId = testingItemId;
				mAbsorptionData = absorptionData;
				mIntegrationOrderedVariableDataCount = 0;
				mNextIntegrationId = 0;
				mBaseSplittingVarData = nullptr;
				mExpectedIntegrationCount = 0;
				mTestActualIntegrationCount = 0;
				mIntendedIntegrationLimit = 0;
				mAllIntegrationTested = false;
				mLastIntegrationTestedVarData = nullptr;
				mLastIntegrationTestedVar = nullptr;
				mNextTestingId = 0;
			}


			CAnsweringSplitPropagationTestControllingItem::~CAnsweringSplitPropagationTestControllingItem() {
				for (QHash<CVariable*, CAnsweringSplitPropagationTestingVariableData*>::const_iterator it = mVariableDataHash.constBegin(), itEnd = mVariableDataHash.constEnd(); it != itEnd; ++it) {
					CAnsweringSplitPropagationTestingVariableData* data = it.value();
					delete data;
				}
			}


			cint64 CAnsweringSplitPropagationTestControllingItem::getTestingItemId() {
				return mTestingItemId;
			}

			CAnsweringSplitPropagationTestControllingItem* CAnsweringSplitPropagationTestControllingItem::setIntegrationLimit(cint64 limit) {
				mIntendedIntegrationLimit = limit;
				return this;
			}

			cint64 CAnsweringSplitPropagationTestControllingItem::getNextVariableDataId() {
				return mVariableDataHash.size();
			}

			CAnsweringSplitPropagationTestControllingItem* CAnsweringSplitPropagationTestControllingItem::addVariableData(CAnsweringSplitPropagationTestingVariableData* varData, bool splittingBase) {
				mVariableDataHash.insert(varData->getVariable(), varData);
				mVariableExpressionDataHash.insert(varData->getVariableExpression(), varData);

				if (splittingBase && !mBaseSplittingVarData) {
					mBaseSplittingVarData = varData;
					varData->setVariableBindingsIntegrationLimit(-1);
				} else {
					mVariableDataList.append(varData);
				}
				return this;
			}



			CAnsweringSplitPropagationTestControllingItem* CAnsweringSplitPropagationTestControllingItem::updateIntegrationOrderedVariableDataList() {
				if (mIntegrationOrderedVariableDataCount < mVariableDataList.size()) {
					for (CAnsweringSplitPropagationTestingVariableData* variableData : mVariableDataList) {
						if (!variableData->hasVariableDataIntegrationId() && variableData->getVariableBindingsIntegrationCount() > 0) {
							variableData->setVariableDataIntegrationId(mNextIntegrationId++);
							mIntegrationOrderedVariableDataList.append(variableData);
						}
					}
				}
				return this;
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestControllingItem::getBaseVariableData() {
				return mBaseSplittingVarData;
			}

			QList<CAnsweringSplitPropagationTestingVariableData*> CAnsweringSplitPropagationTestControllingItem::getIntegrationOrderedVariableDataList() {
				updateIntegrationOrderedVariableDataList();
				return mIntegrationOrderedVariableDataList;
			}

			cint64 CAnsweringSplitPropagationTestControllingItem::getExpectedIntegrationCount() {
				return mExpectedIntegrationCount;
			}

			cint64 CAnsweringSplitPropagationTestControllingItem::getTestActualIntegrationCount() {
				return mTestActualIntegrationCount;
			}


			cint64 CAnsweringSplitPropagationTestControllingItem::getIntendedIntegrationLimit() {
				return mIntendedIntegrationLimit;
			}

			bool CAnsweringSplitPropagationTestControllingItem::tryExtendVariableIntegrationLimits(CVariable** integrationExtendedVariable) {
				if (mTestActualIntegrationCount < mIntendedIntegrationLimit / 2) {
					updateIntegrationOrderedVariableDataList();

					bool allVariableIntegration = mVariableDataList.size() == mIntegrationOrderedVariableDataList.size();
					cint64 collIntegrationCount = 0;
					for (auto rit = mIntegrationOrderedVariableDataList.crbegin(); rit != mIntegrationOrderedVariableDataList.crend(); ++rit) {
						CAnsweringSplitPropagationTestingVariableData* varData = *rit;
						if (varData->hasVariableBiningsIntegrationLimitReached()) {
							cint64 varDataIntCount = varData->getVariableBindingsIntegrationCount();
							cint64 varDataIntLimit = varData->getVariableBindingsIntegrationLimit();
							cint64 varDataIntReq = varData->getVariableBindingsRemainingIntegrationRequirementCount();
							double integratedIndiCountPerBinding = 1;
							collIntegrationCount += varDataIntCount;

							integratedIndiCountPerBinding = (double)collIntegrationCount / (double)varDataIntCount;
							double additionalPossibleIntegrationIndiCount = mIntendedIntegrationLimit - mTestActualIntegrationCount;

							double varIntegrationLimitExtensibleSize = additionalPossibleIntegrationIndiCount / integratedIndiCountPerBinding;
							if (!allVariableIntegration) {
								varIntegrationLimitExtensibleSize /= 2.;
							}
							cint64 varIntegrationLimitExtensibleCount = varIntegrationLimitExtensibleSize;
							if (varIntegrationLimitExtensibleCount <= 0) {
								varIntegrationLimitExtensibleCount = 1;
							}
							if (integrationExtendedVariable) {
								*integrationExtendedVariable = varData->getVariable();
							}
							varData->setVariableBindingsIntegrationLimit(varDataIntLimit + varIntegrationLimitExtensibleCount);
							return true;
						} else {
							collIntegrationCount += varData->getVariableBindingsIntegrationCount();
							if (varData->getVariableBindingsIntegrationCount() <= 0) {
								allVariableIntegration = false;
							}
						}
					}

				}
				return false;
			}



			bool CAnsweringSplitPropagationTestControllingItem::reconfigureVariableIntegrationLimits(cint64 newIntededIntegrationLimit) {
				updateIntegrationOrderedVariableDataList();
				bool allVariableIntegration = mVariableDataList.size() == mIntegrationOrderedVariableDataList.size();
				bool previousLimitReached = mTestActualIntegrationCount >= mIntendedIntegrationLimit;

				double sufficientChangeFactor = 1.;
				double insufficientChangeFactor = 1.;
				if (mTestActualIntegrationCount > 0) {
					insufficientChangeFactor = (double)newIntededIntegrationLimit / (double)mTestActualIntegrationCount;
					sufficientChangeFactor = insufficientChangeFactor * 1.1;
				} else {
					return false;
				}

				double consideringTestIntegrationCount = mTestActualIntegrationCount;
				double collExpectedIntegrationCount = 0;
				for (auto rit = mIntegrationOrderedVariableDataList.crbegin(); rit != mIntegrationOrderedVariableDataList.crend(); ++rit) {
					CAnsweringSplitPropagationTestingVariableData* varData = *rit;
					collExpectedIntegrationCount += varData->getVariableBindingsIntegrationCount();
					if (!varData->hasVariableBiningsIntegrationLimitReached()) {
						cint64 varDataIntCount = varData->getVariableBindingsIntegrationCount();
						cint64 varDataIntLimit = varData->getVariableBindingsIntegrationLimit();
						double integratedIndiCountPerBinding = (double)collExpectedIntegrationCount / (double)varDataIntCount;

						varData->setVariableBindingsIntegrationLimit(varDataIntCount * sufficientChangeFactor);

					} else {

						cint64 varDataIntCount = varData->getVariableBindingsIntegrationCount();
						cint64 varDataIntLimit = varData->getVariableBindingsIntegrationLimit();
						cint64 varDataIntReq = varData->getVariableBindingsRemainingIntegrationRequirementCount();
						double integratedIndiCountPerBinding = (double)collExpectedIntegrationCount / (double)varDataIntCount;
						//double newCollExpectedIntegrationCount = integratedIndiCountPerBinding * (varDataIntCount + varDataIntReq);

						double additionalNewIntCount = (newIntededIntegrationLimit - (consideringTestIntegrationCount - collExpectedIntegrationCount));
						double remainIntIncFactor = additionalNewIntCount / collExpectedIntegrationCount;
						varData->setVariableBindingsIntegrationLimit(varDataIntCount * remainIntIncFactor);


						double newVarDataIntCount = varDataIntCount * remainIntIncFactor;
						if (newVarDataIntCount > varDataIntCount + varDataIntReq) {
							remainIntIncFactor = qMin(remainIntIncFactor, (double)(varDataIntCount + varDataIntReq) / (double)varDataIntCount);
							newVarDataIntCount = varDataIntCount * remainIntIncFactor;

							double newCollExpectedIntegrationCount = collExpectedIntegrationCount * remainIntIncFactor;

							double additionalCollExpectedIntegrationCount = newCollExpectedIntegrationCount - collExpectedIntegrationCount;
							collExpectedIntegrationCount = newCollExpectedIntegrationCount;
							consideringTestIntegrationCount += additionalCollExpectedIntegrationCount;
							insufficientChangeFactor = (double)newIntededIntegrationLimit / (double)consideringTestIntegrationCount;
							sufficientChangeFactor = insufficientChangeFactor * 1.1;

							varData->setVariableBindingsIntegrationLimit(newVarDataIntCount * insufficientChangeFactor);

						} else {
							break;
						}

					}
				}
				mIntendedIntegrationLimit = newIntededIntegrationLimit;
				mTestActualIntegrationCount = 0;
				return true;

			}


			cint64 CAnsweringSplitPropagationTestControllingItem::getNextTestingId() {
				return mNextTestingId++;
			}


			bool CAnsweringSplitPropagationTestControllingItem::hasCheckedAllVariableIntegrations() {
				return mAllIntegrationTested;
			}


			bool CAnsweringSplitPropagationTestControllingItem::disableCheckedVariableIntegrations() {
				mBaseSplittingVarData->resetIntegrations();
				updateIntegrationOrderedVariableDataList();
				CAnsweringSplitPropagationTestingVariableData* lastRemainingIntReqVarData = nullptr;
				for (auto rit = mIntegrationOrderedVariableDataList.crbegin(); rit != mIntegrationOrderedVariableDataList.crend(); ++rit) {
					CAnsweringSplitPropagationTestingVariableData* varData = *rit;
					if (varData->getVariableBindingsRemainingIntegrationRequirementCount() > 0) {
						lastRemainingIntReqVarData = varData;
						break;
					}
				}

				if (lastRemainingIntReqVarData) {
					bool varDataIsBeforeLastReachedIntLimit = true;
					for (auto rit = mIntegrationOrderedVariableDataList.crbegin(); rit != mIntegrationOrderedVariableDataList.crend(); ++rit) {
						CAnsweringSplitPropagationTestingVariableData* varData = *rit;
						if (lastRemainingIntReqVarData == varData) {
							// disable bindings
							varData->disableIntegrations();
							varDataIsBeforeLastReachedIntLimit = false;
							if (varData->hasAllVariableBindingsDisabled()) {
								varData->resetIntegrations();
								return disableCheckedVariableIntegrations();								
							}
						} else if (!varDataIsBeforeLastReachedIntLimit) {
							// reset bindings
							varData->resetIntegrations();
						}
					}
					return true;
				} else {
					mAllIntegrationTested = true;
					return false;
				}

			}


			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestControllingItem::getVariableData(CVariable* var) {
				if (mLastIntegrationTestedVar != var) {
					mLastIntegrationTestedVarData = mVariableDataHash.value(var);
					mLastIntegrationTestedVar = var;
				}
				return mLastIntegrationTestedVarData;
			}


            bool CAnsweringSplitPropagationTestControllingItem::canIntegrateVariableBinding(CVariable* var, const CIndividualReference& indiRef) {
				if (mBaseSplittingVarData->getVariable() == var) {
					return mBaseSplittingVarData->canIntegrateBinding(indiRef, &mTestActualIntegrationCount);
				} else {
					CAnsweringSplitPropagationTestingVariableData* varData = mLastIntegrationTestedVarData;
					if (mLastIntegrationTestedVar != var) {
						mLastIntegrationTestedVarData = mVariableDataHash.value(var);
						mLastIntegrationTestedVar = var;
					}
					if (mLastIntegrationTestedVarData) {
						return mLastIntegrationTestedVarData->canIntegrateBinding(indiRef, &mTestActualIntegrationCount);
					}
				}
				return false;
			}

			CAnsweringPropagationExtendedSteeringController* CAnsweringSplitPropagationTestControllingItem::incVariableBindingsRemainingIntegrationRequirementCount(CVariable* var, cint64 count) {
				if (mBaseSplittingVarData->getVariable() != var) {
					CAnsweringSplitPropagationTestingVariableData* varData = mLastIntegrationTestedVarData;
					if (mLastIntegrationTestedVar != var) {
						mLastIntegrationTestedVarData = mVariableDataHash.value(var);
						mLastIntegrationTestedVar = var;
					}
					if (mLastIntegrationTestedVarData) {
						mLastIntegrationTestedVarData->incVariableBindingsRemainingIntegrationRequirementCount(count);
					}
				}
				return this;
			}


			bool CAnsweringSplitPropagationTestControllingItem::finalizeWithClashing() {
				return false;
			}

			bool CAnsweringSplitPropagationTestControllingItem::finalizeWithBindingExtraction() {
				return true;
			}

			bool CAnsweringSplitPropagationTestControllingItem::isPreparationBindingNominalIndividual(CVariable* variable, CIndividual* indi) {
				return isPreparationBindingNominalIndividual(variable, indi->getIndividualID());
			}

			bool CAnsweringSplitPropagationTestControllingItem::isPreparationBindingNominalIndividual(CVariable* variable, cint64 indiId) {
                CIndividualReference indiRef(indiId);
                return canIntegrateVariableBinding(variable, indiRef);
			}

			bool CAnsweringSplitPropagationTestControllingItem::isPreparationBindingAllIndividuals(CVariable* variable) {
				CExpressionVariable* expVariable = mAbsorptionData->getExpressionVariableVariableHash()->value(variable);
				if (mAbsorptionData->getPrepareVariableSet()->contains(expVariable)) {
					return false;
				}
				return false;
			}


			CAnsweringSplitPropagationTestControllingItem* CAnsweringSplitPropagationTestControllingItem::reset() {
				for (CAnsweringSplitPropagationTestingVariableData* variableData : mVariableDataList) {
					variableData->resetIntegrations();
				}
				mIntegrationOrderedVariableDataCount = 0;
				mAllIntegrationTested = false;
				mNextTestingId = 0;

				return this;
			}

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
