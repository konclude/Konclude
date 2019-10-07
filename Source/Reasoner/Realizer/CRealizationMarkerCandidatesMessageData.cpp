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

#include "CRealizationMarkerCandidatesMessageData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizationMarkerCandidatesMessageData::CRealizationMarkerCandidatesMessageData() {
				mMessageDataType = TELLREALIZATIONMARKERCANDIDATES;
			}


			CRealizationMarkerCandidatesMessageData* CRealizationMarkerCandidatesMessageData::initRealizationMarkerCandidatesMessageData(const CIndividualReference& individualReference, const CIndividualRoleCandidateTestingData& testingData, CConcept* markerConcept, CXLinker<CIndividualReference>* detIndiLinker, CXLinker<CIndividualReference>* ndetIndiLinker) {
				mIndiviualReference = individualReference;
				mTestingData = testingData;
				mConcept = markerConcept;
				mDetIndiLinker = detIndiLinker;
				mNdetIndiLinker = ndetIndiLinker;
				return this;
			}

			CConcept* CRealizationMarkerCandidatesMessageData::getMarkerConcept() {
				return mConcept;
			}

			CIndividualReference CRealizationMarkerCandidatesMessageData::getIndividualReference() {
				return mIndiviualReference;
			}


			CXLinker<CIndividualReference>* CRealizationMarkerCandidatesMessageData::getDeterministicIndividualReferenceLinker() {
				return mDetIndiLinker;
			}

			CXLinker<CIndividualReference>* CRealizationMarkerCandidatesMessageData::getNondeterministicIndividualReferenceLinker() {
				return mNdetIndiLinker;
			}


			CIndividualRoleCandidateTestingData CRealizationMarkerCandidatesMessageData::getIndividualRoleCandidateTestingData() {
				return mTestingData;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
