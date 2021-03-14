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

#include "CAnsweringSplitPropagationTestingVariableData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringSplitPropagationTestingVariableData::CAnsweringSplitPropagationTestingVariableData(cint64 variableDataId, CVariable* variable, CExpressionVariable* variableExpression, QSet<CIndividualReference>* bindingSet) {
				mVariableDataId = variableDataId;
				mIntegratedDataLinker = nullptr;
				mIntegratedBindingsCount = 0;
				mIntegrationBindingsLimit = 10;
				mVariable = variable;
				mVariableExpression = variableExpression;
				mVariableDataIntegrationId = -1;
				mDisabledBindingsCount = 0;
				mLastIntegratedBindingsCount = 0;
				mBindingSet = bindingSet;
				mRemainingIntegrationRequirementCount = 0;

				if (bindingSet) {
					addIntegrateableVariableBindings(bindingSet);
				} else {
					mDynamicBindings = true;
				}
			}


			QSet<CIndividualReference>* CAnsweringSplitPropagationTestingVariableData::getVariableBindingIndividualSet() {
				return mBindingSet;
			}


			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::setVariableBindingsIntegrationLimit(cint64 limit) {
				mIntegrationBindingsLimit = limit;
				return this;
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::addIntegrateableVariableBinding(const CIndividualReference& indiRef) {
				mBindingIndiDataHash[indiRef];
				mDynamicBindings = false;
				return this;
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::addIntegrateableVariableBindings(QSet<CIndividualReference>* bindingSet) {
				if (bindingSet) {
					for (CIndividualReference indiRef : *bindingSet) {
						CAnsweringSplitPropagationTestingVariableBindingData& bindingData = mBindingIndiDataHash[indiRef];
					}
					mDynamicBindings = false;
				}
				return this;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getIntegrateableVariableBindingCount() {
				return mBindingIndiDataHash.size();
			}


			bool CAnsweringSplitPropagationTestingVariableData::visitIntegrateableVariableBindings(function<bool(const CIndividualReference& indiRef)> visitFunc) {
				bool continueVisiting = true;
				bool visited = false;
				for (QHash<CIndividualReference, CAnsweringSplitPropagationTestingVariableBindingData>::const_iterator it = mBindingIndiDataHash.constBegin(), itEnd = mBindingIndiDataHash.constEnd(); it != itEnd && continueVisiting; ++it) {
					continueVisiting = visitFunc(it.key());
					visited = true;
				}
				return visited;
			}


			cint64 CAnsweringSplitPropagationTestingVariableData::getVariableDataId() {
				return mVariableDataId;
			}

			CVariable* CAnsweringSplitPropagationTestingVariableData::getVariable() {
				return mVariable;
			}

			CExpressionVariable* CAnsweringSplitPropagationTestingVariableData::getVariableExpression() {
				return mVariableExpression;
			}

			QHash<CIndividualReference, CAnsweringSplitPropagationTestingVariableBindingData>* CAnsweringSplitPropagationTestingVariableData::getBindingIndividualDataHash() {
				return &mBindingIndiDataHash;
			}


			bool CAnsweringSplitPropagationTestingVariableData::canIntegrateBinding(const CIndividualReference& indiRef, cint64* integationCounter) {
				bool checkIntegrationData = false;
				if (mDynamicBindings && (mIntegrationBindingsLimit < 0 || mIntegratedBindingsCount < mIntegrationBindingsLimit)) {
					checkIntegrationData = true;
				} else if (mBindingIndiDataHash.contains(indiRef)) {
					checkIntegrationData = true;
				}
				if (checkIntegrationData) {
					CAnsweringSplitPropagationTestingVariableBindingData& bindingData = mBindingIndiDataHash[indiRef];
					if (bindingData.isIntegrated()) {
						return true;
					} else {
						if (!bindingData.isDisabled()) {
							if (mIntegrationBindingsLimit < 0 || mIntegratedBindingsCount < mIntegrationBindingsLimit) {
								bindingData.setIntegrated();
								if (integationCounter) {
									(*integationCounter) += 1;
								}
								++mIntegratedBindingsCount;
								bindingData.setNext(mIntegratedDataLinker);
								mIntegratedDataLinker = &bindingData;
								return true;
							} else {
								++mRemainingIntegrationRequirementCount;
								return false;
							}
						} else {
							return false;
						}
					}

				} else {
					return false;
				}
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::resetIntegrations() {
				for (CAnsweringSplitPropagationTestingVariableBindingData* integratedDataLinkerIt = mIntegratedDataLinker; integratedDataLinkerIt; integratedDataLinkerIt = integratedDataLinkerIt->getNext()) {
					integratedDataLinkerIt->resetIntegration();
				}
				mIntegratedBindingsCount = 0;
				mDisabledBindingsCount = 0;
				mIntegratedDataLinker = nullptr;
				mRemainingIntegrationRequirementCount = 0;
				return this;
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::disableIntegrations() {
				for (CAnsweringSplitPropagationTestingVariableBindingData* integratedDataLinkerIt = mIntegratedDataLinker; integratedDataLinkerIt; integratedDataLinkerIt = integratedDataLinkerIt->getNext()) {
					if (integratedDataLinkerIt->isDisabled()) {
						break;
					}
					integratedDataLinkerIt->disableIntegration();
					++mDisabledBindingsCount;
				}
				mIntegratedBindingsCount = 0;
				mIntegratedDataLinker = nullptr;
				mRemainingIntegrationRequirementCount = 0;
				return this;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getVariableDataIntegrationId() {
				return mVariableDataIntegrationId;
			}

			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::setVariableDataIntegrationId(cint64 id) {
				mVariableDataIntegrationId = id;
				return this;
			}


			bool CAnsweringSplitPropagationTestingVariableData::hasVariableDataIntegrationId() {
				return mVariableDataIntegrationId >= 0;
			}


			CAnsweringSplitPropagationTestingVariableData* CAnsweringSplitPropagationTestingVariableData::incVariableBindingsRemainingIntegrationRequirementCount(cint64 count) {
				mRemainingIntegrationRequirementCount += count;
				return this;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getVariableBindingsIntegrationCount() {
				return mIntegratedBindingsCount;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getVariableBindingsRemainingIntegrationRequirementCount() {
				return mRemainingIntegrationRequirementCount;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getVariableBindingsIntegrationLimit() {
				return mIntegrationBindingsLimit;
			}

			bool CAnsweringSplitPropagationTestingVariableData::hasVariableBiningsIntegrationLimitReached() {
				return mIntegrationBindingsLimit >= 0 && mIntegratedBindingsCount >= mIntegrationBindingsLimit;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getDisabledVariableBindingsCount() {
				return mDisabledBindingsCount;
			}

			bool CAnsweringSplitPropagationTestingVariableData::hasAllVariableBindingsDisabled() {
				if (mDisabledBindingsCount >= mBindingIndiDataHash.size()) {
					return true;
				}
				return false;
			}

			cint64 CAnsweringSplitPropagationTestingVariableData::getLastVariableBindingsIntegrationCount(bool updateLast) {
				cint64 tmpBindingsCount = mLastIntegratedBindingsCount;
				if (updateLast) {
					mLastIntegratedBindingsCount = mIntegratedBindingsCount;
				}
				return tmpBindingsCount;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
