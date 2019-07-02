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

#include "COntologyProcessingConceptSaturationRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingConceptSaturationRequirement::COntologyProcessingConceptSaturationRequirement(CConcept* concept, bool conceptNegation, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) {
				mConcept = concept;
				mConceptNegation = conceptNegation;
				mRequiredProcessingFlags = requiredProcessingFlags;
				mForbiddenProcessingFlags = forbiddenProcessingFlags;
				mRequiredErrorFlags = requiredErrorFlags;
				mForbiddenErrorFlags = forbiddenErrorFlags;
			}



			bool COntologyProcessingConceptSaturationRequirement::isRequirementSatisfied(COntology* ontology) {
				if (ontology) {
					if (!mProcessingStatus.hasPartialProcessingFlags(mRequiredProcessingFlags)) {
						return false;
					}
					if (mProcessingStatus.hasPartialProcessingFlags(mForbiddenProcessingFlags)) {
						return false;
					}
					if (!mProcessingStatus.hasPartialErrorFlags(mRequiredErrorFlags)) {
						return false;
					}
					if (mProcessingStatus.hasPartialErrorFlags(mForbiddenErrorFlags)) {
						return false;
					}
					return true;
				}
				return false;
			}

			bool COntologyProcessingConceptSaturationRequirement::isDynamicRequirement() {
				return true;
			}


			COntologyProcessingConceptSaturationRequirement* COntologyProcessingConceptSaturationRequirement::submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags) {
				mProcessingStatus.setProcessingFlags(processingFlags);
				mProcessingStatus.setErrorFlags(errorFlags);
				return this;
			}

			CConcept* COntologyProcessingConceptSaturationRequirement::getConcept() {
				return mConcept;
			}

			bool COntologyProcessingConceptSaturationRequirement::getConceptNegation() {
				return mConceptNegation;
			}


			const QString COntologyProcessingConceptSaturationRequirement::getRequirementFailureString(COntology* ontology) {
				QString failureString;
				if (!isRequirementSatisfied(ontology)) {
					failureString = QString("Saturation of specified concept failed.");
				}
				return failureString;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
