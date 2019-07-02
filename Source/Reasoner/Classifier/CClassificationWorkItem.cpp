/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CClassificationWorkItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CClassificationWorkItem::CClassificationWorkItem(CSatisfiableCalculationJob* satCalcJob, CConcept *satisfiableTestedConcept) {
				mSatCalcJob = satCalcJob;
				testValid = true;
				conceptSatisfiableTest = true;
				conceptSubsumptionTest = false;
				con0 = satisfiableTestedConcept;
				con1 = 0;
			}

			CClassificationWorkItem::CClassificationWorkItem(CSatisfiableCalculationJob* satCalcJob, CConcept *subsumerTestedConcept, CConcept *subsumedTestedConcept) {
				mSatCalcJob = satCalcJob;
				testValid = true;
				conceptSatisfiableTest = false;
				conceptSubsumptionTest = true;
				con0 = subsumerTestedConcept;
				con1 = subsumedTestedConcept;
			}

			CClassificationWorkItem::~CClassificationWorkItem() {
			}



			bool CClassificationWorkItem::isConceptSatisfiableTest() {
				return conceptSatisfiableTest;
			}

			bool CClassificationWorkItem::isConceptSubsumptionTest() {
				return conceptSubsumptionTest;
			}


			CConcept *CClassificationWorkItem::getSatisfiableTestedConcept() {
				if (conceptSatisfiableTest) {
					return con0;
				} 
				return 0;
			}

			CConcept *CClassificationWorkItem::getSubsumerTestedConcept() {
				if (conceptSubsumptionTest) {
					return con0;
				} 
				return 0;
			}

			CConcept *CClassificationWorkItem::getSubsumedTestedConcept() {
				if (conceptSubsumptionTest) {
					return con1;
				} 
				return 0;
			}

			bool CClassificationWorkItem::isTestValid() {
				return testValid;
			}

			CClassificationWorkItem *CClassificationWorkItem::setTestInvalid() {
				testValid = false;
				return this;
			}

			CSatisfiableCalculationJob* CClassificationWorkItem::getSatisfiableCalculationJob()
			{
				return mSatCalcJob;
			}


			bool CClassificationWorkItem::testIsTestRedundant(CTaxonomy *taxonomy) {
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
