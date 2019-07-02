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

#include "CClassConceptClassification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CClassConceptClassification::CClassConceptClassification() {
				mClassConceptTaxonomy = nullptr;
				mStatsColl = nullptr;
				mIndiDepTracking = nullptr;
				mIndiDepTrackingSet = false;
			}

			CClassConceptClassification::~CClassConceptClassification() {
			}


			bool CClassConceptClassification::hasClassConceptTaxonomy() {
				return mClassConceptTaxonomy != nullptr;
			}

			CTaxonomy* CClassConceptClassification::getClassConceptTaxonomy() {
				return mClassConceptTaxonomy;
			}

			CClassConceptClassification* CClassConceptClassification::setClassConceptTaxonomy(CTaxonomy* taxonomy) {
				mClassConceptTaxonomy = taxonomy;
				return this;
			}


			bool CClassConceptClassification::hasClassificationStatistics() {
				return mStatsColl != nullptr;
			}

			CClassificationStatistics* CClassConceptClassification::getClassificationStatistics() {
				return mStatsColl;
			}

			CClassConceptClassification* CClassConceptClassification::setClassificationStatistics(CClassificationStatistics* statColl) {
				mStatsColl = statColl;
				return this;
			}


			bool CClassConceptClassification::hasDependentIndividualsTracking() {
				return mIndiDepTracking != nullptr;
			}

			CIndividualDependenceTracking* CClassConceptClassification::getDependentIndividualsTracking() {
				return mIndiDepTracking;
			}

			CClassConceptClassification* CClassConceptClassification::setDependentIndividualsTracking(CIndividualDependenceTracking* indiDepTracking) {
				mIndiDepTracking = indiDepTracking;
				mIndiDepTrackingSet = true;
				return this;
			}

			bool CClassConceptClassification::isIndividualDependentTrackingSet() {
				return mIndiDepTrackingSet;
			}

		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude
