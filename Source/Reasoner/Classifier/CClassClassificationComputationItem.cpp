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

#include "CClassClassificationComputationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassClassificationComputationItem::CClassClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CConcept *satisfiableTestedConcept) {
				mSatCalcJob = satCalcJob;
				testValid = true;
				conceptSatisfiableTest = true;
				conceptSubsumptionTest = false;
				con0 = satisfiableTestedConcept;
				con1 = 0;
			}

			CClassClassificationComputationItem::CClassClassificationComputationItem(CSatisfiableCalculationJob* satCalcJob, CConcept *subsumerTestedConcept, CConcept *subsumedTestedConcept) {
				mSatCalcJob = satCalcJob;
				testValid = true;
				conceptSatisfiableTest = false;
				conceptSubsumptionTest = true;
				con0 = subsumerTestedConcept;
				con1 = subsumedTestedConcept;
			}

			CClassClassificationComputationItem::~CClassClassificationComputationItem() {
			}



			bool CClassClassificationComputationItem::isConceptSatisfiableTest() {
				return conceptSatisfiableTest;
			}

			bool CClassClassificationComputationItem::isConceptSubsumptionTest() {
				return conceptSubsumptionTest;
			}


			CConcept *CClassClassificationComputationItem::getSatisfiableTestedConcept() {
				if (conceptSatisfiableTest) {
					return con0;
				} 
				return 0;
			}

			CConcept *CClassClassificationComputationItem::getSubsumerTestedConcept() {
				if (conceptSubsumptionTest) {
					return con0;
				} 
				return 0;
			}

			CConcept *CClassClassificationComputationItem::getSubsumedTestedConcept() {
				if (conceptSubsumptionTest) {
					return con1;
				} 
				return 0;
			}

			bool CClassClassificationComputationItem::isTestValid() {
				return testValid;
			}

			CClassClassificationComputationItem *CClassClassificationComputationItem::setTestInvalid() {
				testValid = false;
				return this;
			}

			CSatisfiableCalculationJob* CClassClassificationComputationItem::getSatisfiableCalculationJob()
			{
				return mSatCalcJob;
			}


			bool CClassClassificationComputationItem::testIsTestRedundant(CTaxonomy *taxonomy) {
				bool redundant = false;
				if (conceptSatisfiableTest) {
					if (taxonomy->isSatisfiable(con0) || taxonomy->isNotSatisfiable(con0)) {
						redundant = true;
					}
				} else if (conceptSubsumptionTest) {
					if (taxonomy->isSubsumption(con0,con1) || taxonomy->isNotSubsumption(con0,con1)) {
						redundant = true;
					}
				}
				return redundant;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
