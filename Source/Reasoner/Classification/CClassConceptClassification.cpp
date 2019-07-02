/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#include "CClassConceptClassification.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classification {


			CClassConceptClassification::CClassConceptClassification() {
				mClassConceptTaxonomy = nullptr;
				mStatsColl = nullptr;
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


		}; // end namespace Classification

	}; // end namespace Reasoner

}; // end namespace Konclude
