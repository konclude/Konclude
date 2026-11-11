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

#include "COntologyProcessingDataLiteralRealizationRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingDataLiteralRealizationRequirement::COntologyProcessingDataLiteralRealizationRequirement(CRole* dataRole, const CIndividualReference& indiRef, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) : COntologyProcessingDynamicRealizationRequirement(requiredProcessingFlags,forbiddenProcessingFlags,requiredErrorFlags,forbiddenErrorFlags) {
				mDataRole = dataRole;
				mIndividualReference = indiRef;
			}


			CRole* COntologyProcessingDataLiteralRealizationRequirement::getDataRole() {
				return mDataRole;
			}


			CIndividualReference COntologyProcessingDataLiteralRealizationRequirement::getIndividualReference() {
				return mIndividualReference;
			}


			const QString COntologyProcessingDataLiteralRealizationRequirement::getRequirementFailureString(COntology* ontology) {
				QString failureString;
				if (!isRequirementSatisfied(ontology)) {
					failureString = QString("Realization of the data values of the individual failed.");
				}
				return failureString;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude