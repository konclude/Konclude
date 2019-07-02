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

#ifndef KONCLUDE_REASONER_REALIZER_CREALIZATIONMARKERCANDIDATESMESSAGEDATA_H
#define KONCLUDE_REASONER_REALIZER_CREALIZATIONMARKERCANDIDATESMESSAGEDATA_H

// Libraries includes


// Namespace includes
#include "RealizerSettings.h"
#include "CRealizationMessageData.h"
#include "CIndividualRoleCandidateTestingData.h"

// Other includes
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CIndividualReference.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realizer {


			/*! 
			 *
			 *		\class		CRealizationMarkerCandidatesMessageData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizationMarkerCandidatesMessageData : public CRealizationMessageData {
				// public methods
				public:
					//! Constructor
					CRealizationMarkerCandidatesMessageData();

					CRealizationMarkerCandidatesMessageData* initRealizationMarkerCandidatesMessageData(const CIndividualReference& individualReference, const CIndividualRoleCandidateTestingData& testingData, CConcept* markerConcept, CXLinker<CIndividualReference>* detIndiLinker, CXLinker<CIndividualReference>* ndetIndiLinker);

					CIndividualReference getIndividualReference();
					CConcept* getMarkerConcept();
					CIndividualRoleCandidateTestingData getIndividualRoleCandidateTestingData();
					CXLinker<CIndividualReference>* getDeterministicIndividualReferenceLinker();
					CXLinker<CIndividualReference>* getNondeterministicIndividualReferenceLinker();

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividualReference mIndiviualReference;
					CIndividualRoleCandidateTestingData mTestingData;
					CConcept* mConcept;
					CXLinker<CIndividualReference>* mDetIndiLinker;
					CXLinker<CIndividualReference>* mNdetIndiLinker;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realizer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZER_CREALIZATIONMARKERCANDIDATESMESSAGEDATA_H
