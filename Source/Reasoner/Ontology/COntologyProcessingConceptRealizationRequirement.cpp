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

#include "COntologyProcessingConceptRealizationRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingConceptRealizationRequirement::COntologyProcessingConceptRealizationRequirement(CConcept* concept, bool realizeSubConcepts, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) : COntologyProcessingDynamicRealizationRequirement(requiredProcessingFlags,forbiddenProcessingFlags,requiredErrorFlags,forbiddenErrorFlags) {
				mConcept = concept;
				mIndividual = CIndividualReference();
				mRealizeSubConcepts = realizeSubConcepts;
			}

			COntologyProcessingConceptRealizationRequirement::COntologyProcessingConceptRealizationRequirement(const CIndividualReference& indi, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) : COntologyProcessingDynamicRealizationRequirement(requiredProcessingFlags, forbiddenProcessingFlags, requiredErrorFlags, forbiddenErrorFlags) {
				mConcept = nullptr;
				mIndividual = indi;
				mRealizeSubConcepts = false;
			}

			COntologyProcessingConceptRealizationRequirement::COntologyProcessingConceptRealizationRequirement(const CIndividualReference& indi, CConcept* concept, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) : COntologyProcessingDynamicRealizationRequirement(requiredProcessingFlags, forbiddenProcessingFlags, requiredErrorFlags, forbiddenErrorFlags) {
				mConcept = concept;
				mIndividual = indi;
				mRealizeSubConcepts = false;
			}

			CConcept* COntologyProcessingConceptRealizationRequirement::getConcept() {
				return mConcept;
			}

			CIndividualReference COntologyProcessingConceptRealizationRequirement::getIndividualReference() {
				return mIndividual;
			}

			const QString COntologyProcessingConceptRealizationRequirement::getRequirementFailureString(COntology* ontology) {
				QString failureString;
				if (!isRequirementSatisfied(ontology)) {
					failureString = QString("Realization of specified concept failed.");
				}
				return failureString;
			}
			

			bool COntologyProcessingConceptRealizationRequirement::realizeSubConcepts() {
				return mRealizeSubConcepts;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
