/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CAnsweringHandlingStatistics.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			CAnsweringHandlingStatistics::CAnsweringHandlingStatistics(CAnsweringHandlingStatistics *nextPrecompuationStatistics) {
				resetValues();
			}


			qint64 CAnsweringHandlingStatistics::getTotalAnsweredQueriesCount() {
				return mTotalAnsweredQueries;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incTotalAnsweredQueriesCount(qint64 incCount) {
				mTotalAnsweredQueries += incCount;
				return this;
			}




			qint64 CAnsweringHandlingStatistics::getExistentialQueryPartAbsorptionCount() {
				return mAbsorbedExistentialQueryPartCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incExistentialQueryPartAbsorptionCount(qint64 incCount) {
				mAbsorbedExistentialQueryPartCount += incCount;
				return this;
			}



			qint64 CAnsweringHandlingStatistics::getCreatedComplexConceptItemsCount() {
				return mCreatedComplexConceptItemsCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incCreatedComplexConceptItemsCount(qint64 incCount) {
				mCreatedComplexConceptItemsCount += incCount;
				return this;
			}


			qint64 CAnsweringHandlingStatistics::getComplexConceptItemsPossibleInstanceVerificationCount() {
				return mComplexConceptItemsPossibleInstanceVerificationCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptItemsPossibleInstanceVerificationCount(qint64 incCount) {
				mComplexConceptItemsPossibleInstanceVerificationCount += incCount;
				return this;
			}


			qint64 CAnsweringHandlingStatistics::getComplexConceptItemsSatisfiabilityTestingCount() {
				return mComplexConceptItemsSatisfiabilityTestingCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptItemsSatisfiabilityTestingCount(qint64 incCount) {
				mComplexConceptItemsSatisfiabilityTestingCount += incCount;
				return this;
			}


			qint64 CAnsweringHandlingStatistics::getRequestedRealizationRequirementCount() {
				return mRequestedRealizationRequirementCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incRequestedRealizationRequirementCount(qint64 incCount) {
				mRequestedRealizationRequirementCount += incCount;
				return this;
			}



			qint64 CAnsweringHandlingStatistics::getComplexConceptItemsCandidatePropagationCount() {
				return mComplexConceptItemsCandidatePropagationCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptItemsCandidatePropagationCount(qint64 incCount) {
				mComplexConceptItemsCandidatePropagationCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getComplexConceptItemsSubClassSubsumptionTestingCount() {
				return mComplexConceptItemsSubClassSubsumptionTestingCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptItemsSubClassSubsumptionTestingCount(qint64 incCount) {
				mComplexConceptItemsSubClassSubsumptionTestingCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getComplexConceptItemsSuperClassSubsumptionTestingCount() {
				return mComplexConceptItemsSuperClassSubsumptionTestingCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptItemsSuperClassSubsumptionTestingCount(qint64 incCount) {
				mComplexConceptItemsSuperClassSubsumptionTestingCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getExistentialQueryPartEntailmentTestingCount() {
				return mExistentialQueryPartEntailmentTestingCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incExistentialQueryPartEntailmentTestingCount(qint64 incCount) {
				mExistentialQueryPartEntailmentTestingCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getExistentialQueryPartVariableBindingPropagationCount() {
				return mExistentialQueryPartVariableBindingPropagationCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incExistentialQueryPartVariableBindingPropagationCount(qint64 incCount) {
				mExistentialQueryPartVariableBindingPropagationCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getExistentialQueryPartVariableBindingConfirmationCount() {
				return mExistentialQueryPartVariableBindingConfirmationCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incExistentialQueryPartVariableBindingConfirmationCount(qint64 incCount) {
				mExistentialQueryPartVariableBindingConfirmationCount += incCount;
				return this;
			}

			qint64 CAnsweringHandlingStatistics::getComplexConceptAssertionEntailmentTestingCount() {
				return mComplexConceptAssertionEntailmentTestingCount;
			}

			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::incComplexConceptAssertionEntailmentTestingCount(qint64 incCount) {
				mComplexConceptAssertionEntailmentTestingCount += incCount;
				return this;
			}


			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::resetValues() {
				mTotalAnsweredQueries = 0;
				mAbsorbedExistentialQueryPartCount = 0;
				mCreatedComplexConceptItemsCount = 0;
				mComplexConceptItemsPossibleInstanceVerificationCount = 0;
				mComplexConceptItemsSatisfiabilityTestingCount = 0;
				mRequestedRealizationRequirementCount = 0;
				mComplexConceptItemsCandidatePropagationCount = 0;
				mComplexConceptItemsSubClassSubsumptionTestingCount = 0;
				mComplexConceptItemsSuperClassSubsumptionTestingCount = 0;
				mExistentialQueryPartEntailmentTestingCount = 0;
				mExistentialQueryPartVariableBindingPropagationCount = 0;
				mExistentialQueryPartVariableBindingConfirmationCount = 0;
				mComplexConceptAssertionEntailmentTestingCount = 0;
				return this;
			}


			CAnsweringHandlingStatistics *CAnsweringHandlingStatistics::appendStatistics(CAnsweringHandlingStatistics *statistics) {
				mTotalAnsweredQueries += statistics->getTotalAnsweredQueriesCount();
				mAbsorbedExistentialQueryPartCount += statistics->getExistentialQueryPartAbsorptionCount();
				mCreatedComplexConceptItemsCount += statistics->getCreatedComplexConceptItemsCount();
				mComplexConceptItemsPossibleInstanceVerificationCount += statistics->getComplexConceptItemsPossibleInstanceVerificationCount();
				mComplexConceptItemsSatisfiabilityTestingCount += statistics->getComplexConceptItemsSatisfiabilityTestingCount();
				mRequestedRealizationRequirementCount += statistics->getRequestedRealizationRequirementCount();
				mComplexConceptItemsCandidatePropagationCount += statistics->getComplexConceptItemsCandidatePropagationCount();
				mComplexConceptItemsSubClassSubsumptionTestingCount += statistics->getComplexConceptItemsSubClassSubsumptionTestingCount();
				mComplexConceptItemsSuperClassSubsumptionTestingCount += statistics->getComplexConceptItemsSuperClassSubsumptionTestingCount();
				mExistentialQueryPartEntailmentTestingCount += statistics->getExistentialQueryPartEntailmentTestingCount();
				mExistentialQueryPartVariableBindingPropagationCount += statistics->getExistentialQueryPartVariableBindingPropagationCount();
				mExistentialQueryPartVariableBindingConfirmationCount += statistics->getExistentialQueryPartVariableBindingConfirmationCount();
				mComplexConceptAssertionEntailmentTestingCount += statistics->getComplexConceptAssertionEntailmentTestingCount();
				return this;
			}


		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude
