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

#include "COntologyProcessingComplexAnsweringRequirement.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyProcessingComplexAnsweringRequirement::COntologyProcessingComplexAnsweringRequirement(CComplexAnsweringQuery* query, cint64 requiredProcessingFlags, cint64 forbiddenProcessingFlags, cint64 requiredErrorFlags, cint64 forbiddenErrorFlags) {
				mRequiredProcessingFlags = requiredProcessingFlags;
				mForbiddenProcessingFlags = forbiddenProcessingFlags;
				mRequiredErrorFlags = requiredErrorFlags;
				mForbiddenErrorFlags = forbiddenErrorFlags;
				mQuery = query;
			}



			bool COntologyProcessingComplexAnsweringRequirement::isRequirementSatisfied(COntology* ontology) {
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

			bool COntologyProcessingComplexAnsweringRequirement::isDynamicRequirement() {
				return true;
			}


			COntologyProcessingComplexAnsweringRequirement* COntologyProcessingComplexAnsweringRequirement::submitRequirementUpdate(cint64 processingFlags, cint64 errorFlags) {
				mProcessingStatus.setProcessingFlags(processingFlags);
				mProcessingStatus.setErrorFlags(errorFlags);
				return this;
			}


			const QString COntologyProcessingComplexAnsweringRequirement::getRequirementFailureString(COntology* ontology) {
				QString failureString;
				if (!isRequirementSatisfied(ontology)) {
					failureString = QString("Answering of query failed.");
				}
				return failureString;
			}

			CComplexAnsweringQuery* COntologyProcessingComplexAnsweringRequirement::getQuery() {
				return mQuery;
			}



		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
