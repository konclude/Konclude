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

#include "CRealizationMarkerCandidatesMessageData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realizer {


			CRealizationMarkerCandidatesMessageData::CRealizationMarkerCandidatesMessageData() {
				mMessageDataType = TELLREALIZATIONMARKERCANDIDATES;
			}


			CRealizationMarkerCandidatesMessageData* CRealizationMarkerCandidatesMessageData::initRealizationMarkerCandidatesMessageData(CIndividual* individual, CConcept* markerConcept, CXLinker<CIndividual*>* detIndiLinker, CXLinker<CIndividual*>* ndetIndiLinker) {
				mIndiviual = individual;
				mConcept = markerConcept;
				mDetIndiLinker = detIndiLinker;
				mNdetIndiLinker = ndetIndiLinker;
				return this;
			}

			CConcept* CRealizationMarkerCandidatesMessageData::getMarkerConcept() {
				return mConcept;
			}

			CIndividual* CRealizationMarkerCandidatesMessageData::getIndividual() {
				return mIndiviual;
			}


			CXLinker<CIndividual*>* CRealizationMarkerCandidatesMessageData::getDeterministicIndividualLinker() {
				return mDetIndiLinker;
			}

			CXLinker<CIndividual*>* CRealizationMarkerCandidatesMessageData::getNondeterministicIndividualLinker() {
				return mNdetIndiLinker;
			}


		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude
