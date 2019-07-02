/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#include "CIncrementalKPSetClassOntologyClassificationItem.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			CIncrementalKPSetClassOntologyClassificationItem::CIncrementalKPSetClassOntologyClassificationItem(CConfigurationBase *configuration, CClassifierStatistics *nextClassificationStatistics) 
					: COptimizedKPSetClassOntologyClassificationItem(configuration,nextClassificationStatistics) {

				mPreviousHierarchyReused = false;
				mReusedClassConClassification = nullptr;

				mPreviousReusedClassCalculationCount = 0;
				mNewInitializedClassCalculationCount = 0;
			}

			CIncrementalKPSetClassOntologyClassificationItem::~CIncrementalKPSetClassOntologyClassificationItem() {
			}


			CIncrementalKPSetClassOntologyClassificationItem* CIncrementalKPSetClassOntologyClassificationItem::setPreviousHierarchyReused(bool reused) {
				mPreviousHierarchyReused = reused;
				return this;
			}

			bool CIncrementalKPSetClassOntologyClassificationItem::isPreviousHierarchyReused() {
				return mPreviousHierarchyReused;
			}


			CClassConceptClassification* CIncrementalKPSetClassOntologyClassificationItem::getPreviousReusedClassClassification() {
				return mReusedClassConClassification;
			}

			CClassConceptClassification* CIncrementalKPSetClassOntologyClassificationItem::setPreviousReusedClassClassification(CClassConceptClassification* reusedClassClassification) {
				return mReusedClassConClassification;
			}


			cint64 CIncrementalKPSetClassOntologyClassificationItem::getPreviousReusedClassCalculationCount() {
				return mPreviousReusedClassCalculationCount;
			}

			cint64 CIncrementalKPSetClassOntologyClassificationItem::getNewInitializedClassCalculationCount() {
				return mNewInitializedClassCalculationCount;
			}

			CIncrementalKPSetClassOntologyClassificationItem* CIncrementalKPSetClassOntologyClassificationItem::setPreviousReusedClassCalculationCount(cint64 count) {
				mPreviousReusedClassCalculationCount = count;
				return this;
			}

			CIncrementalKPSetClassOntologyClassificationItem* CIncrementalKPSetClassOntologyClassificationItem::incPreviousReusedClassCalculationCount(cint64 incCount) {
				mPreviousReusedClassCalculationCount += incCount;
				return this;
			}

			CIncrementalKPSetClassOntologyClassificationItem* CIncrementalKPSetClassOntologyClassificationItem::setNewInitializedClassCalculationCount(cint64 count) {
				mNewInitializedClassCalculationCount = count;
				return this;
			}

			CIncrementalKPSetClassOntologyClassificationItem* CIncrementalKPSetClassOntologyClassificationItem::incNewInitializedClassCalculationCount(cint64 incCount) {
				mNewInitializedClassCalculationCount += incCount;
				return this;
			}



		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude
