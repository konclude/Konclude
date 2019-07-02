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

#include "CSaturationConceptReferenceLinking.h"


namespace Konclude {

	namespace Reasoner {

		namespace Consistiser {


			CSaturationConceptReferenceLinking::CSaturationConceptReferenceLinking() {
				mPotentiallyExistInitConcept = false;
				mDataRangeConcept = false;
				mIndiProcessNodeForConcept = nullptr;
			}

			bool CSaturationConceptReferenceLinking::isPotentiallyExistInitializationConcept() {
				return mPotentiallyExistInitConcept;
			}

			CSaturationConceptReferenceLinking* CSaturationConceptReferenceLinking::setPotentiallyExistInitializationConcept(bool initConcept) {
				mPotentiallyExistInitConcept = initConcept;
				return this;
			}

			CSaturationConceptReferenceLinking* CSaturationConceptReferenceLinking::setIndividualProcessNodeForConcept(CProcessReference* indiNode) {
				mIndiProcessNodeForConcept = indiNode;
				return this;
			}

			CProcessReference* CSaturationConceptReferenceLinking::getIndividualProcessNodeForConcept() {
				return mIndiProcessNodeForConcept;
			}

			bool CSaturationConceptReferenceLinking::isDataRangeConcept() {
				return mDataRangeConcept;
			}

			CSaturationConceptReferenceLinking* CSaturationConceptReferenceLinking::setDataRangeConcept(bool dataRangeConcept) {
				mDataRangeConcept = dataRangeConcept;
				return this;
			}


		}; // end namespace Consistiser

	}; // end namespace Reasoner

}; // end namespace Konclude
